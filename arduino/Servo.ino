#include <Servo.h>
#include "../curve.hh"

// /usr/share/arduino/hardware/arduino/cores/arduino/CDC.cpp (serial stuff)

unsigned long t0;
/*
Servo servo;
int pulse;
int wait;
char dir;
*/
Curve middle(DEFAULT_PULSE_WIDTH, 250);

void setup() {
  Serial.begin(9600);
  t0 = millis();
  /*
  servo.attach(6);
  pulse = DEFAULT_PULSE_WIDTH;
  dir = 0;
  wait = 5000;
  */
}

void loop() {
  int dt = millis() - t0;
  if (Serial.available()) {
    char c = Serial.read();
    if (c >= '0' && c <= '9')
      middle.retarget((c - '0') * 100);
    else {
      Serial.print(middle.pos());
      Serial.write("\r\n");
    };
  };
  middle.update(dt * 0.001);
  t0 += dt;
  // (millis)
  // (target #:middle 200 #:left 100 #:right 200 #:acceleration 3)
  // (pos); '((#:millis ...) (#:middle . 200) ...)
  // (target)

  // use writeMicroseconds, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH
  /*
  int dt = millis() - t0;
  t0 += dt;
  if (dir == 0) {
    wait -= dt;
    if (wait < 0) {
      wait = 5000;
      dir = 1;
    };
  } else if (dir == 1) {
    pulse += dt;
    if (pulse > MAX_PULSE_WIDTH) {
      pulse = MAX_PULSE_WIDTH;
      dir = 2;
    }
  } else if (dir == 2) {
    pulse -= dt;
    if (pulse < MIN_PULSE_WIDTH) {
      pulse = MIN_PULSE_WIDTH;
      dir = 3;
    }
  } else {
    pulse += dt;
    if (pulse > DEFAULT_PULSE_WIDTH) {
      pulse = DEFAULT_PULSE_WIDTH;
      dir = 0;
    }
  };
  servo.writeMicroseconds(pulse);
  */
}
