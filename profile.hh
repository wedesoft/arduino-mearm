#ifndef __PROFILE_HH
#define __PROFILE_HH

#include <math.h>

class Profile
{
public:
  Profile(void) { reset(); }
  Profile(float distance, float duration) {
    reset(distance, duration);
  }
  void reset(void) {
    m_distance = 0;
    m_duration = 0;
  }
  void reset(float distance, float duration) {
    m_distance = distance;
    m_duration = duration;
    m_c5 =   6 * distance / powf(m_duration, 5);
    m_c4 = -15 * distance / powf(m_duration, 4);
    m_c3 =  10 * distance / powf(m_duration, 3);
  };
  float distance(void) { return m_distance; }
  float duration(void) { return m_duration; }
  bool empty(void) { return m_duration == 0; }
  float value(float time) {
    float retval;
    if (time > 0)
      if (time < m_duration)
        retval = ((m_c5 * time + m_c4) * time + m_c3) * time * time * time;
      else
        retval = m_distance;
    else
      retval = 0;
    return retval;
  }
  static float timeRequired(float distance, float maxJerk) {
    return cbrtf(60 * distance / maxJerk);
  }
protected:
  float m_distance;
  float m_duration;
  float m_c5;
  float m_c4;
  float m_c3;

};

#endif
