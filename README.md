# ROS/Arduino Serial Motor Demo

This is demonstration of a ROS 2 interface to an ESP32 running differential-drive motor control code with BTS7960 motor drivers.

The codes are as follows:

- `esp32_2_pi_demo` - Demonstration for wifi communication from ESP32 to Pi
- `esp32_motor_demo` - ROS2 code for the motor driver and GUI nodes
- `esp32_motor_demo_msgs` - Support variables for the *esp32_motor_demo* code
- `ros_esp32_bridge_wifi` - Sketch for the ESP32, to run the wireless communications. Done in *PlatformIO*
- `ros_esp32_bridge_serial` - Sketch for the ESP32 to run the serial communication with ROS2. Done in *Arduino IDE*. Only works with original serial ROS2 demo.

## Hardware Configuration

This project uses BTS7960 motor drivers instead of L298N. The BTS7960 offers several advantages:
- Higher current capability (up to 43A)
- More efficient with less heat generation
- Different control scheme using RPWM and LPWM pins

### BTS7960 Pin Configuration:
- Each motor uses:
  - RPWM pin (forward direction)
  - LPWM pin (reverse direction)
  - EN pin (enable control)
- Default pin mapping:
  - Left Motor: LPWM=33, RPWM=32, EN=5
  - Right Motor: LPWM=27, RPWM=26, EN=25

## Components

The `esp32_motor_demo` package consists of two nodes, `driver.py` and `gui.py`. The idea is that the driver can be run on an onboard PC inside a robot (e.g. a Raspberry Pi), interfacing with the lower-level hardware. The driver exposes motor control through ROS topics (see below), which are to be published by the user's software.

The GUI provides a simple interface for development and testing of such a system. It publishes and subscribes to the appropriate topics.

## Driver Configuration & Usage

The driver has a few parameters:

- `encoder_cpr` - Encoder counts per revolution
- `loop_rate` - Execution rate of the *ESP32* code (see Arduino side documentation for details)
- `esp32_ip` - This is the IP of the ESP32. I'm using a static IP (default `http://192.168.1.211`)
- `baud_rate` - Obsolete since I'm using a wifi connection
- `serial_port` - Obsolete
- `serial_debug` - Enables debugging of serial commands (default `false`)

To run, e.g.
```
ros2 run esp32_motor_demo driver --ros-args -p encoder_cpr:=3440 -p loop_rate:=30 
```

It makes use of the following topics:
- `motor_command` - Subscribes a `MotorCommand`, in rads/sec for each of the two motors
- `motor_vels` - Publishes a `MotorVels`, motor velocities in rads/sec
- `encoder_vals` - Publishes an `EncoderVals`, raw encoder counts for each motor

## GUI Usage

Has two modes, one for raw PWM input (-255 to 255) and one for closed-loop control. In this mode you must first set the limits for the sliders.

## Power Considerations

When using BTS7960 motor drivers:
- Use an appropriate power supply rated for your motors
- BTS7960 requires a separate logic power supply (5V) in addition to motor power
- Ensure proper grounding between ESP32 and BTS7960
- The ESP32's 3.3V logic is compatible with BTS7960 input pins