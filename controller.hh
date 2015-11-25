#ifndef __CONTROLLER_HH
#define __CONTROLLER_HH

const int BASE     = 0;
const int ELBOW    = 1;
const int SHOULDER = 2;
const int GRIPPER  = 3;
const int DRIVES   = 4;

class ControllerBase
{
public:
  ControllerBase(void): m_number(0), m_fraction(0), m_sign(0) {}
  virtual ~ControllerBase() {}
  int drive(char c) {
    switch (c) {
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
  float clip(float value, float lower, float upper) {
    return value < lower ? lower : value > upper ? upper : value;
  }
  float angleToPWM(float angle, float offset, float resolution, int lower, int upper) {
    return clip(offset + angle * resolution, lower, upper);
  }
  float pwmToAngle(int pwm, float offset, float resolution) {
    return (pwm - offset) / resolution;
  }
  void resetNumber(void) {
    m_number = 0;
    m_fraction = 0;
    m_sign = 0;
  };
  void parseChar(char c) {
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
      if (m_sign != 0) {
        if (m_fraction == 0) m_fraction = 1;
        retargetDrive(drive(c), m_number * m_fraction * m_sign);
        resetNumber();
      } else
        reportPosition(drive(c));
      break;
    default:
      if (c >= '0' && c <= '9') {
        m_number = 10 * m_number + (c - '0');
        m_fraction *= 0.1;
        if (m_sign == 0) m_sign = 1;
      } else {
        resetNumber();
        stopDrives();
      };
    };
  }
  virtual void reportTime(void) = 0;
  virtual void reportPosition(int) = 0;
  virtual void retargetDrive(int, float) = 0;
  virtual void stopDrives(void) = 0;
protected:
  float m_number;
  float m_fraction;
  char m_sign;
};

#endif
