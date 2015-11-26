# arduino-mearm

Responsive servo steering of the Phenoptix [MeArm][1] robot using the [Arduino][2] microcontroller board.

[![MeArm servo steering](http://img.youtube.com/vi/ZGc0j4WMrsY/0.jpg)](http://www.youtube.com/watch?v=ZGc0j4WMrsY)

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
* **-2.5b**: set base servo angle to -2.5 degrees
* **B**: get base servo position
* **S**: get shoulder servo position
* **E**: get elbow servo position
* **G**: get gripper servo position

You can exit the *screen* terminal using Ctrl-A \\.

**Warning: code currently does not prevent self-collisions of the robot which can damage the servo!**

**Warning: closing the gripper too far on an object might damage the servo!**

## file structure

* *.*: control software with tests
* *arduino*: software to upload onto Arduino board
* *etc*: prototype software, math, and simulations

# External Links

* [MeArm][1]
* [Arduino][2]
* [Google Test][4]
* [Turnigy TG9e servo][5]
* [Control the position of a RC servo motor with your Arduino][6]
* [Multi-tasking the Arduino][7]
* [MeArm kinematics][8]
* [MeArm control using potentiometers][10]
* [MeArm inverse kinematics project][9]
* [Robot arm Arduino project for beginners][11]

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
