# arduino-mearm

Responsive servo steering of the [MeArm][1] robot using the [Arduino][2] microcontroller board.

Note, you might have to [uninstall *brltty* if it conflicts with the Arduino ttyUSB0][3].

## test and build

```
sudo aptitude install arduino-mk screen
make check
make
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

[1]: http://mearm.com/
[2]: https://www.arduino.cc/
[3]: http://www.ladyada.net/learn/arduino/lesson0-lin.html
