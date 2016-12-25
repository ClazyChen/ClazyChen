#include "cubecolorset.h"
#include "magiccubeglwidget.h"

CubeColorSet::CubeColorSet():
    front(FC_YELLOW), back(FC_WHITE), left(FC_GREEN),
    right(FC_BLUE), up(FC_RED), down(FC_ORANGE)
{
}

CubeColorSet::CubeColorSet(int to_front, int to_back, int to_left, int to_right, int to_up, int to_down):
    front(to_front), back(to_back), left(to_left),
    right(to_right), up(to_up), down(to_down)
{
}
