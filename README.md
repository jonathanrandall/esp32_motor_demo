# ROS/Arduino Serial Motor Demo

This is demonstration of a ROS 2 interface to an esp32 running differential-drive motor control code.

The codes are as follows.

- `esp32_2_pi_demo` - Demonstration for wifi communication from esp32 to pi
- `esp32_motor_demo` - ros2 code for the motor driver and gui nodes
- `esp32_motor_demo_msgs` - support variables for the *esp32_motor_demo* code
- `ros_esp32_bridge_wifi` - sketch for the esp32, to run the wireless communications. Done in *platformio*
- `ros_esp32_bridge_serial` - sketch for the esp32 to run the serial communication with the ros2. Done in *arduino IDE*. Only works with original serial ros2 demo.



## Components

The `esp32_motor_demo` package consists of two nodes, `driver.py` and `gui.py`. The idea is that the driver can be run on an onboard PC inside a robot (e.g. a Raspberry Pi), interfacing with the lower-level hardware. The driver exposes motor control through ROS topics (see below), which are to be published by the user's software.

The GUI provides a simple interface for development and testing of such a system. It publishes and subscribes to the appropriate topics.


## Driver configuration & usage

The driver has a few parameters:

- `encoder_cpr` - Encoder counts per revolution
- `loop_rate` - Execution rate of the *esp32* code (see Arduino side documentation for details)
- `esp32_ip` - This is the ip of the esp32. I'm using a static ip (default `http://192.168.1.211`)
- `baud_rate` - Obsolete since I'm using a wifi connection
- `serial_port` - Obsolete
- `serial_debug` - Enables debugging of serial commands (default `false`)

To run, e.g.
```
ros2 run esp32_motor_demo driver --ros-args -p encoder_cpr:=3440 -p loop_rate:=30 
```

It makes use of the following topics
- `motor_command` - Subscribes a `MotorCommand`, in rads/sec for each of the two motors
- `motor_vels` - Publishes a `MotorVels`, motor velocities in rads/sec
- `encoder_vals` - Publishes an `EncoderVals`, raw encoder counts for each motor



## GUI Usage

Has two modes, one for raw PWM input (-255 to 255) and one for closed-loop control. In this mode you must first set the limits for the sliders.






