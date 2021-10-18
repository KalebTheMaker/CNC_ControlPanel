![Element14 Image](https://github.com/KalebClark/MakerCalc/blob/master/EpisodeImage.png)
# CNC Control Panel
This is a control panel for [MACH4 Motion Control software](https://www.machsupport.com/software/mach4/) that will be used with an [AVID CNC PRO4896](https://www.avidcnc.com/pro4896-4-x-8-cnc-router-kit-p-253.html) CNC Router. (Add More description)

### Element14
The main home for this project is on the Element14 Community. There you can find the BOM (Bill of Materials), updates, the build video and discussions with myself and the community about this build.

You can report bugs here, but all other inqueries should be directed to the link below. I will be glad to answer any questions on the Element 14 Community Page.

[Element14 Connumity Project Page](https://www.element14.com/community/docs/DOC-95185/l/episode-451-build-an-off-grid-wikipedia-with-raspberry-pi)

## Keyboard Emulation
The [Arduino NANO IOT 33](https://docs.arduino.cc/hardware/nano-33-iot) with Cortex-M0 SAMD21 processor can emulate a keyboard just like the [Arduino Micro](https://store-usa.arduino.cc/products/arduino-micro)'s ATmega32U4 processor.

## Code Notes
### Buttons that emulate a keyboard
Library Used: EasyButton
The buttons that emulate a keyboard are setup to mimic holding down a key on the keyboard. Normally here you would get a single keydown and keyup event that would cycle every iteration. I used a keyboard debugger to see exactly what happens on a keyboard when you hold the button down, and this is what I found. There is an initial key down event, followed by repeated key down events (rather than keydown, key up every cycle). This is how I am sending them here. Check for presss, repeat the keydown event non-blocking until released, then send the release signal.

## Installation
This is a pretty normal Arduino install. Just fire up the the Arduino IDE, configure it for your board and compile/upload.

