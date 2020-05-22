/** Touchscreen interface

== PROTOCOL ==
All messages are sent in the following format:
X#DATA | x is a message type, # represents an optional button number,
          and DATA is data to send along with the message
Each message ends with a newline (unix-style i.e. \n). Thus, there
is a one-to-one correspondence between lines and messages.
Note: messages are also processed with 'r'.
Both messages to and from the button pad follow this protocol.

= DATA TYPES =
Button numbers are sent as a single hexadecimal digit (0 - B).
Booleans are reprsented by either 't' or 'f'. Using any other letter
will result in an error.
Strings are sent as-is.

= SENT COMMANDS =
i        | Program has started.
s#B      | Button with number # was set to the boolean B.
eMESSAGE | Error with message string message.
t#TITLE  | Inform that button with number # has title TITLE.

= RECEIVED COMMANDS =
s#B      | Set button with number # to the boolean B.
t#TITLE  | Set the title of button # to TITLE, where TITLE is a string.
q#       | Query the title of button with number #.
*/

// the regular Adafruit "TouchScreen.h" library only works on AVRs

// different mcufriend shields have Touchscreen on different pins
// and rotation.
// Run the TouchScreen_Calibr_native sketch for calibration of your shield

#include "MCUFRIEND_kbv.h"
#include "font.h"
#include "corners.h"
MCUFRIEND_kbv tft;       // hard-wired for UNO shields anyway.
#include "TouchScreen_kbv.h"
#include <EEPROM.h>

// touchscreen constants
const int XP=8,XM=A2,YP=A3,YM=9; //320x480 ID=0x9486
const int TS_LEFT=112,TS_RT=909,TS_TOP=957,TS_BOT=90;

TouchScreen_kbv ts = TouchScreen_kbv(XP, YP, XM, YM, 300);
TSPoint_kbv tp;
String command;

#define MINPRESSURE 100
#define MAXPRESSURE 1000
#define MINDURATION 50
#define MAXDURATION 3000

#define MAXTITLE 15 // Max title length

uint16_t ID, oldcolor, currentcolor;
uint8_t Orientation = 3;    //LANDSCAPE

/* uint16_t CORNER_WHITE[4][4] = { */
/*                                {0x0000, 0x2104, 0xbdf7, 0xef7d}, */
/*                                {0x2104, 0xef7d, 0xffff, 0xffff}, */
/*                                {0xbdf7, 0xffff, 0xffff, 0xffff}, */
/*                                {0xef7d, 0xffff, 0xffff, 0xffff}, */
/* }; */

/* uint16_t CORNER_GREY[4][4] = { */
/*                               {0x0000, 0x0000, 0x10a2, 0x2b2b}, */
/*                               {0x0000, 0x18e3, 0x39c7, 0x39c7}, */
/*                               {0x10a2, 0x39c7, 0x39c7, 0x39c7}, */
/*                               {0x2b2b, 0x39c7, 0x39c7, 0x39c7}, */
/* }; */

/* uint16_t CORNER_BLACK_HG[4][4] = { */
/*                               {0xffff, 0xffff, 0xdefb, 0x4208}, */
/*                               {0xffff, 0xdefb, 0x0000, 0x0000}, */
/*                               {0xdefb, 0x0000, 0x0000, 0x0000}, */
/*                               {0x4208, 0x0000, 0x0000, 0x0000}, */
/* }; */

/* uint16_t CORNER_BLACK_LG[2][2] = { */
/*                             {0xbdd7, 0x5acb}, */
/*                             {0x5acb, 0x4208}, */
/* }; */
/* uint16_t CORNER_BLACK_SM[1][1] = { */
/*                             {0xa514}, */
/* }; */

char titles[12][MAXTITLE];
boolean states[12] = {false, true, false, false, false, true, false, false, false, false, true, false};

#define BLACK   0x0000
#define WHITE   0xffff
/* #define GREY    0x39c7 */
/* #define DKGREY  0x4208 */
/* #define LTGREY  0xbdf7 // 0xef7d */

