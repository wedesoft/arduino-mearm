# Arduino Hello World

Using [android-mk][1] to let the LED blink.

```Shell
usermod $USER -a -G dialout
sudo aptitude install gcc-avr avr-libc binutils-avr avrdude arduino-mk
make
make upload
```

[1]: http://www.jamesrobertson.eu/blog/2012/sep/20/uploading-a-sketch-from-the-comman.html
[2]: http://www.mightwerk.com/software/EB62E441-1C1B-43AC-9CEA-FF0AE8753833/index.html
[3]: https://learn.adafruit.com/adafruit-proto-shield-arduino/overview
