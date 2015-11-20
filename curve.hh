#ifndef __CURVE_H
#define __CURVE_H

#include <math.h>

// /usr/share/arduino/hardware/arduino/cores/arduino/CDC.cpp (serial stuff)
class Curve
{
public:
  Curve(float pos, float bound):
    m_pos(pos), m_speed(0), m_acceleration(0), m_target(0), m_bound(bound) {}
  float pos(void) { return m_pos; }
  void retarget(float target) {
    m_target = target;
    m_acceleration = (target >= stopValue() ? 1 : -1) * m_bound;
  }
  void advance(int time) {
    m_pos += 0.5 * m_acceleration * time * time + m_speed * time;
    m_speed += m_acceleration * time;
  }
  float reverseTime(void) {
    char sign = m_acceleration >= 0 ? 1 : -1;
    return (sqrt(2 * m_speed * m_speed + 4 * m_acceleration * (m_target - m_pos)) - 2 * sign * m_speed) / (2 * m_bound);
  }
  float stopTime(void) {
    return fabs(m_speed) / m_bound;
  }
  float stopValue(void) {
    float t = stopTime();
    return m_pos + m_speed * t - 0.5 * (m_speed >= 0 ? 1: -1) * m_bound * t * t;
  }
protected:
  float m_pos;
  float m_speed;
  float m_acceleration;
  float m_target;
  float m_bound;
};

#endif
