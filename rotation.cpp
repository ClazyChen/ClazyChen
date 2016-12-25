#include "rotation.h"
#include "mymath.h"
#include "singlecube.h"

Quaternion::Quaternion():
    x(0.0f),y(0.0f),z(0.0f),w(1.0f)
{
}

Quaternion::Quaternion(GLfloat to_x, GLfloat to_y, GLfloat to_z, GLfloat to_w):
    x(to_x),y(to_y),z(to_z),w(to_w)
{
    Normalize();
}

Quaternion::Quaternion(GLfloat angle, easy_vector3 axis)
{
    GLfloat length_axis_vector = (GLfloat)sqrt((double)(axis.x*axis.x + axis.y*axis.y + axis.z*axis.z));
    x = axis.x / length_axis_vector * (GLfloat)sin((double)AngleToRadian(angle)/2.0);
    y = axis.y / length_axis_vector * (GLfloat)sin((double)AngleToRadian(angle)/2.0);
    z = axis.z / length_axis_vector * (GLfloat)sin((double)AngleToRadian(angle)/2.0);
    w = (GLfloat)cos((double)AngleToRadian(angle)/2.0);
}

Quaternion::Quaternion(Rotation rotation)
{
    (*this) = Quaternion(rotation.angle, rotation.axis);
}

Quaternion::~Quaternion()
{
}

GLfloat Quaternion::GetAngle()
{
    return RadianToAngle(2.0f*acos((double)w));
}

GLfloat Quaternion::GetAxisX()
{
    return (w<=-1.0f || w>=1.0f?0.0f:x/(GLfloat)sqrt(1.0-(double)(w*w)));
}

GLfloat Quaternion::GetAxisY()
{
    return (w<=-1.0f || w>=1.0f?0.0f:y/(GLfloat)sqrt(1.0-(double)(w*w)));
}

GLfloat Quaternion::GetAxisZ()
{
    return (w<=-1.0f || w>=1.0f?1.0f:z/(GLfloat)sqrt(1.0-(double)(w*w)));
}

GLfloat Quaternion::Length()
{
    return ((GLfloat)sqrt((double)(x*x+y*y+z*z+w*w)));
}

GLfloat Quaternion::SqrLength()
{
    return x*x+y*y+z*z+w*w;
}

Quaternion Quaternion::Conjugate()
{
    return Quaternion(-x, -y, -z, w);
}

Quaternion Quaternion::Inverse()
{
    return Conjugate() / SqrLength();
}

void Quaternion::Normalize()
{
    GLfloat length_temp = Length();
    x /= length_temp;
    y /= length_temp;
    z /= length_temp;
    w /= length_temp;
}

Quaternion operator * (Quaternion a, Quaternion b)
{
    return Quaternion(a.x*b.w+a.w*b.x+a.y*b.z-a.z*b.y,
                    a.y*b.w+a.w*b.y+a.z*b.x-a.x*b.z,
                    a.z*b.w+a.w*b.z+a.x*b.y-a.y*b.x,
                    a.w*b.w-a.x*b.x-a.y*b.y-a.z*b.z);
}

Quaternion operator / (Quaternion a, GLfloat b)
{
    return Quaternion(a.x / b, a.y / b, a.z / b, a.w / b);
}

easy_vector3 operator * (Quaternion a, easy_vector3 b)
{
    Quaternion temp = a * Quaternion(b.x, b.y, b.z, 0) * a.Inverse();
    return easy_vector3(temp.x, temp.y, temp.z);
}

Rotation::Rotation():
    angle(0.0f),axis(easy_vector3(0.0f, 0.0f, 1.0f))
{
}

Rotation::Rotation(easy_vector3 to_axis, GLfloat to_angle)
{
    Quaternion quaternion(to_angle, to_axis);
    angle = quaternion.GetAngle();
    axis = easy_vector3(quaternion.GetAxisX(), quaternion.GetAxisY(), quaternion.GetAxisZ());
}

Rotation::Rotation(Quaternion quaternion):
    angle(quaternion.GetAngle()),axis(quaternion.GetAxisX(),
                                      quaternion.GetAxisY(),
                                      quaternion.GetAxisZ())
{
}

Rotation operator* (Rotation a, Rotation b)
{
    Quaternion q_a(a);
    Quaternion q_b(b);
    return Rotation(q_a * q_b);
}
