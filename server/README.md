# Backend code

The server has three responsibilities:
- Respond to HTTP requests with `sanic` (so the website can function)
- Communicate with the touchpad over serial with `serial_asyncio` (so pressing butons on the touchpad does something)
- Communicate with Home Assistant using `asyncws` (so home assistant and the touchpad are synced)

You'll notice there's a lot of `async` in this list. That's how the code is structured: 3 loops (web, serial, and websocket) all running asynchronously.

## Running

This project using Pipenv to manage dependencies. Run `pipenv install` to install packages from the Pipfile.
Once you have an access token from Home Assistant, run `APP_ACCESS_TOKEN=token pipenv run python3 main.py` to launch the server. It will run on `localhost:8000`.

The Arduino touchscreen is to be connected over USB. The device path defaults to `/dev/ttyACM0`, but this can be changed using the `APP_SERIAL_PORT` environment variable.
