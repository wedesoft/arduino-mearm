#include <Servo.h>

Servo servo;
unsigned long t0;
int pulse;
char dir;

void setup() {
  servo.attach(6);
  t0 = millis();
  pulse = 0;
  dir = 1;
}

void loop() {
  // use writeMicroseconds, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH
  int dt = millis() - t0;
  t0 += dt;
  if (dir == 1) {
    pulse += dt;
    if (pulse > MAX_PULSE_WIDTH) {
      pulse = MAX_PULSE_WIDTH;
      dir = 0;
    }
  } else {
    pulse -= dt;
    if (pulse < MIN_PULSE_WIDTH) {
      pulse = MIN_PULSE_WIDTH;
      dir = 1;
    }
  };
  servo.writeMicroseconds(pulse);
}
