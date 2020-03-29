# Arduino Code

Code here is designed to run on an Arduino Leonardo (simply chosen becaue due to limited space, buying a slim USB cable for a micro-USB port was much easier than buying one for the bulky USB port of an Uno.

The excess of files is due to me having to modify the MCU and GFX libraries slim down the sketch size. They are missing a few lines of code compared to their original form. However, since they are in this repository, you don't have to install any external libraries!

## What are these Python files?

The files `corners.py` and `font.py` generate `corners.h` and `font.h` respectively. Both of these files exist because it is much easier to create anti-aliased text in Python and export it as bitmaps for the Arduino code to consume rather than try to perform the same computations on the Arduino. `corners.py` generates bitmaps for creating rectangles with rounded corners, and `font.py` generates bitmaps for a random font I found on my computer.

## Uploading

You can either upload the sketch using the Arduino IDE, or use

```bash
arduino-cli compile -u -b arduino:avr:leonardo -p /dev/ttyACM0 .
```
