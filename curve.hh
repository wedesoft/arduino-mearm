#ifndef __CURVE_H
#define __CURVE_H

// /usr/share/arduino/hardware/arduino/cores/arduino/CDC.cpp (serial stuff)
class Curve
{
public:
  Curve(float pos, float bound = 0):
    m_pos(pos), m_speed(0), m_acceleration(0), m_target(0), m_bound(bound) {}
  float pos(void) { return m_pos; }
  void retarget(float target) {
    m_target = target;
    m_acceleration = m_bound;
  }
  void advance(int time) {
    m_pos += 0.5 * m_acceleration * time * time + m_speed * time;
    m_speed += m_acceleration * time;
  }
protected:
  float m_pos;
  float m_speed;
  float m_acceleration;
  float m_target;
  float m_bound;
};

#endif
