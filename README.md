KeyboardCombinerArduinoSketch
=============================

Arduino sketch: USB Keyboard combiner.
It combines the inputs from two keyboards, primarily to work around
the way Mac computers do not allow the modifier keys of one keyboard to act
on the keys of another keyboard.
In particular, it allows you to use foot pedals as Shift/Alt/Command/Control modifiers
for a keyboard.

Runs on an Arduino that can act as a USB client:
 - Leonardo
 - Due (yes, the shield plugs straight into the Due despite the voltage differences). If using the Due, then you must include the Arduino SPI library like so: ```#include <SPI.h>``` in your .ino file.

Requires the USB Host Shield 2 from Circuits at Home.
https://www.circuitsathome.com/products-page/arduino-shields

It has only been tested using combinations of:
 - the standard Mac Keyboard
 - the Kinesis Freestyle 2 for Mac Keyboard
 - the Kinesis Savant Elite/USB 3 Pedal

Relies on the USB Host Shield 2 library:
https://github.com/felis/USB_Host_Shield_2.0

This sketch was adapted from the USBHIDBootKbd example in the USB Host Shield 2 library.

The code is released under the GNU General Public License.

