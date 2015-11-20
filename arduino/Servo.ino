#include <Servo.h>
#include "../curve.hh"

/*
Servo servo;
unsigned long t0;
int pulse;
int wait;
char dir;
*/
Curve middle(DEFAULT_PULSE_WIDTH);

void setup() {
  /*
  servo.attach(6);
  t0 = millis();
  pulse = DEFAULT_PULSE_WIDTH;
  dir = 0;
  wait = 5000;
  */
}

void loop() {
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
