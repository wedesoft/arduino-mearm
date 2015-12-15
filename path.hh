#ifndef __PATH_HH
#define __PATH_HH

#include "profile.hh"

class Path
{
public:
  Path(void): m_offset(0) {
    memset(&m_time, 0, sizeof(m_time));
  }
  float pos(void) {
    return m_offset + m_profile[0].value(m_time[0]) + m_profile[1].value(m_time[1]);
  }
  float update(float time) {
    update(time, 0);
    update(time, 1);
    return pos();
  }
  void update(float time, int index) {
    m_time[index] += time;
    if (m_time[index] >= m_profile[index].duration()) {
      m_offset += m_profile[index].distance();
      m_profile[index] = Profile();
    };
  }
  void stop(float pos) {
    m_offset = pos;
    m_profile[0] = Profile();
    m_profile[1] = Profile();
  }
  void retarget(float target, float duration) {
    retarget(target, duration, 0) || retarget(target, duration, 1);
  }
  float target(void) {
    return m_offset + m_profile[0].distance() + m_profile[1].distance();
  }
  bool retarget(float value, float duration, int index) {
    if (m_profile[index].empty()) {
      m_time[index] = 0;
      m_profile[index] = Profile(value - target(), duration);
      return true;
    } else
      return false;
  };
protected:
  Profile m_profile[2];
  float m_time[2];
  float m_offset;
};

#endif
