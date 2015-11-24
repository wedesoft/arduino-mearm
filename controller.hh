#ifndef __CONTROLLER_HH
#define __CONTROLLER_HH

#define BASE     0
#define ELBOW    1
#define SHOULDER 2
#define GRIPPER  3
#define DRIVES   4

class ControllerBase
{
public:
  ControllerBase(void): m_number(0) {}
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
  int clip(int value, int lower, int upper) {
    return value < lower ? lower : value > upper ? upper : value;
  }
  void parseChar(char c) {
    switch (c) {
    case 't':
      reportTime();
      break;
    case 'b':
    case 'e':
    case 's':
    case 'g':
      if (m_number > 0) {
        retargetDrive(drive(c), m_number);
        m_number = 0;
      } else
        reportPosition(drive(c));
      break;
    default:
      if (c >= '0' && c <= '9')
        m_number = 10 * m_number + (c - '0');
      else {
        m_number = 0;
        stopDrives();
      };
    };
  }
  virtual void reportTime(void) = 0;
  virtual void reportPosition(int) = 0;
  virtual void retargetDrive(int, int) = 0;
  virtual void stopDrives(void) = 0;
protected:
  int m_number;
};

#endif
