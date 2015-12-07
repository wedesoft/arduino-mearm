ARDUINO_DIR = /usr/share/arduino
BOARD_TAG    = diecimila
ARDUINO_PORT = /dev/ttyUSB0
ARDUINO_LIBS = Servo
SCREEN = screen
include $(ARDUINO_DIR)/Arduino.mk

$(OBJDIR)/mearm.o: ../curve.hh ../controller.hh ../calibration.hh

repl:
	$(SCREEN) $(ARDUINO_PORT) 9600
