#include <Servo.h>
#include "../controller.hh"
#include "../curve.hh"

// BASE, SHOULDER, ELBOW, GRIPPER
const int SERVOPIN[] = {11, 9, 10, 6};
const int OFFSET[] = {1380, 1357, 1589, 1327};
const int MIN[] = {544, 856, 544, 544};// must not be below 544
const int MAX[] = {2400, 2246, 1950, 2022};// must not be above 2400
const float RESOLUTION[] = {11.12222, 11.12222, 9.8888, 15.44444};


class ServoCurve: public Curve
{
public:
  ServoCurve(void): Curve(0, 0.00015) {}
};

class Controller: public ControllerBase
{
public:
  Controller(void) {}
  void setup(void) {
    for (int drive=0; drive<DRIVES; drive++)
      m_servo[drive].attach(SERVOPIN[drive]);
  }
  float angleToPWMDrive(float angle, int drive) {
    return angleToPWM(angle, OFFSET[drive], RESOLUTION[drive], MIN[drive], MAX[drive]);
  }
  void update(int dt) {
    for (int drive=0; drive<DRIVES; drive++) {
      float pwm = angleToPWMDrive(m_curve[drive].update(dt), drive);
      m_servo[drive].writeMicroseconds(round(pwm));
    };
  };
  void reportTime(void) {
    Serial.print(millis() * 0.001);
    Serial.write("\r\n");
  }
  void reportPosition(int drive) {
    Serial.print(m_curve[drive].pos());
    Serial.write("\r\n");
  }
  void reportPWM(int drive) {
    float pwm = angleToPWMDrive(m_curve[drive].pos(), drive);
    Serial.print(round(pwm));
    Serial.write("\r\n");
  }
  void retargetDrive(int drive, float target) {
    float pwm = angleToPWMDrive(target, drive);
    float angle = pwmToAngle(pwm, OFFSET[drive], RESOLUTION[drive]);
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