#define RED      0x9186
#define GREEN    0x0788
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0

#define BOXWIDTH 160
#define BOXHEIGHT 80
#define BOXPAD 10

#define BUTWIDTH BOXWIDTH-2*BOXPAD
#define BUTHEIGHT BOXHEIGHT-2*BOXPAD

void drawButtons() {
    int titlei = 0;
    for (int i = BOXPAD; i < tft.width(); i += BOXWIDTH) {
        for (int j = BOXPAD; j < tft.height(); j += BOXHEIGHT) {
            button(i, j, titlei++);
        }
    }
}

void setup() {
    tft.reset();
    ID = tft.readID();
    tft.begin(ID);
    tft.setRotation(Orientation);
    tft.fillScreen(BLACK);

    Serial.begin(9600);
    Serial.println('i');
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < MAXTITLE; j++) {
            char c = EEPROM.read(i * MAXTITLE + j);
            titles[i][j] = c;
            if (c == '\0') break;
        }
    }

    drawButtons();
}

int xbox = -1;
int ybox = -1;
unsigned long lastPress = -1;

void touch() {
    int title = xbox*4 + ybox;
    states[title] = !states[title];
    Serial.print('s');
    Serial.print(title, HEX);
    Serial.println(states[title] ? 't' : 'f');
    Serial.flush();

    int butx = xbox*BOXWIDTH+BOXPAD;
    int buty = ybox*BOXHEIGHT+BOXPAD;
    if (states[title]) { // Enabling
        for (int step = 0; step <= 255; step += 51) {
            uint16_t col = (step/8 << 11) + (step/4 << 5) + (step/8);
            tft.fillRect(butx+4, buty+4, BUTWIDTH-8, BUTHEIGHT-8, col);
            if (step == 0) buttonTitle(butx, buty, xbox*4 + ybox, false);
        }
        roundedRectangle(butx, buty, BUTWIDTH, BUTHEIGHT,
                         (uint16_t *)CORNER_WHITE, 4, WHITE);
    } else { //Disabling
        for (int step = 255; step > 0; step -= 51) {
            uint16_t col = (step/8 << 11) + (step/4 << 5) + (step/8);
            tft.fillRect(butx+4, buty+4, BUTWIDTH-8, BUTHEIGHT-8, col);
            if (step == 255) buttonTitle(butx, buty, xbox*4 + ybox, true);
        }
        roundedRectangle(butx, buty, BUTWIDTH, BUTHEIGHT,
                         (uint16_t *)CORNER_LTGREY, 4, LTGREY);
        roundedRectangle(butx+4, buty+4, BUTWIDTH-8, BUTHEIGHT-8,
                         (uint16_t *)CORNER_BLACK_HG, 4, BLACK);
    }
    buttonExtra(butx, buty, title);
    buttonTitle(butx, buty, title, states[title]);

    // Send a second time just to make sure it's received
    Serial.print('s');
    Serial.print(title, HEX);
    Serial.println(states[title] ? 't' : 'f');
    Serial.flush();
}

void processCommand() {
    switch (command[0]) {
    case 's': {
        int title = hexToButton(command[1]);
        if (title < 0) break;
        if (command[2] != 't' && command[2] != 'f') {
            Serial.println("eInvalid boolean");
            break;
        }
        boolean newState = (command[2] == 't');
        if (states[title] != newState) {
          states[title] = newState;
          autoButton(title);
        }
        break;
    }
    case 't': {
        int title = hexToButton(command[1]);
        if (title < 0) break;
        command.substring(2).toCharArray(titles[title], MAXTITLE);
        int i;
        for (i = 0; i < MAXTITLE && titles[title][i] != '\0'; i++) {
            EEPROM.write(title * MAXTITLE + i, titles[title][i]);
        }
        EEPROM.write(title * MAXTITLE + i, '\0');
        autoButton(title);
        break;
    }
    case 'q': {
        int title = hexToButton(command[1]);
        if (title < 0) break;
        Serial.print('t');
        Serial.print(title, HEX);
        Serial.println(titles[title]);
        break;
    }
    default:
        Serial.print("eUnkown command \"");
        Serial.print(command[0]);
        Serial.print("\" in \"");
        Serial.print(command);
        Serial.println("\"");
    }
    Serial.flush();
}

