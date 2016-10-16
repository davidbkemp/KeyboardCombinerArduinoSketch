/*
* Arduino sketch: USB Keyboard combiner.
* It combines the inputs from multiple keyboards, primarily to work around
* the way Mac computers do not allow the modifier keys of one keyboard to act
* on the keys of another keyboard.
* In particular, it allows you to use foot pedals as Shift/Alt/Command/Control modifiers
* for a keyboard.
*
* Runs on an Arduino that can act as a USB client (Leonardo or Due),
* with the USB Host Shield 2 from Circuits at Home.
* https://www.circuitsathome.com/products-page/arduino-shields
*
* Relies on the USB Host Shield 2 library:
* https://github.com/felis/USB_Host_Shield_2.0
*
* This sketch was adapted from the USBHIDBootKbd example in the USB Host Shield 2 library.
*
* For more details, see: https://github.com/davidbkemp/KeyboardCombinerArduinoSketch/wiki
*
* The code is released under the GNU General Public License.
*/

#include "Keyboard.h"
#include "Mouse.h"

#include <hidboot.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

#define UHS_HID_BOOT_KEY_MAC_RETURN 0x28
#define UHS_HID_BOOT_KEY_MAC_ENTER  0x58
#define UHS_HID_BOOT_KEY_ESC        0x29
#define UHS_HID_BOOT_KEY_DEL        0x4C
#define UHS_HID_BOOT_KEY_BACKSPACE  0x2A
#define UHS_HID_BOOT_KEY_HOME       0x4A
#define UHS_HID_BOOT_KEY_PAGE_UP    0x4B
#define UHS_HID_BOOT_KEY_END        0x4D
#define UHS_HID_BOOT_KEY_PAGE_DN    0x4E
#define UHS_HID_BOOT_KEY_CLEAR      0x53
#define UHS_HID_BOOT_KEY_PAD_EQL    0x67
#define UHS_HID_BOOT_KEY_TAB        0x2B
#define UHS_HID_BOOT_KEY_CAPS       0x39


const boolean debug = false;
boolean keyboardInitialized = false;
boolean mouseInitialized = false;
void pressKey(uint8_t key);
void releaseKey(uint8_t key);
void moveMouse(signed char x, signed char y);
void mousePress(char button);
void mouseRelease(char button);

// Keep track of press/release events for every key.
uint8_t keyPressCount[256];

const uint8_t modifier_keys[] = {KEY_LEFT_CTRL, KEY_LEFT_SHIFT, KEY_LEFT_ALT, KEY_LEFT_GUI,
        KEY_RIGHT_CTRL, KEY_RIGHT_SHIFT, KEY_RIGHT_ALT, KEY_RIGHT_GUI};


class MouseRptParser : public MouseReportParser
{
protected:
  void OnMouseMove  (MOUSEINFO *mi);
  void OnLeftButtonUp (MOUSEINFO *mi);
  void OnLeftButtonDown (MOUSEINFO *mi);
  void OnRightButtonUp  (MOUSEINFO *mi);
  void OnRightButtonDown  (MOUSEINFO *mi);
  void OnMiddleButtonUp (MOUSEINFO *mi);
  void OnMiddleButtonDown (MOUSEINFO *mi);
};

void MouseRptParser::OnMouseMove(MOUSEINFO *mi)
{
    moveMouse(mi->dX, mi->dY);
};
void MouseRptParser::OnLeftButtonUp (MOUSEINFO *mi)
{
    mouseRelease(MOUSE_LEFT);
};
void MouseRptParser::OnLeftButtonDown (MOUSEINFO *mi)
{
    mousePress(MOUSE_LEFT);
};
void MouseRptParser::OnRightButtonUp  (MOUSEINFO *mi)
{
    mouseRelease(MOUSE_RIGHT);
};
void MouseRptParser::OnRightButtonDown  (MOUSEINFO *mi)
{
    mousePress(MOUSE_RIGHT);
};
void MouseRptParser::OnMiddleButtonUp (MOUSEINFO *mi)
{
    mouseRelease(MOUSE_MIDDLE);
};
void MouseRptParser::OnMiddleButtonDown (MOUSEINFO *mi)
{
    mousePress(MOUSE_MIDDLE);
};


class KbdRptParser : public KeyboardReportParser
{
        uint8_t mapToAscii(uint8_t);
        void PrintKey(uint8_t mod, uint8_t key);

protected:
        void OnControlKeysChanged(uint8_t before, uint8_t after);
        void OnKeyDown(uint8_t mod, uint8_t key);
        void OnKeyUp(uint8_t mod, uint8_t key);
};

