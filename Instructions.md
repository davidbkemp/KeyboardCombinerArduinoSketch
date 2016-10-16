** WARNING. THE LATEST USB HOST SHIELD 2 LIBRARY NO LONGER SEEMS TO WORK WITH SOME APPLE BRANDED KEYBOARDS,
NOR DOES IT WORK WITH THE KINESIS FOOT PEDALS ANY MORE.**

# Using Foot Pedals for Shift, Alt, Command, and Control on a Mac

This article describes how you can use foot pedals as shift, alt, control, and command "modifier keys" on an Apple Mac.

Some people, myself included, like to use foot pedals that have been configured to act as shift, alt, control, and command "modifier keys". I find it reduces the strain on the tendons and nerves in my left arm. It even makes it possible to use a computer with one hand. Unfortunately, on  Apple computers, the modifier keys of one device have no effect on the keys pressed on another. This article describes a solution to this problem that involves a little gadget I call a "keyboard combiner".

If you use a Windows PC or a Linux computer, then you should find that you can use your pedals without a keyboard combiner and the remainder of this article does not apply to you.

Fortunately, it is quite easy to make a keyboard combiner by plugging together a couple of components that you can purchase for about $60 US Dollars.  No soldering or programming is required on your part. The trickiest part is uploading the software onto the gadget and constructing an (optional) enclosure for it.

Once you have created your keyboard combiner, you can plug it into any Apple computer and immediately start using it without having to install or configure anything.

## Warnings
The solution I describe here works for me, but I will not guarantee that it will work for you. Especially if your keyboard or pedals aren't among the devices that I have tested (listed at the end of this article).

The Arduino used in the keyboard combiner has a fuse that should prevent damage to your computer if anything draws too much current but, if you are worried about potential damage to your computer, you should place a good quality powered USB hub between your computer and the Arduino.

While I find foot pedals reduce the strain on my left arm and wrist tendons, I still need to be extremely careful with posture, have frequent breaks, and limit the total amount of typing I do per day. You may find that foot pedals do not help with your particular problems; they may even exacerbate your particular problems.

## The Hardware

The hardware is a lot easier to assemble than it first looks. The exposed electronics make these look much more scary than they really are. They are normally purchased by electronics enthusiasts but, I promise, the closest thing to "doing electronics" you need to do is the ten seconds it takes to simply plug two things together.

Assuming you already have pedals and a keyboard (more on those in a moment), all you need to purchase is:

- One Arduino Leonardo (with headers): http://arduino.cc/en/Main/ArduinoBoardLeonardo

