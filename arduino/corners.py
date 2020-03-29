from PIL import Image, ImageDraw

SUPER_SIZE = 200 # Resolution to draw supersampled images at

def color(tupple):
    r, g, b = tupple
    return min(round(r/8) << 11, 31<<11) + min(round(g/4) << 5, 63<<5) + min(round(b/8), 31)

def quarter_circle(fg, bg, r):
    image = Image.new('RGB', (SUPER_SIZE, SUPER_SIZE), color=bg)
    draw = ImageDraw.Draw(image)
    draw.ellipse((0, 0, 2*SUPER_SIZE, 2*SUPER_SIZE), fill = fg)

    image.thumbnail((r, r), Image.ANTIALIAS)
    pixels = image.load()
    colors = []
    for y in range(r):
        colors.append([color(pixels[x, y]) for x in range(r)])
    return colors

def circle_str(name, fg, br, r):
    colors = quarter_circle(fg, br, r)
    s = 'uint16_t ' + name + (f'[{r}]')*2 + ' = {\n'
    for row in colors:
        s += '    {' + ', '.join('0x{:04x}'.format(c) for c in row) + '},\n'
    f.write(s + '};\n')

def def_str(name, clr):
    f.write(f'#define {name} 0x{color(clr):04x}\n')

GREY = (56, 56, 56)
LTGREY = (184, 184, 184)
DKGREY = (64, 64, 64)
GREEN = (0, 240, 64)
f = open('corners.h', 'w')

def_str('GREY', GREY)
def_str('LTGREY', LTGREY)
def_str('DKGREY', DKGREY)
def_str('GREEN', GREEN)
circle_str('CORNER_WHITE', 'white', 'black', 4)
circle_str('CORNER_GREY', GREY, 'black', 4)
circle_str('CORNER_LTGREY', LTGREY, 'black', 4)
circle_str('CORNER_BLACK_HG', 'black', LTGREY, 4)

circle_str('CORNER_BLACK_LG', DKGREY, LTGREY, 2)
circle_str('CORNER_BLACK_SM', DKGREY, LTGREY, 1)
circle_str('CORNER_GREEN_LG', GREEN, 'white', 2)
circle_str('CORNER_GREEN_SM', GREEN, 'white', 1)

f.close()