// Adapted from KeyboardReportParser::OemToAscii
// Maps the key reported from the USB-Host to a byte that can be passed to Keyboard.press()
// As well as mapping the printable characters, it also maps most of the non-printable keys.
uint8_t KbdRptParser::mapToAscii(uint8_t key) {
        // [a-z]
        if (VALUE_WITHIN(key, 0x04, 0x1d)) {
                return (key - 4 + 'a');
        }// Numbers
        else if (VALUE_WITHIN(key, 0x1e, 0x27)) {
                 return ((key == UHS_HID_BOOT_KEY_ZERO) ? '0' : key - 0x1e + '1');
        }// Keypad Numbers
        else if(VALUE_WITHIN(key, 0x59, 0x61)) {
                return (key - 0x59 + '1');
        } else if(VALUE_WITHIN(key, 0x2d, 0x38)) {
                return (uint8_t)pgm_read_byte(&getSymKeysLo()[key - 0x2d]);
        } else if(VALUE_WITHIN(key, 0x54, 0x57)) {
                return (uint8_t)pgm_read_byte(&getPadKeys()[key - 0x54]);
        } else if(VALUE_WITHIN(key, 0x3A, 0x45)) {
                // Function keys F1- F12
                return key - 0x3A + 0xC2;
        } else if(VALUE_WITHIN(key, 0x4F, 0x52)) {
                // Arrow keys
                return key - 0x4F + 0xD7;
        } else {
                switch(key) {
                        case UHS_HID_BOOT_KEY_SPACE: return 0x20;
                        case UHS_HID_BOOT_KEY_MAC_ENTER: return 0x0A;
                        case UHS_HID_BOOT_KEY_MAC_RETURN: return KEY_RETURN;
                        case UHS_HID_BOOT_KEY_ZERO2: return '0';
                        case UHS_HID_BOOT_KEY_PERIOD: return '.';
                        case UHS_HID_BOOT_KEY_ESC: return KEY_ESC;
                        case UHS_HID_BOOT_KEY_DEL: return KEY_DELETE;
                        case UHS_HID_BOOT_KEY_BACKSPACE: return KEY_BACKSPACE;
                        case UHS_HID_BOOT_KEY_HOME: return KEY_HOME;
                        case UHS_HID_BOOT_KEY_PAGE_UP: return KEY_PAGE_UP;
                        case UHS_HID_BOOT_KEY_END: return KEY_END;
                        case UHS_HID_BOOT_KEY_PAGE_DN: return KEY_PAGE_DOWN;
                        case UHS_HID_BOOT_KEY_PAD_EQL: return '=';
                        case UHS_HID_BOOT_KEY_TAB: return KEY_TAB;
                        case UHS_HID_BOOT_KEY_CAPS: return KEY_CAPS_LOCK;
                }
        }
        return ( 0);
}

void KbdRptParser::PrintKey(uint8_t m, uint8_t key)
{
  if (debug) {
    MODIFIERKEYS mod;
    *((uint8_t*)&mod) = m;
    Serial.print((mod.bmLeftCtrl   == 1) ? "C" : " ");
    Serial.print((mod.bmLeftShift  == 1) ? "S" : " ");
    Serial.print((mod.bmLeftAlt    == 1) ? "A" : " ");
    Serial.print((mod.bmLeftGUI    == 1) ? "G" : " ");

    Serial.print(" >");
    PrintHex<uint8_t>(key, 0x80);
    Serial.print("< ");

    Serial.print((mod.bmRightCtrl   == 1) ? "C" : " ");
    Serial.print((mod.bmRightShift  == 1) ? "S" : " ");
    Serial.print((mod.bmRightAlt    == 1) ? "A" : " ");
    Serial.println((mod.bmRightGUI    == 1) ? "G" : " ");
  }
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
    if (debug) {
      Serial.print("DN ");
      PrintKey(mod, key);
    }
    uint8_t c = mapToAscii(key);

    if (c) {
        pressKey(c);
    }
}

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after) {
    MODIFIERKEYS beforeMod;
    *((uint8_t*)&beforeMod) = before;

    MODIFIERKEYS afterMod;
    *((uint8_t*)&afterMod) = after;

    uint8_t allBefore[] = {beforeMod.bmLeftCtrl, beforeMod.bmLeftShift, beforeMod.bmLeftAlt, beforeMod.bmLeftGUI,
          beforeMod.bmRightCtrl, beforeMod.bmRightShift, beforeMod.bmRightAlt, beforeMod.bmRightGUI};
    uint8_t allAfter[] = {afterMod.bmLeftCtrl, afterMod.bmLeftShift, afterMod.bmLeftAlt, afterMod.bmLeftGUI,
          afterMod.bmRightCtrl, afterMod.bmRightShift, afterMod.bmRightAlt, afterMod.bmRightGUI};

    for(int i = 0; i < 8; i++) {
      if (allBefore[i] != allAfter[i]) {
        if (allAfter[i]) {
          pressKey(modifier_keys[i]);
        } else {
          releaseKey(modifier_keys[i]);
        }
      }
    }
}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
    if (debug) {
      Serial.print("UP ");
      PrintKey(mod, key);
    }
    uint8_t c = mapToAscii(key);

    if (c) {
        releaseKey(c);
    }
}

