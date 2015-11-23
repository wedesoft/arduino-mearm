#ifndef __CONTROLLER_HH
#define __CONTROLLER_HH

class ControllerBase
{
public:
  typedef enum {Middle, Left, Right, Claw} Drive;
  ControllerBase(void): m_number(0) {}
  virtual ~ControllerBase() {}
  Drive drive(char c) {
    switch (c) {
    case 'l':
      return Left;
    case 'r':
      return Right;
    case 'c':
      return Claw;
    default:
      return Middle;
    };
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
  virtual void reportPosition(Drive) = 0;
  virtual void retargetDrive(Drive, int) = 0;
protected:
  int m_number;
};

#endif
