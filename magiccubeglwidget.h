#ifndef MAGICCUBEGLWIDGET_H
#define MAGICCUBEGLWIDGET_H

#include <QGLWidget>
#include <GL/glu.h>

#define DRT_CLOCKWISE -1
#define DRT_COUNTERCLOCKWISE 1

#define POS_NONE    0
#define POS_FRONT   1
#define POS_BACK    -1
#define POS_RIGHT   2
#define POS_LEFT    -2
#define POS_UP      4
#define POS_DOWN    -4

#define FC_BLACK  0
#define FC_WHITE  -2
#define FC_RED    -4
#define FC_GREEN  -8
#define FC_YELLOW 2
#define FC_ORANGE 4
#define FC_BLUE   8

#define S_BLACK  0
#define S_WHITE  1
#define S_YELLOW 2
#define S_RED    4
#define S_GREEN  8
#define S_BLUE   16
#define S_ORANGE 32

class SingleCube;
class easy_vector3;
class QTimer;
class Rotation;
class CubeColorSet;
class RubicCubeSolver;
class SetFlashSpeedWidget;
class Tester;

class MagicCubeGlWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MagicCubeGlWidget(QWidget *parent = 0);
    ~MagicCubeGlWidget();

signals:
    void operationend();
    void startingend();

public slots:
    void Flash_Rotate_Play();
    void UpdateSpeed(int user_operation, int auto_solve);
    void RandomOperateOnce();
    void StartGame();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void BuildCube(SingleCube *to_build);
    void Rotate_Face_Start(int face_color, int direction);
    void Update_ColorSet();
    void SelectByColor(int face_color);
    int FindPositionByColor(int face_color);
    int FindPositionByColorForNewFile(easy_vector3 face_color);
    void ResetSelect();
    void ReadFromFile();

private:
    bool full_screen;
    bool key_operation_on;

    bool starting;

    CubeColorSet *color_set;//存储六个方向的面中心块颜色

    GLuint selected_texture;//清华校徽纹理

    SingleCube *cube[3][3][3];//用来存储，指针指定对象后不发生变化

    SingleCube *cube_pointer_by_position[3][3][3];//用来指向某个位置的方格

    void pointer_change();
    /*
     * pointer_change()在动画结束的时候根据temp_face_color和temp_direction的值对cube_pointer_by_position
     * 的指针指向做出修改，转换一轮，同时对cube[][][]->center_position进行修改，使其符合新的位置，然后将
     * cube[][][]->rotate的信息清除
     */

    int pass_rotation_angle;

    int selected_face_color;
    int selected_face_position;

    int temp_face_color;
    int temp_direction;
    bool rotating;

    QTimer *flash_rotate_timer;

    Rotation *environment_rotate;//把绝对坐标系转换成环境坐标系的旋转变换
    GLfloat environment_zoom;

    QPoint temp_mouse_position;
    bool rightbutton_pressed;
    bool leftbutton_pressed;
    int MagicCubeForSolve[6][3][3];//传给吴一凡部分的数组
    void getMagicCubeForSolve();

    RubicCubeSolver *Solver;//调用吴一凡部分解魔方
    void Flash_AutoSolve_Play();
    bool autosolve_on;
    int passed_position;
    void continue_solve(int now_position);// 进行下一个位置的解

    GLfloat rotate_step_length;
    int rotate_angle_once;
    void SetFlashSpeed(int speed);//设置放映速度（1~1000）

    int FlashSpeedWhenUserOperate;
    int FlashSpeedWhenAutoSolve;

    SetFlashSpeedWidget *SpeedSet;
    void ShowSpeedSet();

    void RandomOperate();
    int have_operated;

    int changecolor_Czk_to_Wyf(easy_vector3 colorForChange);
    Tester * tester;
};

#endif // MAGICCUBEGLWIDGET_H
