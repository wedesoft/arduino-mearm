# Arduino Hello World

Using [android-mk][1] to let the LED blink.

```Shell
sudo aptitude install arduino-mk
usermod $USER -a -G dialout
make
make upload
```

Use *screen* to connect to the Arduino board and control the LED.

```Shell
screen /dev/ttyUSB0 9600
```

Also see: http://www.wedesoft.de/arduino-hello-world.html

[1]: http://www.jamesrobertson.eu/blog/2012/sep/20/uploading-a-sketch-from-the-comman.html
[2]: http://www.mightwerk.com/software/EB62E441-1C1B-43AC-9CEA-FF0AE8753833/index.html
[3]: https://learn.adafruit.com/adafruit-proto-shield-arduino/overview
