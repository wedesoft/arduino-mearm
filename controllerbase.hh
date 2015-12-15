#ifndef __CONTROLLER_HH
#define __CONTROLLER_HH

#include "calibration.hh"
#include "path.hh"

const int BASE     = 0;
const int SHOULDER = 1;
const int ELBOW    = 2;
const int GRIPPER  = 3;
const int DRIVES   = 4;

class ControllerBase
{
public:
  ControllerBase(void): m_number(0), m_fraction(0), m_sign(0), m_load(false), m_save(false) {
    memset(m_teach, sizeof(m_teach), 0);
  }
  virtual ~ControllerBase() {}
  Path &curve(int drive) { return m_curve[drive]; }
  int drive(char c) {
    switch (c) {
    case 'e':
    case 'E':
      return ELBOW;
    case 's':
    case 'S':
      return SHOULDER;
    case 'g':
    case 'G':
      return GRIPPER;
    default:
      return BASE;
    };
  }
  float limit(float value, float lower, float upper) {
    return value < lower ? lower : value > upper ? upper : value;
  }
  float clip(int drive, float value) {
    return limit(value, lower(drive), upper(drive));
  }
  float angleToPWM(int drive, float angle) {
    return offset(drive) + angle * resolution(drive);
  }
  float pwmToAngle(int drive, float pwm) {
    return (pwm - offset(drive)) / resolution(drive);
  }
  float limitArm(int drive, float target)
  {
    float other;
    switch (drive) {
    case ELBOW:
      other = m_curve[SHOULDER].target();
      return limit(target, -45.0 - other, 45.0 - other);
    case SHOULDER:
      other = m_curve[ELBOW].target();
      return limit(target, -45.0 - other, 45.0 - other);
    default:
      return target;
    };
  }
  void resetNumber(void) {
    m_number = 0;
    m_fraction = 0;
    m_sign = 0;
  };
  void parseChar(char c) {
    if (m_load) {
      if (c >= 'a' && c <= 'z') {
        for (int i=0; i<DRIVES; i++)
          targetAngle(i, m_teach[c - 'a'][i]);
      } else
        stopDrives();
      resetNumber();
      m_load = false;
    } else if (m_save) {
      if (c >= 'a' && c <= 'z') {
        for (int i=0; i<DRIVES; i++)
          m_teach[c - 'a'][i] = m_curve[i].target();
      } else
        stopDrives();
      resetNumber();
      m_save = false;
    } else {
      switch (c) {
      case 't':
        reportTime();
        break;
      case '.':
        if (m_fraction > 0)
          resetNumber();
        else
          m_fraction = 1;
        break;
      case '-':
        m_sign = m_sign == 0 ? -1 : -m_sign;
        break;
      case 'b':
      case 'e':
      case 's':
      case 'g':
      case 'B':
      case 'E':
      case 'S':
      case 'G':
        if (m_sign != 0) {
          if (m_fraction == 0) m_fraction = 1;
          if (isupper(c))
            targetPWM(drive(c), m_number * m_fraction * m_sign);
          else
            targetAngle(drive(c), m_number * m_fraction * m_sign);
          resetNumber();
        } else
          if (isupper(c))
            reportPWM(angleToPWM(drive(c), m_curve[drive(c)].pos()));
          else
            reportAngle(m_curve[drive(c)].pos());
        break;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        m_number = 10 * m_number + (c - '0');
        m_fraction *= 0.1;
        if (m_sign == 0) m_sign = 1;
        break;
      case '@':
        m_load = true;
        break;
      case 'm':
        m_save = true;
        break;
      default:
        resetNumber();
        stopDrives();
      };
    };
  }
  void targetPWM(int drive, float pwm) {
    float angle = limitArm(drive, pwmToAngle(drive, clip(drive, pwm)));
    m_curve[drive].retarget(angle, Profile::timeRequired(fabs(angle - m_curve[drive].target()), MAXJERK));
  }
  void targetAngle(int drive, float angle) {
    targetPWM(drive, angleToPWM(drive, angle));
  }
  void update(int dt) {
    for (int drive=0; drive<DRIVES; drive++)
      writePWM(drive, round(angleToPWM(drive, m_curve[drive].update(dt))));
  }
  void stopDrives(void) {
    for (int drive=0; drive<DRIVES; drive++)
      m_curve[drive].stop(m_curve[drive].pos());
  }
  virtual int offset(int drive) = 0;
  virtual float resolution(int drive) = 0;
  virtual int lower(int drive) = 0;
  virtual int upper(int drive) = 0;
  virtual void reportTime(void) = 0;
  virtual void reportAngle(float) = 0;
  virtual void reportPWM(float) = 0;
  virtual void writePWM(int, int) = 0;
protected:
  float m_number;
  float m_fraction;
  char m_sign;
  bool m_load;
  bool m_save;
  float m_teach[26][DRIVES];
  Path m_curve[DRIVES];
};

#endif