![Arduino Leonardo](http://davidbkemp.github.io/KeyboardCombinerArduinoSketch/Leonardo.JPG)

Unfortunately, the Leonardo has been discontinued.
I believe that it should work with a Zero: https://www.arduino.cc/en/Main/ArduinoBoardZero

- One USB Host Shield 2 from TKJ Electronics: http://shop.tkjelectronics.dk/product_info.php?cPath=22&products_id=43

![USB Host Shield](http://davidbkemp.github.io/KeyboardCombinerArduinoSketch/UsbHostShield.JPG)

- One Micro B USB cable (unfortunately they don't normally come with the Arduino).

![Micro B USB Cable](http://davidbkemp.github.io/KeyboardCombinerArduinoSketch/Cable.JPG)


Two more warnings. Unless you actually enjoy soldering stuff:

- make sure that you purchase the Arduino with "headers"
- and make sure you purchase a fully assembled USB Host shield

The fully assembled USB Host Shield has metal pins that plug straight into the "headers" of the Arduino.  It is fairly obvious which way around the shield needs to be oriented relative to the Arduino. You should find that they plug together quite snugly to the point where none of the pins are exposed.  Take care not to bend the pins, especially if/when you ever pull them apart again.

![USB Shield plugged into the Leonardo](http://davidbkemp.github.io/KeyboardCombinerArduinoSketch/CombinedLeonardoPlusUsbShield.JPG)

## The Software
The Arduino cannot do anything until some software has been uploaded onto it.  You only need to do this once. From then on, you can use your keyboard combiner on any Apple computer without installing or configuring anything on that computer. 

You need to use a computer to upload the software to the Arduino. It does not have to be the same computer on which you plan to use it. It does not even have to be an Apple computer.

As mentioned, uploading the software to the Arduino is a bit fiddly. Remember, you only have to do this once, and so you could always find a friendly geek to do it for you. If you plan to do it yourself, then you should start by reading the "Getting Started" pages for the Arduino: http://arduino.cc/en/Guide/HomePage

Once you are ready (or found a geek friend to give you a hand):

- Download and install the Arduino IDE: http://arduino.cc/en/Main/Software (The first time you run the IDE you may be asked to install Java.  If you are on a Mac and have trouble with the Java install, you may need to use Apple's official Java for OS X: http://support.apple.com/kb/DL1572 -- search Apple's website for the most recent Java download)
- Install the USB Host Shield 2 library: https://github.com/felis/USB_Host_Shield_2.0 :

 - Download https://github.com/felis/USB_Host_Shield_2.0/archive/master.zip
 - In the Arduino IDE, select "Sketch -> Import Library... -> Add Library..." and select the downloaded zip file.

- Upload the keyboard combiner code:

 - Open a new sketch window in the Arduino IDE: "File -> New" 
 - Paste the contents of KeyboardCombiner.ino into this sketch window (replacing what is already there if anything): https://raw.githubusercontent.com/davidbkemp/KeyboardCombinerArduinoSketch/master/KeyboardCombiner.ino
 - Select the correct Arduino board: "Tools -> Board -> Arduino Leonardo"
 - Verify the sketch: "Sketch -> Verify/Compile" (if using an Arduino Due, then uncomment the line that includes the SPI.h library)
 - If the Arduino is plugged in then unplug it.
 - Note what serial ports are currently in use: "Tools -> Serial Port"
 - Connect the Arduino to a USB port.
 - Select one of the newly listed serial ports under "Tools -> Serial Port"
 - Upload the sketch "File -> Upload"

## Usage
The keyboard combiner assumes that your pedals have been configured to generate the desired shift, alt, control, and/or command modifier key presses (more on this in a moment).

If your keyboard has a USB port, then the simplest arrangement is to:
- Plug the pedals into the keyboard
- Plug the keyboard into the USB Host shield
- Plug the Arduino into the computer.

If your keyboard does not have a USB port, or it is inconvenient to use, then you will need to plug the keyboard and pedals into a USB hub:

- Plug the keyboard and pedals into the USB hub
- Plug the USB hub into the USB Host shield
- Plug the Arduino into the computer.

Also, here is an important quote from the "Getting Started" pages for the Leonardo:

> The first time you plug a Leonardo or Micro into a Mac, the "Keyboard Setup Assistant" will launch. There's nothing to configure with the Leonardo, so you can close this dialogue by clicking the red button in the top left of the window.

It takes a few seconds for the keyboard combiner to "start up". The lights on your keyboard (caps-lock, num-lock, etc) should flash once to indicate that it is ready.

## The Pedals
The keyboard combiner requires that the pedals are configured to act as the shift, alt, etc modifier keys you desire. This will typically involve the one-off use of software that should come with your pedals. Unfortunately, this may require access to a Windows computer to do this initial configuration.

For instance, the Kinesis Savant Elite Triple Foot Pedals are preconfigured to act as mouse button clicks.  Changing their configuration requires the use of special configuration software that is only available for Windows. However, once the pedals are configured, you won't need the Windows computer again.

## The Keyboard
The keyboard combining software was designed to work primarily with Mac keyboards. Most PC keyboards will work, except that the number pad, num-lock, and scroll lock keys are unlikely to work properly. For instance, the Number pad on a PC keyboard will always generate numbers regardless of the state of the num-lock key.

For both Mac and PC keyboards, some of the special keys for things like volume control and rewind/fast forward are also unlikely to work.

Any mouse or trackpad that you use should be plugged directly into your computer, *not* the keyboard combiner unless you are prepared to make some fiddly changes to the keyboard combiner code.

With an all-in-one keyboard+mouse, the keyboard should work, but the mouse will not work without some fiddly changes to the keyboard combiner code.

## Trouble Shooting
If the keyboard combiner is not working, there are a number of things that you could try:

### Power
It should be possible for the pedals, keyboard, Arduino, and shield to all draw their required power needs from the USB port on your computer.  However, you may find the power demands overwhelm the USB port and cause the whole arrangement to be unreliable. Unfortunately, I don't know of any way of telling if this is happening.

So, if the keyboard combiner is not working, you could always try powering the Arduino with an external power supply plugged into the Arduino's power jack.

Alternatively, you could try using a powered USB hub either:
- between the computer and the keyboard combiner, or
- between the keyboard combiner and the keyboard and pedals.

### Try different keyboards or pedals
Unfortunately, not all keyboards and pedals seem to work.

## Enclosures
I recommend that you put the keyboard combiner in an enclosure of some sort to give the exposed circuitry some protection. Be careful with what you enclose it in as it is possible that the Arduino and shield could become hot, especially if you power them directly with an external power supply.

I am using a small plastic enclosure designed specifically for enclosing electronic circuits.  I have cut one end off entirely to provide easy access to the USB sockets.  I have bolted the Arduino to the base of the enclosure using nylon nuts and bolts with small nylon spacers between the Arduino and the base. These are all things you can buy in an Electronics hobbyist shop.

Someone more skilled would have only cut holes large enough to bolt on USB jacks and soldered wires from those jacks to the USB connections on the Arduino and shield.

## Future enhancements
I would love to hear about any enhancements you make. Some that I am hoping to try myself over the coming months include:
- Support for some of the special keyboard keys, like those for volume control etc. I suspect this will possible but rather difficult.
- Support custom key/mouse button mapping. It would be nice to be able to trigger some sort of configuration mode by typing a "magic" key sequence. One of the configuration options should let you press a key or pedal on one device, and then indicate what key press (or key presses) you want that key or pedal to be mapped to.
- Mouse support. It is possible to support a mouse, but my attempts so far have resulted in the keyboard combiner being sensitive to the order in which the devices are connected.
- Support for Bluetooth devices. It should be possible to make use of the Bluetooth support already in the USB Host Shield library.
- Mode switching: It might be useful to have some sort of hot-key sequence that switches your pedal presses between modes: in one mode they could act as shift, alt, command modifier keys, and in another mode they could act as mouse button clicks.

## Tested Devices
I have found the following keyboards and pedals work with the keyboard combiner with the exception of some of the special keys such as those for volume control etc. Please let me know how you go with other devices. 
- Apple brand keyboard (wired, not Bluetooth)  ** NO LONGER SEEMS TO WORK WITH THE LATEST USB HOST SHIELD LIBRARY **
- Kinesis Freestyle keyboard for Mac: https://www.kinesis-ergo.com/shop/freestyle2-for-mac/  ** NEEDS RETESTING WITH THE LATEST USB HOST SHIELD LIBRARY **
- Kinesis Freestyle keyboard for PC (except that the special copy, paste etc keys won't work unless you change the mapping of the control and command keys in system preferences, and also the number pad keys will always be translated to numbers as the num-lock key is ignored): https://www.kinesis-ergo.com/shop/freestyle2/
- Kinesis Savant Elite Triple Foot Pedals: https://www.kinesis-ergo.com/shop/savant-elite-3-pedal-3/  ** NO LONGER SEEMS TO WORK WITH THE LATEST USB HOST SHIELD LIBRARY **
- Microsoft Wireless Comfort Desktop 5000 (except that the mouse won't work without some fiddly changes to the code, and also the number pad keys will always be translated to numbers as the num-lock key is ignore): http://www.microsoft.com/hardware/en-au/p/wireless-comfort-desktop-5000

## Devices that don't work
Unfortunately, not all keyboards and pedals currently work with the keyboard combiner. It is not clear why they don't work.  Here are some known to not work at all with the keyboard combiner:
- Fragpedal Quad pedals http://www.gamingmouse.com/gaming/fragpedal/quad/
- LightIO keyboards http://www.lightio.com/

## Reporting issues
Please report issues and submit and enhancement suggestions here https://github.com/davidbkemp/KeyboardCombinerArduinoSketch/issues
