#ifndef __CALIBRATION_HH
#define __CALIBRATION_HH

const int BASE     = 0;
const int SHOULDER = 1;
const int ELBOW    = 2;
const int GRIPPER  = 3;
const int DRIVES   = 4;

#warning "Adjust these values for your robot to avoid self-collisions!"
const int SERVOPIN[] = {11, 9, 10, 6};
const int OFFSET[] = {1380, 1357, 1589, 1327};// normally around 1500
const int MIN[] = {544, 856, 544, 544};// must not be below 544
const int MAX[] = {2400, 2246, 1786, 2022};// must not be above 2400
const float RESOLUTION[] = {11.12222, 11.12222, 9.8888, 15.44444};

const float BOUND = 0.00015;

#endif
