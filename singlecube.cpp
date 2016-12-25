#include "singlecube.h"
#include "rotation.h"
#include <QDebug>

SingleCube::SingleCube()
{
}

void SingleCube::compos_rotation(Rotation New_rotate)
{
    Quaternion now_rotation(rotate);
    //qDebug() << now_rotation.GetAngle() << now_rotation.GetAxisX() << now_rotation.GetAxisY() << now_rotation.GetAxisZ() << endl;
    Quaternion added_rotation(New_rotate);
    Quaternion new_rotation = now_rotation * added_rotation;
    new_rotation.Normalize();
    rotate = Rotation(new_rotation);
}

void SingleCube::compos_rotation(easy_vector3 New_rotate_axis, GLfloat New_rotate_angle)
{
    compos_rotation(Rotation(New_rotate_axis, New_rotate_angle));
}

