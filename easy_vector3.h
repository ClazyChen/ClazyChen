#include <GL/glu.h>

struct int_vector3
{
    int x;
    int y;
    int z;
    int_vector3():
        x(0),y(0),z(0)
    {
    }
    int_vector3(int t_x, int t_y, int t_z):
        x(t_x),y(t_y),z(t_z)
    {
    }
};

struct easy_vector3
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    easy_vector3():
        x(0.0f),y(0.0f),z(0.0f)
    {
    }
    easy_vector3(GLfloat t_x, GLfloat t_y, GLfloat t_z):
        x(t_x),y(t_y),z(t_z)
    {
    }
    easy_vector3(int_vector3 input_vector):
        x((GLfloat)(input_vector.x << 1) - 2.0f),
        y((GLfloat)(input_vector.y << 1) - 2.0f),
        z((GLfloat)(input_vector.z << 1) - 2.0f)
    {
    }
    bool operator == (easy_vector3 a)
    {
        return (x == a.x && y == a.y && z == a.z);
    }
};

#define MCC_BLACK easy_vector3(0.0f,0.0f,0.0f)
#define MCC_WHITE easy_vector3(1.0f,1.0f,1.0f)
#define MCC_RED easy_vector3(1.0f,0.0f,0.0f)
#define MCC_GREEN easy_vector3(0.0f,1.0f,0.0f)
#define MCC_BLUE easy_vector3(0.0f,0.0f,1.0f)
#define MCC_YELLOW easy_vector3(1.0f,1.0f,0.0f)
#define MCC_ORANGE easy_vector3(1.0f,0.5f,0.0f)
#define MCC_PINK easy_vector3(1.0f,0.0f,1.0f)
