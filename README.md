# arduino-mearm

Responsive servo steering of the [MeArm][1] robot using the [Arduino][2] microcontroller board.

[![MeArm servo steering](http://img.youtube.com/vi/pa5znudwhvE/0.jpg)](http://www.youtube.com/watch?v=pa5znudwhvE)

Note, you might have to [uninstall *brltty* if it conflicts with the Arduino ttyUSB0][3].

## build

```
sudo aptitude install arduino-mk screen
make
```

## test

```
make check
```

## install on Arduino

```
make upload
```

## control robot

```
screen /dev/ttyUSB0 9600
```

Examples of servo commands are:

* **t**: get time
* **m**: get middle servo position
* **1500m**: set middle servo to center position

You can exit the *screen* terminal using Ctrl-A \\.

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

[1]: http://mearm.com/
[2]: https://www.arduino.cc/
[3]: http://www.ladyada.net/learn/arduino/lesson0-lin.html
[4]: https://code.google.com/p/googlemock/wiki/ForDummies
[5]: http://www.servodatabase.com/servo/turnigy/tg9e
[6]: https://www.arduino.cc/en/Tutorial/Knob
[7]: https://learn.adafruit.com/multi-tasking-the-arduino-part-1?view=all
