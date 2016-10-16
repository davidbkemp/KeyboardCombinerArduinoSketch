KeyboardCombinerArduinoSketch
=============================

Arduino sketch: USB Keyboard combiner.

** WARNING. THE LATEST USB HOST SHIELD 2 LIBRARY NO LONGER SEEMS TO WORK WITH SOME APPLE BRANDED KEYBOARDS,
NOR DOES IT WORK WITH THE KINESIS FOOT PEDALS ANY MORE.**

Detailed instructions: https://github.com/davidbkemp/KeyboardCombinerArduinoSketch/blob/master/Instructions.md

It combines the inputs from up to three keyboards, primarily to work around
the way Mac computers do not allow the modifier keys of one keyboard to act
on the keys of another keyboard.
In particular, it allows you to use foot pedals as Shift/Alt/Command/Control modifiers
for a keyboard.

Runs on an Arduino that can act as a USB client.
Note that this limits the models of Arduino that may be used.
It will **not** work with a Uno.
I have tested it on the Leonardo and Due, but they have been discontinued.
I believe it should work with the Zero (please let me know if you try it).

Requires the USB Host Shield 2 from TKJ Electronics.
http://shop.tkjelectronics.dk/product_info.php?cPath=22&products_id=43

Relies on the USB Host Shield 2 library:
https://github.com/felis/USB_Host_Shield_2.0

This sketch was adapted from the USBHIDBootKbd example in the USB Host Shield 2 library.

The code is released under the GNU General Public License.


