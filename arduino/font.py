import freetype
face = freetype.Face('/usr/share/fonts/liberation/LiberationSans-Regular.ttf')
face.set_pixel_sizes(0, 20)

chars = []
fontheader = []
fontindices = []
fontdata = []

# Iterate through all characters we want to make available to the Arduino program
for c in range(32, 127):
    chars.append(chr(c))
    face.load_char(chr(c), freetype.FT_LOAD_RENDER |
                   freetype.FT_LOAD_TARGET_NORMAL)
    slot = face.glyph
    fontheader.extend([slot.bitmap.rows, slot.bitmap.width, slot.bitmap_left, slot.bitmap_top, slot.advance.x >> 6])
    fontindices.append(len(fontdata))
    if slot.bitmap.pitch != slot.bitmap.width:
        raise Exception('Pitch off')

    buffer = [x // 8 for x in slot.bitmap.buffer]
    for i in range(0, (3 - len(buffer) % 3) % 3):
        buffer.append(0)
    for i in range(0, len(buffer), 3):
        x = (buffer[i] << 10) + (buffer[i+1] << 5) + buffer[i+2]
        fontdata.append(x)
with open('font.h', 'w') as f:
    print('const int8_t FONTHEAD[] = {' + ', '.join(str(i) for i in fontheader) + '};', file=f)
    print('const uint16_t FONTIDX[] PROGMEM = {' + ', '.join(str(i) for i in fontindices) + '};', file=f)
    print('const uint16_t FONTDATA[] PROGMEM = {' + ', '.join(hex(i) for i in fontdata) + '};', file=f)
print('characters', chars)
print('fonthead', len(fontheader)/1000, 'kb')
print('fontidx', 2*len(fontindices)/1000, 'kb')
print('fontdata', 2*len(fontdata)/1000, 'kb')
print('total', (len(fontheader) + 4*len(fontindices) + 2*len(fontdata))/1000, 'kb')
