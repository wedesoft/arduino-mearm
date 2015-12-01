ARDUINO_DIR = /usr/share/arduino
BOARD_TAG    = diecimila
ARDUINO_PORT = /dev/ttyUSB0
ARDUINO_LIBS =
SCREEN = screen
include /usr/share/arduino/Arduino.mk

build-$(BOARD_TAG)/Servo.o: ../curve.hh ../controller.hh ../calibration.hh

repl:
	$(SCREEN) $(ARDUINO_PORT) 9600
