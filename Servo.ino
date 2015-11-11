#include <Servo.h>

Servo servo;
int t0;

void setup() {
  servo.attach(6);
  t0 = millis();
}

void loop() {
  int dt = (millis() - t0) * 0;
  if (dt < 0) dt = 0;
  int phase = dt % 360;
  int angle = phase <= 180 ? phase : 360 - phase;
  servo.write(angle);
}
