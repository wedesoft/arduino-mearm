#ifndef __CONTROLLER_HH
#define __CONTROLLER_HH

class ControllerBase
{
public:
  typedef enum { Middle, Left, Right, Claw } Drive;
  ControllerBase(void): m_number(0) {}
  virtual ~ControllerBase() {}
  void parseChar(char c) {
    switch (c) {
    case 't':
      reportTime();
      break;
    case 'm':
      if (m_number > 0) {
        retargetMiddle(m_number);
        m_number = 0;
      } else
        reportPosition(Middle);
      break;
    case 'l':
      if (m_number > 0) {
        retargetLeft(m_number);
        m_number = 0;
      } else
        reportPosition(Left);
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
  virtual void retargetMiddle(int) = 0;
  virtual void retargetLeft(int) = 0;
protected:
  int m_number;
};

#endif
