#ifndef __CONTROLLER_HH
#define __CONTROLLER_HH

#define MIDDLE 0
#define LEFT   1
#define RIGHT  2
#define CLAW   3
#define DRIVES 4

class ControllerBase
{
public:
  ControllerBase(void): m_number(0) {}
  virtual ~ControllerBase() {}
  int drive(char c) {
    switch (c) {
    case 'l':
      return LEFT;
    case 'r':
      return RIGHT;
    case 'c':
      return CLAW;
    default:
      return MIDDLE;
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
    case 'm':
    case 'l':
    case 'r':
    case 'c':
      if (m_number > 0) {
        retargetDrive(drive(c), m_number);
        m_number = 0;
      } else
        reportPosition(drive(c));
      break;
    default:
      if (c >= '0' && c <= '9')
        m_number = 10 * m_number + (c - '0');
      else
        m_number = 0;
    };
  }
  virtual void reportTime() = 0;
  virtual void reportPosition(int) = 0;
  virtual void retargetDrive(int, int) = 0;
protected:
  int m_number;
};

#endif
