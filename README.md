# Touchapad

Touchapad is an touchscreen-based button pad designed to be placed outdoors and control [Home Assistant](https://home-assistant.io) devices.

This repository hosts the code used for:
 - The Arduino controlling the touchscreen
 - STL files for 3D-printed parts
 - KiCad files for the Arduino shield. You can download KiCad to export Gerber files configured to however your PCB manufacturer requires.
 - Python server for linking the Arduino touchscreen to Home Assistant and serving the configuration frontend
 - Configuration frontend source code (uses Svelte so you've got to compile it)

<img src="https://rianadon.github.io/blog/assets/2020-05-20-box1.jpg" alt="The Touchapad" width="400" height="300" />

Design and architecture information can be found at the project's [blog post](https://rianadon.github.io/blog/2020/05/23/touchapad.html).
