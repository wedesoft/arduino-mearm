#ifndef __CURVE_H
#define __CURVE_H

// /usr/share/arduino/hardware/arduino/cores/arduino/CDC.cpp (serial stuff)
class Curve
{
public:
  Curve(float pos, float acceleration = 0):
    m_pos(pos), m_target(0), m_acceleration(acceleration) {}
  float pos(void) { return m_pos; }
  void retarget(float target) { m_target = target; }
  void advance(int time) {}
protected:
  float m_pos;
  float m_target;
  float m_acceleration;
};

#endif
