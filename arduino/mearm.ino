#include <Servo.h>
#include "../controller.hh"
#include "../curve.hh"

const int RESOLUTION = 11.333333333;
const int OFFSET[] = {1380, 1610, 1500, 1500};

class ServoCurve: public Curve
{
public:
  ServoCurve(void): Curve(DEFAULT_PULSE_WIDTH, 0.00015) {}
};

class Controller: public ControllerBase
{
public:
  Controller(void) {}
  void setup(void) {
    int servoPin[] = {11, 10, 9, 6};
    for (int drive=0; drive<DRIVES; drive++)
      m_servo[drive].attach(servoPin[drive]);
  }
  float angleToPWMDrive(float angle, int drive) {
    return angleToPWM(angle, OFFSET[drive], RESOLUTION, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  }
  void update(int dt) {
    for (int drive=0; drive<DRIVES; drive++) {
      float pwm = angleToPWMDrive(m_curve[drive].update(dt), drive);
      m_servo[drive].writeMicroseconds(round(pwm));
    };
  };
  void reportTime(void) {
    Serial.print(millis());
    Serial.write("\r\n");
  }
  void reportPosition(int drive) {
    Serial.print(m_curve[drive].pos());
    Serial.write("\r\n");
  }
  void retargetDrive(int drive, float target) {
    float pwm = angleToPWMDrive(target, drive);
    float angle = pwmToAngle(pwm, OFFSET[drive], RESOLUTION);
    m_curve[drive].retarget(angle);
  }
  void stopDrives(void)
  {
    for (int drive=0; drive<DRIVES; drive++)
      m_curve[drive].stop();
  }
protected:
  ServoCurve m_curve[4];
  Servo m_servo[4];
};

unsigned long t0;

Controller controller;

void setup() {
  controller.setup();
  Serial.begin(9600);
  t0 = millis();
}

void loop() {
  int dt = millis() - t0;
  if (Serial.available())
    controller.parseChar(Serial.read());
  controller.update(dt);
  t0 += dt;
}
