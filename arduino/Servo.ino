#include <Servo.h>
#include "../controller.hh"
#include "../curve.hh"

unsigned long t0;

class Controller: public ControllerBase
{
public:
  Controller(void):
    m_middleCurve(DEFAULT_PULSE_WIDTH, 0.0015),
    m_leftCurve(DEFAULT_PULSE_WIDTH, 0.0015),
    m_rightCurve(DEFAULT_PULSE_WIDTH, 0.0015),
    m_clawCurve(DEFAULT_PULSE_WIDTH, 0.0015)
    {}
  void setup(void) {
    m_middleServo.attach(11);
    m_leftServo.attach(10);
    m_rightServo.attach(9);
    m_clawServo.attach(6);
  }
  void update(int dt) {
    m_middleCurve.update(dt);
    m_middleServo.writeMicroseconds(m_middleCurve.pos());
    m_leftCurve.update(dt);
    m_leftServo.writeMicroseconds(m_leftCurve.pos());
    m_rightCurve.update(dt);
    m_rightServo.writeMicroseconds(m_rightCurve.pos());
    m_clawCurve.update(dt);
    m_clawServo.writeMicroseconds(m_clawCurve.pos());
  };
  void reportTime(void) {
    Serial.print(millis());
    Serial.write("\r\n");
  }
  void reportPosition(Drive drive) {
    int pos = 0;
    switch (drive) {
    case Left:
      pos = m_leftCurve.pos();
      break;
    case Right:
      pos = m_rightCurve.pos();
      break;
    case Claw:
      pos = m_clawCurve.pos();
      break;
    default:
      pos = m_middleCurve.pos();
    };
    Serial.print(pos);
    Serial.write("\r\n");
  }
  void retargetDrive(Drive drive, int value) {
    int target;
    if (value < MIN_PULSE_WIDTH)
      target = MIN_PULSE_WIDTH;
    else if (value > MAX_PULSE_WIDTH)
      target = MAX_PULSE_WIDTH;
    else
      target = value;
    switch (drive) {
    case Left:
      m_leftCurve.retarget(target);
      break;
    case Right:
      m_rightCurve.retarget(target);
      break;
    case Claw:
      m_clawCurve.retarget(target);
      break;
    default:
      m_middleCurve.retarget(target);
    };
  }
protected:
  Curve m_middleCurve;
  Curve m_leftCurve;
  Curve m_rightCurve;
  Curve m_clawCurve;
  Servo m_middleServo;
  Servo m_leftServo;
  Servo m_rightServo;
  Servo m_clawServo;
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
