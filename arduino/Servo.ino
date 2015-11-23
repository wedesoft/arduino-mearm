#include <Servo.h>
#include "../controller.hh"
#include "../curve.hh"

unsigned long t0;

class Controller: public ControllerBase
{
public:
  Controller(void): m_middleCurve(DEFAULT_PULSE_WIDTH, 0.0015) {}
  void setup(void) {
    m_middleServo.attach(6);
  }
  void update(int dt) {
    m_middleCurve.update(dt);
    m_middleServo.writeMicroseconds(m_middleCurve.pos());
  };
  void reportTime(void) {
    Serial.print(millis());
    Serial.write("\r\n");
  }
  void reportPosition(Drive drive) {
    switch (drive) {
    case Middle:
      Serial.print(m_middleCurve.pos());
      Serial.write("\r\n");
      break;
    default:
      break;
    };
  }
  void retargetMiddle(int value) {
    int target;
    if (value < MIN_PULSE_WIDTH)
      target = MIN_PULSE_WIDTH;
    else if (value > MAX_PULSE_WIDTH)
      target = MAX_PULSE_WIDTH;
    else
      target = value;
    m_middleCurve.retarget(target);
  }
  void retargetLeft(int value) {
  }
protected:
  Curve m_middleCurve;
  Servo m_middleServo;
};

Controller controller;

void setup() {
  controller.setup();
  Serial.begin(9600);
  t0 = millis();
}

void loop() {
  int dt = millis() - t0;
  if (Serial.available()) {
    controller.parseChar(Serial.read());
  };
  controller.update(dt);
  t0 += dt;
}
