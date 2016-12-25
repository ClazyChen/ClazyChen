#ifndef CUBECOLORSET_H
#define CUBECOLORSET_H

class CubeColorSet
{
public:
    CubeColorSet();
    CubeColorSet(int to_front, int to_back, int to_left,
                 int to_right, int to_up, int to_down);

    int front;
    int back;
    int left;
    int right;
    int up;
    int down;
};

#endif // CUBECOLORSET_H
