# ESP RC Wifi Car web control

NodeMcu ESP8266/ESP32 web control joystick. 

Uses Joystick to control the movement of a robot/rc car. 
Uses async HTML protocol. 

## Compatibility
Compatible with ESP8266 and ESP32


## Configuration

You may want to modify your password and SSID to a personal one, and set motor Driver pins.

src/config.h contains the setup.

## Build 

### 1 Sketch 
Recommended to compile using VS Code + PlatformIO

### 2 Server Files 
In PIO Tab, use PIO > "Build Filesystem Image" to build ./data  

## Upload 

### 1 Sketch 
Use normmal build & upload from PIO

### 2 Server Files 
In PIO Tab, use PIO > "Upload Filesystem Image" to upload ./data content to SPIFFS 

## Connect
Use yout phone, connect to wifi "Joystick-x" (x being the chip ID of the ESP), open web navigator of your preference and go to 192.168.4.1 you will see the joystick web interface

## More info

You can find more detailed info about how it works and how to modify it in the following [blog post](), and see it in action in the following [youtube video]().


## Used Libraries and acknowledgements

Uses the following libraries and couldn't be done withouth the following posts
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [TB6612_ESP32](https://github.com/pablopeza/TB6612FNG_ESP32)
- [Joytick JS library from bobbotek](https://github.com/bobboteck/JoyStick)
- [Differential control](https://www.impulseadventure.com/elec/robot-differential-steering.html)
- [Async service](https://github.com/neonious/lowjs_esp32_examples/blob/master/neonious_one/cellphone_controlled_rc_car/www/index.html)
- [web baker](https://gchq.github.io/CyberChef/#recipe=Gzip('Dynamic%20Huffman%20Coding','index.html.gz','',false)To_Hex('0x',0)Split('0x',',0x'))
- [SMARS](https://www.thingiverse.com/thing:2662828)
- [SMARS](https://github.com/nkmakes/SMARS-esp32)

Huge thanks to the authors, feel free to donate them!
