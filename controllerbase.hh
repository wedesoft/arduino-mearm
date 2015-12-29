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
  ControllerBase(void): m_number(0), m_fraction(0), m_sign(0), m_load(false), m_save(false), m_index(0) {
    memset(m_teach, 0, sizeof(m_teach));
    memset(m_configuration, 0, sizeof(m_configuration));
  }
  virtual ~ControllerBase() {}
  Path &curve(int drive) { return m_curve[drive]; }
  int drive(char c) {
    switch (tolower(c)) {
    case 'e':
      return ELBOW;
    case 's':
      return SHOULDER;
    case 'g':
      return GRIPPER;
    default:
      return BASE;
    };
  }
  float target(int drive) {
    return m_curve[drive].target();
  }
  float limit(float value, float lower, float upper) {
    return value < lower ? lower : value > upper ? upper : value;
  }
  float angleToPWM(int drive, float angle) {
    return offset(drive) + angle * resolution(drive);
  }
  float pwmToAngle(int drive, float pwm) {
    return (pwm - offset(drive)) / resolution(drive);
  }
  float clipPWM(int drive, float value) {
    return limit(value, lower(drive), upper(drive));
  }
  float clipAngle(int drive, float value) {
    return pwmToAngle(drive, clipPWM(drive, angleToPWM(drive, value)));
  }
  float limitJoint(float value, float other) {
    return limit(value, -45 - other, 45 - other);
  }
  float limitArmAngle(int drive, float value)
  {
    switch (drive) {
    case ELBOW:
      return limitJoint(value, target(SHOULDER));
    case SHOULDER:
      return limitJoint(value, target(ELBOW));
    default:
      return value;
    };
  }
  float limitElbowAngle(int drive, float value, float shoulder)
  {
    return drive == ELBOW ? limitJoint(value, shoulder) : value;
  }
  void saveTeachPoint(int index) {
    for (int i=0; i<DRIVES; i++)
      m_teach[index][i] = target(i);
  }
  void takeConfigurationValue(void) {
    if (m_index < 4) {
      float angle = limitElbowAngle(m_index, clipAngle(m_index, number()), m_configuration[SHOULDER]);
      m_configuration[m_index] = angle;
      m_index++;
    };
    resetNumber();
  }
  float timeRequired(int drive, float angle) {
    return Profile::timeRequired(fabs(angle - target(drive)), MAXJERK);
  }
  void targetAngleUnsafe(int drive, float angle, float time) {
    m_curve[drive].retarget(angle, time);
  }
  void targetPWM(int drive, float pwm) {
    float angle = limitArmAngle(drive, pwmToAngle(drive, clipPWM(drive, pwm)));
    targetAngleUnsafe(drive, angle, timeRequired(drive, angle));
  }
  void targetAngle(int drive, float value) {
    float angle = limitArmAngle(drive, clipAngle(drive, value));
    targetAngleUnsafe(drive, angle, timeRequired(drive, angle));
  }
  void targetPoint(float point[])
  {
    float time = 0;
    for (int i=0; i<DRIVES; i++) {
      float driveTime = timeRequired(i, point[i]);
      time = time < driveTime ? driveTime : time;
    };
    for (int i=0; i<DRIVES; i++)
      targetAngleUnsafe(i, point[i], time);
  }
  void update(float dt) {
    for (int drive=0; drive<DRIVES; drive++)
      writePWM(drive, round(angleToPWM(drive, m_curve[drive].update(dt))));
  }
  void stopDrives(void) {
    resetParser();
    for (int drive=0; drive<DRIVES; drive++)
      m_curve[drive].stop(m_curve[drive].pos());
  }
  float number(void) {
    float fraction = (m_fraction == 0) ? 1 : m_fraction;
    return m_number * fraction * m_sign;
  }
  void resetNumber(void) {
    m_number = 0;
    m_fraction = 0;
    m_sign = 0;
  }
  void resetParser(void) {
    resetNumber();
    m_index = 0;
  }
  void parseChar(char c) {
    if (m_load) {
      if (c >= 'a' && c <= 'l')
        targetPoint(m_teach[c - 'a']);
      else
        stopDrives();
      resetParser();
      m_load = false;
    } else if (m_save) {
      if (c >= 'a' && c <= 'l')
        saveTeachPoint(c - 'a');
      else
        stopDrives();
      resetParser();
      m_save = false;
    } else {
      switch (c) {
      case 't':
        reportTime();
        resetParser();
        break;
      case '.':
        if (m_fraction > 0)
          resetParser();
        else
          m_fraction = 1;
        break;
      case '-':
        m_sign = m_sign == 0 ? -1 : -m_sign;
        m_number = 0;
        m_fraction = 0;
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
          if (isupper(c))
            targetPWM(drive(c), number());
          else
            targetAngle(drive(c), number());
        } else
          if (isupper(c))
            reportPWM(angleToPWM(drive(c), m_curve[drive(c)].pos()));
          else
            reportAngle(m_curve[drive(c)].pos());
        resetParser();
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
      case '\'':
        m_load = true;
        break;
      case 'm':
        m_save = true;
        break;
      case ' ':
        takeConfigurationValue();
        break;
      case 'c':
        takeConfigurationValue();
        targetPoint(m_configuration);
        resetParser();
        break;
      default:
        stopDrives();
      };
    };
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
  float m_teach[12][DRIVES];
  int m_index;
  float m_configuration[4];
  Path m_curve[DRIVES];
};

#endif
