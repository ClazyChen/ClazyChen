#ifndef SINGLECUBE_H
#define SINGLECUBE_H

#include <GL/glu.h>
#include "rotation.h"

struct cube_color
{
    easy_vector3 front;
    easy_vector3 back;
    easy_vector3 down;
    easy_vector3 up;
    easy_vector3 left;
    easy_vector3 right;
    cube_color():
        front(MCC_BLACK),back(MCC_BLACK),down(MCC_BLACK),
        up(MCC_BLACK),left(MCC_BLACK),right(MCC_BLACK)
    {
    }
    cube_color(easy_vector3 face_color, easy_vector3 back_color, easy_vector3 left_color,
               easy_vector3 right_color, easy_vector3 top_color, easy_vector3 bottom_color):
        front(face_color),back(back_color),down(left_color),
        up(right_color),left(top_color),right(bottom_color)
    {
    }
};

class SingleCube
{
public:
    SingleCube();

    //一个三维向量表示中心位置，此向量各分量+1后变成cube_pointer_by_position的下标
    easy_vector3 center_position;

    /*
     * 坐标系变换说明：
     * --观察坐标系
     * -》原始坐标系（观察坐标系往z-方向推20单位长度，使整个魔方图形在观察范围内）
     * -》环境坐标系（原始坐标系做缩放+旋转变换，控制整个魔方的摆放方向和位置）
     * -》单方格坐标系（播放动画时将环境坐标系旋转到该方格的方向）
     * -》单方格绘制坐标系（单方格坐标系按center_position平移到方格中心）
     */

    //rotate表示从环境坐标系变换到单方格坐标系的旋转变换，仅当播放动画的时候有效
    Rotation rotate;

    cube_color colors;

    //下面两个函数表示旋转变换的复合，在原有的rotate变换上附加一个旋转变换
    void compos_rotation(easy_vector3 New_rotate_axis, GLfloat New_rotate_angle);
    void compos_rotation(Rotation New_rotate);

    bool selected;//表示是否选中
};

#endif // SINGLECUBE_H