void ensureMouseInitialized() {
    if (!mouseInitialized) {
        mouseInitialized = true;
        if (debug) {
          Serial.println("Initializing mouse");
        } else {
            Mouse.begin();
        }
    }
}

void moveMouse(int8_t x, int8_t y) {
    ensureMouseInitialized();
    if (debug) {
        Serial.print("dx=");
        Serial.print(x, DEC);
        Serial.print(" dy=");
        Serial.println(y, DEC);
    } else {
        Mouse.move(x, y, 0);
    }
}
void mousePress(char button) {
    ensureMouseInitialized();
    if (debug) {
        Serial.print("mouse button pressed: ");
        Serial.println(button, DEC);
    } else {
        Mouse.press(button);
    }
}

void mouseRelease(char button) {
    ensureMouseInitialized();
    if (debug) {
        Serial.print("mouse button released: ");
        Serial.println(button, DEC);
    } else {
        Mouse.release(button);
    }
}


void pressKey(uint8_t key)
{
    if (!keyboardInitialized) {
        keyboardInitialized = true;
        if (debug) {
          Serial.println("Initializing keyboard");
        } else {
          Keyboard.begin();
        }
    }

    keyPressCount[key]++;
    if (keyPressCount[key] == 1) {
      if (debug) {
        Serial.print("Press: ");
        Serial.println(key, HEX);
      } else {
        Keyboard.press(key);
      }
    }
};

void releaseKey(uint8_t key)
{
    keyPressCount[key]--;
    if (keyPressCount[key] == 0) {
      if (debug) {
        Serial.print("Release: ");
        Serial.println(key, HEX);
      } else {
        Keyboard.release(key);
      }
    }
};

USB        usb;
USBHub     hub(&usb);
USBHub     hub2(&usb);
USBHub     hub3(&usb);
USBHub     hub4(&usb);
//USBHub     hub5(&usb);
//USBHub     hub6(&usb);
//USBHub     hub7(&usb);

// Don't use the mouse or composite variation unless you can carefully connect them
// the order they are are activated.
// HIDBoot<USB_HID_PROTOCOL_KEYBOARD | USB_HID_PROTOCOL_MOUSE> hidComposite1(&usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    hidKeyboard1(&usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    hidKeyboard2(&usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    hidKeyboard3(&usb);
// HIDBoot<USB_HID_PROTOCOL_MOUSE>       hidMouse(&usb);

// KbdRptParser compositeKeyboardReportParser1;
// MouseRptParser compositeMouseReportParser1;
KbdRptParser keyboardReportParser1;
KbdRptParser keyboardReportParser2;
KbdRptParser keyboardReportParser3;
// MouseRptParser mouseParser;

void setup()
{
    for (int i = 0; i < 256; i++) {
      keyPressCount[i] = 0;
    }

    if (debug) {
        Serial.begin( 115200 );
#if !defined(__MIPSEL__)
        while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
        Serial.println("Started");
    }
    if (usb.Init() == -1 && debug) {
        Serial.println("OSC did not start.");
    }

    delay( 200 );

//    hidComposite1.SetReportParser(0, &compositeKeyboardReportParser1);
//    hidComposite1.SetReportParser(1, &compositeMouseReportParser1);
    hidKeyboard1.SetReportParser(0, &keyboardReportParser1);
    hidKeyboard2.SetReportParser(0, &keyboardReportParser2);
    hidKeyboard3.SetReportParser(0, &keyboardReportParser3);
//    hidMouse.SetReportParser(0, &mouseParser);

    if (debug) {
        Serial.println("Set the report parsers");
    }

}

void loop()
{
    usb.Task();
}

