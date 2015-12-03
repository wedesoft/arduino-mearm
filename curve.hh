#ifndef __CURVE_HH
#define __CURVE_HH

#include <math.h>

class Curve
{
public:
  Curve(void): m_pos(0), m_speed(0), m_acceleration(0), m_target(0), m_bound(1), m_state(halt) {}
  float pos(void) { return m_pos; }
  void setBound(float bound) { m_bound = bound; }
  float target(void) { return m_target; }
  void retarget(float target) {
    m_target = target;
    m_acceleration = (target >= stopValue() ? 1 : -1) * m_bound;
    m_state = accelerate;
  }
  void advance(float time) {
    m_pos += 0.5 * m_acceleration * time * time + m_speed * time;
    m_speed += m_acceleration * time;
  }
  float update(float time) {
    float t;
    switch (m_state) {
    case accelerate:
      t = reverseTime();
      if (time > t) {
        advance(t);
        m_acceleration = -m_acceleration;
        m_state = decelerate;
        update(time - t);
      } else
        advance(time);
      break;
    case decelerate:
      t = stopTime();
      if (time > t) {
        advance(t);
        m_pos = m_target;
        finish();
      } else
        advance(time);
      break;
    default:
      break;
    };
    return m_pos;
  }
  float reverseTime(void) {
    char sign = m_acceleration >= 0 ? 1 : -1;
    return (sqrt(2 * m_speed * m_speed + 4 * m_acceleration * (m_target - m_pos)) - 2 * sign * m_speed) / (2 * m_bound);
  }
  void stop(float pos) {
    m_pos = pos;
    m_target = pos;
    finish();
  }
  void finish(void) {
    m_speed = 0;
    m_acceleration = 0;
    m_state = halt;
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
  enum {accelerate, decelerate, halt} m_state;
};

#endif
