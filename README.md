# esp32-ps4-robot

## Description:
**esp32-ps4-robot** is an open-source project that uses Bluetooth, an ESP32, and a PS4 controller to command a dual-motor robot.

**Note:** This project uses a modified version of aed3's PS4-esp32 library to establish a bluetooth connection between an ESP32 
and a PS4 controller and to read controller input. You can find the original, unmodified source code here:
[https://github.com/aed3/PS4-esp32](https://github.com/aed3/PS4-esp32)

Basic steering controls have been implemented using the left and right analog sticks, but the project can be expanded to 
accommodate other features, such as lighting control, pan/tilt control, turret control, etc. In reality, the PS4 controller's buttons and joysticks can be mapped to various commands, which should only be limited by your hardware.

## Features:
**Bluetooth Connectivity:** Bluetooth connection between the ESP32 and PS4 
controller eliminates the need for physical wires and allows for freedom of movement.

**PS4 Controller Integration:** The PS4 controller is widely available and can be 
paired with an ESP32 by overriding the controller's bluetooth MAC address. You can use existing tools like the Sixaxis
Pairing tool for this.

**Customizable Mapping:** You can customize the mapping of the PS4 controller's buttons and joysticks to 
commands, giving you flexibility and control.

**Open-Source and Extensible:** The esp32-ps4-robot project is open-source, which allows you to modify and extend the code 
to suit your specific requirements. You can contribute to the project by adding new features, improving the code, 
or by fixing issues.
