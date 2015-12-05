# arduino-mearm

Responsive and smooth servo steering of the Phenoptix [MeArm][1] robot using the [Arduino][2] microcontroller board.

[![MeArm servo steering](http://img.youtube.com/vi/0XRP3wu5jbU/0.jpg)](http://www.youtube.com/watch?v=0XRP3wu5jbU)

Note, you might have to [uninstall *brltty* if it conflicts with the Arduino ttyUSB0][3].

## build

Install the Arduino Makefile, the *screen* serial terminal, and the Google test library as follows:

```
sudo aptitude install arduino-mk screen google-mock
```

Then build the Arduino program using *make*:

```
make
```

## test

You can also build and run the tests on the *PC* using the check target:

```
make check
```

## install on Arduino

The upload target will upload the program via */dev/ttyUSB0* to the *Arduino* board.

```
make upload
```

**Warning: disconnect the servos from the power source while uploading the program to prevent erratic motion!**

## control robot

```
screen /dev/ttyUSB0 9600
```

Examples of servo commands are:

* **t**: get time
* **b**: get base servo angle
* **s**: get shoulder servo angle
* **e**: get elbow servo angle
* **g**: get gripper servo angle
* **B**: get base servo pulse width
* **S**: get shoulder servo pulse width
* **E**: get elbow servo pulse width
* **G**: get gripper servo pulse width
* **45b**: set base servo angle to 45 degrees
* **-12.5**: set shoulder servo angle to -12.5 degrees
* **10e**: set elbow servo angle to 10 degrees
* **30g**: set gripper servo angle to 30 degrees
* **2400**: set base servo pulse width to 2400
* **1500S**: set shoulder servo pulse width to 1500
* **1500E**: set elbow servo pulse width to 1500
* **544G**: set gripper servo pulse width to 544
* **ma**: save teach point *a*
* **@a**: go to teach point *a*
* **x**: stop all servos

You can exit the *screen* terminal using Ctrl-A \\.

**Warning: self-collisions of the robot can damage the servos!**

**Warning: closing the gripper too far on an object can damage the servo!**

## file structure

* *.*: control software with tests
* *arduino*: software to upload onto Arduino board
* *etc*: prototype software, math, and simulations

# External Links

* [MeArm][1]
* [Arduino][2]
* [Instructions for MeArm with Arduino][13]
* [Google Test][4]
* [Turnigy TG9e servo][5]
* [Control the position of a RC servo motor with your Arduino][6]
* [Multi-tasking the Arduino][7]
* [MeArm kinematics][8]
* [MeArm control using potentiometers][10]
* [MeArm inverse kinematics project][9]
* [Robot arm Arduino project for beginners][11]
* [MeArm 0.4 assembly][12]
* [Arduino Servo issues][13]

[1]: http://mearm.com/
[2]: https://www.arduino.cc/
[3]: http://www.ladyada.net/learn/arduino/lesson0-lin.html
[4]: https://code.google.com/p/googlemock/wiki/ForDummies
[5]: http://www.servodatabase.com/servo/turnigy/tg9e
[6]: https://www.arduino.cc/en/Tutorial/Knob
[7]: https://learn.adafruit.com/multi-tasking-the-arduino-part-1?view=all
[8]: http://bitofahack.com/post/1433701488
[9]: https://github.com/yorkhackspace/meArm
[10]: https://github.com/phenoptix/MeArm
[11]: http://lifehacker.com/build-a-kickass-robot-arm-the-perfect-arduino-project-1700643747
[12]: http://www.mathias-wilhelm.de/arduino/projects/phenoptix-mearm/
[13]: http://mearm.com/pages/instructions
[14]: http://www.double-oops.org/mini-blog/debuggingarduinoservoissues
