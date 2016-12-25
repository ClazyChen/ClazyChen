#ifndef ROTATION_H
#define ROTATION_H

#include <GL/glu.h>
#include "easy_vector3.h"

class Rotation;

class Quaternion
{
public:
    Quaternion();
    Quaternion(GLfloat to_x, GLfloat to_y, GLfloat to_z, GLfloat to_w);
    Quaternion(GLfloat angle, easy_vector3 axis);
    Quaternion(Rotation rotation);
    ~Quaternion();
    GLfloat x, y, z, w;

    void Normalize();
    GLfloat Length();
    GLfloat SqrLength();
    Quaternion Inverse();
    Quaternion Conjugate();
    friend easy_vector3 operator* (Quaternion a, easy_vector3 b);
    friend Quaternion operator/ (Quaternion a, GLfloat b);
    friend Quaternion operator* (Quaternion a, Quaternion b);
    GLfloat GetAngle();
    GLfloat GetAxisX();
    GLfloat GetAxisY();
    GLfloat GetAxisZ();
};

class Rotation
{
public:
    Rotation();
    Rotation(easy_vector3 to_axis, GLfloat to_angle);
    Rotation(Quaternion quaternion);

    GLfloat angle;
    easy_vector3 axis;
    friend Rotation operator* (Rotation a, Rotation b);
};

#endif // ROTATION_H
