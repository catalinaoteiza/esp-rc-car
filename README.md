# ESP RC WiFi Car — Web Control

Web-based joystick control for RC cars and mobile robots using an ESP8266 or ESP32.

The project provides a browser-based control interface served by the microcontroller over Wi-Fi. It uses WebSockets for real-time control commands and supports different motor driver configurations.

## Mecanum / Omnibot Branch

This branch extends the original RC car controller to support four independently controlled wheels, enabling omnidirectional movement with mecanum or omni wheels.

### Features

- ESP8266 / ESP32 support.
- Four independently controlled motors.
- Compatible with PWM/direction and dual-PWM motor control configurations.
- Web-based joystick interface.
- Six basic movement directions: forward, backward, strafe left, strafe right, rotate left, and rotate right.
- Configurable motor direction inversion.
- Wi-Fi access point for direct connection without an external router.
- WebSocket communication for low-latency control.
- Optional servo position commands through the control interface.

### Hardware

- NodeMCU ESP8266 or compatible ESP32 development board.
- Four DC gear motors with mecanum or omni wheels.
- Quad motor driver, such as an L298N or L293D, or a compatible driver supporting the required control mode.
- 12 V power system, for example, a 3S lithium-ion battery pack using 18650 cells.
- Optional servo motors.

**Important:** The motor driver, motors, and battery must be compatible with each other and with the selected microcontroller. Do not power motors directly from the ESP GPIO pins.

## Configuration

Edit `src/config.h` to configure the Wi-Fi credentials and motor driver pins.

The default access point name is generated using the `ROV-` prefix followed by the microcontroller's chip identifier.

```cpp
const char* ssid_base = "ROV-";
const char* password = "1234567890";
```

Change the default password before deploying the robot.

Configure the motor pins according to your wiring and selected motor driver. The project supports two motor control approaches:

- **PWM + DIR:** One PWM pin controls speed and another pin selects direction.
- **Dual PWM:** Two control pins drive the motor in opposite directions.

Make sure the selected control functions match the actual motor driver hardware.

## Build

Recommended development environment: **Visual Studio Code + PlatformIO**.

### 1. Build the firmware

Open the project in VS Code and run the PlatformIO **Build** task.

### 2. Build the web interface filesystem image

Place the web interface files in the project's `data/` directory.

In PlatformIO, run **Build Filesystem Image** to package the files for upload to the microcontroller's filesystem.

## Upload

### 1. Upload the firmware

Run the PlatformIO **Upload** task.

### 2. Upload the web interface

Run **Upload Filesystem Image** to upload the contents of `data/` to the configured filesystem.

The firmware uses LittleFS in the current implementation. Make sure the PlatformIO filesystem configuration matches the firmware.

## Connect and Control

1. Power on the robot.
2. Connect your phone or computer to the Wi-Fi access point created by the microcontroller.
3. Find the network named `ROV-<chip ID>` (or the prefix configured in the firmware).
4. Open a web browser and navigate to:

   `http://192.168.4.1`

5. Enable motor control through the interface and use the joysticks to drive the robot.

The ESP8266/ESP32 acts as the access point, so an external Wi-Fi router is not required.

## WebSocket Protocol

The browser sends comma-separated values to the WebSocket server on port `82`.

### Packet format

```text
Forward,Strafe,Rotate,ServoLeft,ServoRight
```

| Field | Description |
|---|---|
| `Forward` | Forward/backward movement command |
| `Strafe` | Left/right lateral movement command |
| `Rotate` | Rotation command |
| `ServoLeft` | Left servo target position in degrees |
| `ServoRight` | Right servo target position in degrees |

Example:

```text
50,0,0,90,90
```

This packet requests forward movement, no lateral movement, no rotation, and servo positions of 90°.

The first three fields are used for mecanum drive calculations. Servo positions are transmitted for future or optional servo control, depending on the firmware implementation.

**Sign convention:** The joystick interface and firmware should use a consistent convention for forward/backward movement, strafing, and rotation. In particular, negative rotation values represent turning right in the convention adopted by this project.

## Over-the-Air Updates

The firmware initializes ArduinoOTA, allowing firmware updates over Wi-Fi when supported by the development environment and network configuration.

## Libraries and Acknowledgements

This project builds on the work of the following authors and projects:

- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) — asynchronous web server.
- [TB6612_ESP32](https://github.com/pablopeza/TB6612FNG_ESP32) — TB6612FNG motor driver reference.
- [Joystick JavaScript Library by Bobboteck](https://github.com/bobboteck/JoyStick) — browser-based joystick controls.
- [Differential Steering](https://www.impulseadventure.com/elec/robot-differential-steering.html) — steering and motor control reference.
- [Async RC Car Example](https://github.com/neonious/lowjs_esp32_examples/blob/master/neonious_one/cellphone_controlled_rc_car/www/index.html) — WebSocket transmission example.
- [CyberChef](https://gchq.github.io/CyberChef/) — useful for web asset encoding and compression workflows.
- [SMARS](https://www.thingiverse.com/thing:2662828) — original SMARS project.
- [SMARS ESP32](https://github.com/nkmakes/SMARS-esp32) — ESP32 implementation reference.

Huge thanks to the authors and contributors whose work helped make this project possible.

## More Information

- **Blog post:** Add the project article URL here.
- **Demo video:** Add the YouTube video URL here.

## License

GPL-3.0 license. 
Check the licenses of the original libraries and any reused source code before redistributing the project.