void loop()
{
    if (Serial.available()) {
        char r = Serial.read();
        if (r == '\n' || r == '\r') {
            if (command.length() > 0) processCommand();
            command = "";
        } else {
            command += r;
        }
    }

    uint16_t xpos, ypos;  //screen coordinates
    tp = ts.getPoint();   //tp.x, tp.y are ADC values

    // if Sharing pins, you'll need to fix the directions of the touchscreen pins
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE) {
            // most mcufriend have touch (with icons) that extends below the TFT
        // screens without icons need to reserve a space for "erase"
        // scale the ADC values from ts.getPoint() to screen values e.g. 0-239
        //
        // Calibration is true for PORTRAIT. tp.y is always long dimension
        // map to your current pixel orientation
        switch (Orientation) {
            case 0:
                xpos = map(tp.x, TS_LEFT, TS_RT, 0, tft.width());
                ypos = map(tp.y, TS_TOP, TS_BOT, 0, tft.height());
                break;
            case 1:
                xpos = map(tp.y, TS_TOP, TS_BOT, 0, tft.width());
                ypos = map(tp.x, TS_RT, TS_LEFT, 0, tft.height());
                break;
            case 2:
                xpos = map(tp.x, TS_RT, TS_LEFT, 0, tft.width());
                ypos = map(tp.y, TS_BOT, TS_TOP, 0, tft.height());
                break;
            case 3:
                xpos = map(tp.y, TS_BOT, TS_TOP, 0, tft.width());
                ypos = map(tp.x, TS_LEFT, TS_RT, 0, tft.height());
                break;
        }

        int newx = xpos / BOXWIDTH;
        int newy = ypos / BOXHEIGHT;
        if (newx >= 3 || newy >= 4) return;

        if (xpos / BOXWIDTH != xbox || ypos / BOXHEIGHT != ybox) {
            xbox = xpos / BOXWIDTH;
            ybox = ypos / BOXHEIGHT;
            lastPress = millis();
        }

        /* unsigned long duration = millis() - lastPress; */
        /* if (duration > MINDURATION && duration < MAXDURATION) { */
        touch();
        /* } */
    }
}

void autoButton(int title) {
    int butx = title / 4;
    int buty = title % 4;
    button(butx * BOXWIDTH + BOXPAD, buty * BOXHEIGHT + BOXPAD, title);
}

void button(uint16_t x, uint16_t y, int title) {
    roundedRectangle(x+3, y+6, BUTWIDTH, BUTHEIGHT,
                     (uint16_t *)CORNER_GREY, 4, GREY);
    roundedRectangle(x, y, BUTWIDTH, BUTHEIGHT,
                     (uint16_t *)(states[title] ? CORNER_WHITE : CORNER_LTGREY), 4, states[title] ? WHITE : LTGREY);
    if (!states[title]) {
        roundedRectangle(x+4, y+4, BUTWIDTH-8, BUTHEIGHT-8,
                         (uint16_t *)CORNER_BLACK_HG, 4, BLACK);
    }
    buttonExtra(x, y, title);
    /* tft.setTextColor(WHITE); */
    /* tft.setCursor(x+23, y + 22); */
    /* tft.setTextSize(2); */
    /* tft.println("A button"); */
    buttonTitle(x, y, title, states[title]);
}

