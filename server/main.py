import asyncio
import asyncws
from json import load, loads, dump, dumps
from serial_asyncio import open_serial_connection
from sanic import Sanic
from sanic.log import logger
from sanic.response import json, text, empty
from sanic_validation import validate_json

app = Sanic(name='outdoortouchscreen', load_env='APP_')
app.config.SERIAL_PORT = '/dev/ttyACM0'
app.config.HA_URL = 'ws://ubuge1:8123/api/websocket'
app.config.STATES_FILE = 'state.json'
app.config.NUM_BUTTONS = 12
app.config.DOMAINS = ['light', 'automation', 'media_player', 'climate', 'switch']
state_schema = {
    'type': 'dict',
    'schema': {
        'id': { 'type': 'string', 'required': True },
        'name': { 'type': 'string', 'required': True }
    }
}
states_schema = { f'{n:x}': state_schema for n in range(app.config.NUM_BUTTONS) }

# Load database of button -> state mappings from filesystem
try:
    with open(app.config.STATES_FILE) as f:
        states = load(f)
except:
    logger.error('Using empty states database')
onoffs = {} # Initial button states
ha_states = {} # States from home assistant

# Configure each url the app will respond to
app.static('/', '../web/public/index.html')
app.static('/', '../web/public')

@app.get('/states')
async def get_states(request):
    return json(states)

@app.get('/states.js')
async def get_statesjs(request):
    return text('window.states=' + dumps(states), content_type='application/javascript')

def format_state(s):
    return {
        'id': s['entity_id'],
        'name': s['attributes'].get('friendly_name', s['entity_id']),
        'on': s['state'] == 'on'
    }

@app.get('/hastates')
async def get_hastates(request):
    return json([format_state(s)  for s in ha_states.values()
                 if s['entity_id'].split('.')[0] in app.config.DOMAINS])

@app.post('/states')
@validate_json(states_schema, clean=True)
async def post_states(request, valid_json):
    global states
    states = valid_json
    with open(app.config.STATES_FILE, 'w') as f:
        dump(states, f)
    update_titles(sw)
    update_states(sw)
    return empty()

async def websocket():
    """Initialize the websocket that talks to home assistant."""
    ws = await asyncws.connect(app.config.HA_URL)
    await ws.send(dumps({ 'type': 'auth',
                          'access_token': app.config.ACCESS_TOKEN }))
    await ws.send(dumps({ 'id': 1, 'type': 'subscribe_events',
                          'event_type': 'state_changed' }))
    await ws.send(dumps({ 'id': 2, 'type': 'get_states' }))
    return ws

async def serial():
    """Initialize the serial connection with the touchscreen."""
    reader, writer = await open_serial_connection(url=app.config.SERIAL_PORT, baudrate=9600)
    logger.info('Created serial connection')
    update_titles(writer)
    return reader, writer

def update_titles(sw):
    """Update all button titles displayed on the touchscreen."""
    for num in range(app.config.NUM_BUTTONS):
        sw.write(f'q{num:x}\n'.encode())

def update_states(sw):
    """Updates all button states indicated on the touchscreen."""
    for num, state in states.items():
        if state['id'] not in ha_states:
            continue
        ha_state = (ha_states[state['id']]['state'] == 'on')
        if num not in onoffs or onoffs[num] != ha_state:
            onoffs[num] = ha_state
            logger.info('Updating state %s to %s', num, ha_state)
            stateletter = 't' if ha_state else 'f'
            sw.write(f's{num}{stateletter}\n'.encode())

async def handle_ws(message, sw):
    """Handle a new message from the home assistant websocket."""
    message = loads(message)
    if 'id' not in message:
        logger.warning('Message %s has no id', message)
        return
    if message['id'] == 1 and message['type'] == 'event':
        # A state changed (from subscribing to events)
        data = message['event']['data']
        ha_states[data['entity_id']] = data['new_state']
        update_states(sw)
    if message['id'] == 2:
        logger.info('Fetched states from home assistant')
        for state in message['result']:
            ha_states[state['entity_id']] = state
        update_states(sw)

wsid = 2
async def handle_ser(message, sw, ws):
    """Handle a new message from the touchscreen serial connection."""
    global wsid
    num = message[1]
    if message.startswith('t'):
        device_title = message[2:]
        if num in states and device_title != states[num]['name']:
            logger.info('Updating title from %s to %s', device_title, states[num]['name'])
            sw.write(f"t{num}{states[num]['name']}\n".encode())
    if message.startswith('s'):
        is_on = (message[2] == 't')
        if num in onoffs and onoffs[num] == is_on:
            return
        onoffs[num] = is_on
        if num in states:
            logger.info('Calling service')
            wsid += 1
            await ws.send(dumps({
                'id': wsid,
                'type': 'call_service',
                'domain': states[num]['id'].split('.')[0],
                'service': 'turn_on' if is_on else 'turn_off',
                'service_data': {
                    'entity_id': states[num]['id']
                }
            }))

async def messaging():
    """Receives messages from both the websocket and touchscreenn and responds to them."""
    global sw
    sr, sw = await serial()
    ws = await websocket()
    wstask = asyncio.create_task(ws.recv())
    sertask = asyncio.create_task(sr.readuntil(b'\n'))
    pending = { wstask, sertask }
    while True:
        done, pending = await asyncio.wait(pending, return_when=asyncio.FIRST_COMPLETED)
        if wstask in done:
            message = wstask.result()
            wstask = asyncio.create_task(ws.recv())
            pending.add(wstask)
            if message is None:
                raise Exception('Websocket connection has closed')
            try:
                if len(message) < 1000:
                    logger.info(message)
                await handle_ws(message, sw)
            except Exception as e:
                logger.error(e)
        if sertask in done:
            message = sertask.result()
            sertask = asyncio.create_task(sr.readuntil(b'\n'))
            pending.add(sertask)
            try:
                message = message.strip().decode()
                logger.info(message)
                await handle_ser(message, sw, ws)
            except Exception as e:
                logger.error(e)

async def main():
    # Start both the server and the websocket coroutines independently
    server = app.create_server(host='0.0.0.0', port=8000, return_asyncio_server=True)
    messagingtask = asyncio.create_task(messaging())
    await asyncio.wait([server, messagingtask])

if __name__ == "__main__":
    asyncio.run(main())
