#ifndef __PROFILE_HH
#define __PROFILE_HH

#include <math.h>

class Profile
{
public:
  Profile(float distance, float maxJerk=0.01): m_distance(distance) {
    m_duration = cbrtf(60 * distance / maxJerk);
    m_c5 =   6 * distance / powf(m_duration, 5);
    m_c4 = -15 * distance / powf(m_duration, 4);
    m_c3 =  10 * distance / powf(m_duration, 3);
  }
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
  };
  float duration(void) {
    return m_duration;
  }
protected:
  float m_distance;
  float m_duration;
  float m_c5;
  float m_c4;
  float m_c3;

};

#endif
