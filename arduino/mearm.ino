#include <Servo.h>
#include "../calibration.hh"
#include "../controllerbase.hh"

class Controller: public ControllerBase
{
public:
  Controller(void) {}
  void setup(void) {
    for (int drive=0; drive<DRIVES; drive++)
      m_servo[drive].attach(SERVOPIN[drive]);
  }
  int offset(int drive) { return OFFSET[drive]; }
  float resolution(int drive) { return RESOLUTION[drive]; }
  int lower(int drive) { return MIN[drive]; }
  int upper(int drive) { return MAX[drive]; }
  void reportTime(void) {
    Serial.print(millis());
    Serial.write("\r\n");
  }
  void reportAngle(float angle) {
    Serial.print(angle);
    Serial.write("\r\n");
  }
  void reportPWM(float pwm) {
    Serial.print(round(pwm));
    Serial.write("\r\n");
  }
  void writePWM(int drive, int pwm) {
    m_servo[drive].writeMicroseconds(pwm);
  }
protected:
  Servo m_servo[DRIVES];
};

unsigned long t0;

Controller controller;

const int LED = 13;

bool on;

void setup() {
  on = false;
  controller.setup();
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  t0 = millis();
}

void loop() {
  int dt = millis() - t0;
  if (Serial.available())
    controller.parseChar(Serial.read());
  if (dt >= 20) {
    on = !on;
    digitalWrite(LED, on ? HIGH : LOW);
    controller.update(dt);
    t0 += dt;
  };
}