void buttonExtra(uint16_t x, uint16_t y, int title) {
    if (states[title]) {
        roundedRectangle(x+8, y+2, BUTWIDTH-16, 3, (uint16_t *)CORNER_GREEN_SM, 1, GREEN);
        roundedRectangle(x+5, y+54, BUTWIDTH-10, 4, (uint16_t *)CORNER_GREEN_LG, 2, GREEN);
    } else {
        roundedRectangle(x+8, y+2, BUTWIDTH-16, 3, (uint16_t *)CORNER_BLACK_SM, 1, DKGREY);
        roundedRectangle(x+5, y+54, BUTWIDTH-10, 4, (uint16_t *)CORNER_BLACK_LG, 2, DKGREY);
    }
}

void buttonTitle(uint16_t x, uint16_t y, int title, boolean inverse) {
    drawString(titles[title], x+70 - textWidth(titles[title])/2, y + 38, inverse);
}

/** Draw an wxh image at (x,y) given directons */
void drawPixmap(uint16_t *image, uint16_t x, uint16_t y, uint16_t w, uint16_t h, boolean xfor, boolean yfor) {
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            if (xfor) {
                if (yfor) tft.drawPixel(x+i, y+j, *(image + j*h + i));
                else      tft.drawPixel(x+i, y+h-1-j, *(image + j*h + i));
            } else {
                if (yfor) tft.drawPixel(x+w-1-i, y+j, *(image + j*h + i));
                else      tft.drawPixel(x+w-1-i, y+h-1-j, *(image + j*h + i));
            }
        }
    }
}

/** Draw a rounded rectangle bounded by (x, y, x+w, y+h) */
void roundedRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *corners, uint16_t cw, uint16_t fill) {
    // Radius is fixed to 3 pixels.
    drawPixmap(corners, x, y, cw, cw, true, true);
    tft.fillRect(x+cw, y, w-cw*2, cw, fill);
    drawPixmap(corners, x+w-cw, y, cw, cw, false, true);
    tft.fillRect(x, y+cw, w, h-cw, fill);
    drawPixmap(corners, x, y+h-cw, cw, cw, true, false);
    tft.fillRect(x+cw, y+h-cw, w-cw*2, cw, fill);
    drawPixmap(corners, x+w-cw, y+h-cw, cw, cw, false, false);
}

/** Draw a character */
uint16_t drawChar(char c, uint16_t x, uint16_t y, boolean inverse) {
    int hindex = (c - 32) * 5;
    int rows = FONTHEAD[hindex];
    int width = FONTHEAD[hindex+1];
    int left = FONTHEAD[hindex+2];
    int top = FONTHEAD[hindex+3];
    int advance = FONTHEAD[hindex+4];

    int datindex = pgm_read_word_near(FONTIDX + c - 32);
    int mode = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < width; j++) {
            int dat = pgm_read_word_near(FONTDATA + datindex);
            int color;
            if (mode == 0) {
                color = dat >> 10;
            } else if (mode == 1) {
                color = (dat >> 5) & 31;
            } else {
                color = dat & 31;
            }
            if (inverse) color = 31 - color;
            uint16_t rgb = (color << 11) + ((color * 2) << 5) + color;
            tft.drawPixel(x+j+left, y+i-top, rgb);
            mode++;
            if (mode == 3) {
                mode = 0;
                datindex++;
            }
        }
    }
    return advance;
}

void drawString(String str, uint16_t x, uint16_t y, boolean inverse) {
    for (int i = 0; i < MAXTITLE && str[i] != '\0'; i++) {
        x += drawChar(str[i], x, y, inverse);
    }
}

int textWidth(String str) {
    int w = 0;
    for (int i = 0; i < MAXTITLE && str[i] != '\0'; i++) {
        int hindex = (str[i] - 32) * 5;
        int advance = FONTHEAD[hindex+4];
        w += advance;
    }
    return w;
}

/** Converts hex char --> integer */
int ctoi(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    return -1;
}

int hexToButton(char c) {
    int title = ctoi(command[1]);
    if (title < 0) {
        Serial.println("eInvalid hex character " + command[1]);
        return -1;
    } else if (title > 11) {
        Serial.println("eOnly 12 buttons (0 - 11) supported");
        return -2;
    }
    return title;
}
