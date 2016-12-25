#ifndef ANGLE_H
#define ANGLE_H

#endif // ANGLE_H

#include <cmath>
#include <GL/glu.h>

#define MY_PI (GLfloat)M_PI

GLfloat AngleToRadian(GLfloat angle)
{
    return angle*MY_PI/180.0f;
}

GLfloat RadianToAngle(GLfloat radian)
{
    return radian*180.0f/MY_PI;
}
