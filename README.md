# esp32-ps4-robot

## Description:
**esp32-ps4-robot** is an open-source project that enables you to control a dual-motor rc using a PS4 controller via 
Bluetooth. 

**Note:** This project uses a modified version of aed3's PS4-esp32 library to establish a connection between an ESP32 
and a PS4 controller. You can find the original source code here:
[https://github.com/aed3/PS4-esp32](https://github.com/aed3/PS4-esp32)

Basic steering controls are implemented using the left and right analog sticks, but the project can be expanded to 
accommodate other features, such as lighting control, pan/tilt control for a camera or a mechanical arm, control of a 
turret, etc. The PS4 controller's buttons and joysticks can be mapped to various commands, only limited by the hardware 
capabilities of your robot.

## Features:
**Bluetooth Connectivity:** The project uses Bluetooth to establish a wireless connection between the ESP32 and PS4 
controller, eliminating the need for physical wires and allowing for freedom of movement.

**PS4 Controller Integration:** The PS4 controller is a popular and widely available gaming controller that can be 
paired with an ESP32 by overriding the controller's bluetooth MAC address. You can use existing tools like the Sixaxis
Pairing tool for this.

**Customizable Mapping:** The project allows you to customize the mapping of PS4 controller buttons and joysticks to 
commands, giving you flexibility and control.

**Open-Source and Extensible:** The esp32-ps4-robot project is open-source, allowing you to modify and extend the code 
to suit your specific requirements. You can also contribute to the project by adding new features, improving the code, 
or by fixing issues.
