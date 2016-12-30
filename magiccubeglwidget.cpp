#include "magiccubeglwidget.h"
#include "singlecube.h"
#include "rotation.h"
#include "cubecolorset.h"
#include "universal.h"
#include "rubiccubesolver.h"
#include "setflashspeedwidget.h"
#include <QKeyEvent>
#include <cmath>
#include <QWheelEvent>
#include <QTimer>
#include <QDebug>
#include <cstdlib>
#include <ctime>
#include <QFileDialog>
#include <fstream>
#include "tester.h"

#define HALF_EDGE_LENGTH 0.975f
#define HALF_COLOR_LENGTH 0.9f
#define HALF_FLOAT_LENGTH 0.976f

#define MAX_RANDOM_OPERATION 30

//#define TEST

MagicCubeGlWidget::MagicCubeGlWidget(QWidget *parent) :
    QGLWidget(parent)
{

    full_screen               = false;
    key_operation_on          = false;
    rotating                  = false;
    rightbutton_pressed       = false;
    leftbutton_pressed        = false;
    pass_rotation_angle       = 0.0f;
    environment_rotate        = new Rotation();
    environment_zoom          = 12.0f;
    color_set                 = new CubeColorSet();
    selected_face_color       = FC_BLACK;
    selected_face_position    = POS_NONE;
    Solver                    = new RubicCubeSolver();
    autosolve_on              = false;
    passed_position           = 0;
    FlashSpeedWhenAutoSolve   = 1000;
    FlashSpeedWhenUserOperate = 100;
    have_operated             = 0;
    starting                  = true;
    tester                    = new Tester();

    SetFlashSpeed(FlashSpeedWhenUserOperate);
#ifndef TEST
    connect(this, SIGNAL(operationend()), this, SLOT(RandomOperateOnce()));
#endif
    connect(this, SIGNAL(startingend()), this, SLOT(StartGame()));

    //initialize cubes
    int i, j, k;
    for (i = 0; i < 3; ++ i)
        for (j = 0; j < 3; ++ j)
            for (k = 0; k < 3; ++ k)
            {
                cube[i][j][k] = new SingleCube();
                cube[i][j][k] ->selected = false;
                cube[i][j][k] ->center_position =
                        easy_vector3((GLfloat)i*2.0f-2.0f,
                                     (GLfloat)j*2.0f-2.0f,
                                     (GLfloat)k*2.0f-2.0f);
                cube[i][j][k] ->rotate = Rotation();
                cube[i][j][k] ->colors = cube_color();
                cube_pointer_by_position[i][j][k] = cube[i][j][k];
            }
    for (i = 0; i < 3; ++ i)
        for (j = 0; j < 3; ++ j)
        {
            cube[i][j][2] ->colors.front = MCC_YELLOW;
            cube[i][j][0] ->colors.back = MCC_WHITE;
            cube[i][0][j] ->colors.down = MCC_ORANGE;
            cube[i][2][j] ->colors.up = MCC_RED;
            cube[0][i][j] ->colors.left = MCC_GREEN;
            cube[2][i][j] ->colors.right = MCC_BLUE;
        }
    whiteCubePosition = cube_pointer_by_position[1][1][0];
}

MagicCubeGlWidget::~MagicCubeGlWidget()
{
}

void MagicCubeGlWidget::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);

    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_TEXTURE_2D);
}

void MagicCubeGlWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLint)w, (GLint)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MagicCubeGlWidget::Flash_Rotate_Play()
{
    if (rotating)
    {
        int i, j;
        switch (temp_face_color)
        {
        case FC_YELLOW:
            for (i = 0; i < 3; ++ i)
                for (j = 0; j < 3; ++ j)
                    cube_pointer_by_position[i][j][2] ->compos_rotation(cube[1][1][2] ->center_position, temp_direction*rotate_step_length);
            break;
        case FC_WHITE:
            for (i = 0; i < 3; ++ i)
                for (j = 0; j < 3; ++ j)
                    cube_pointer_by_position[i][j][0] ->compos_rotation(cube[1][1][0] ->center_position, temp_direction*rotate_step_length);
            break;
        case FC_BLUE:
            for (i = 0; i < 3; ++ i)
                for (j = 0; j < 3; ++ j)
                    cube_pointer_by_position[2][i][j] ->compos_rotation(cube[2][1][1] ->center_position, temp_direction*rotate_step_length);
            break;
        case FC_GREEN:
            for (i = 0; i < 3; ++ i)
                for (j = 0; j < 3; ++ j)
                    cube_pointer_by_position[0][i][j] ->compos_rotation(cube[0][1][1] ->center_position, temp_direction*rotate_step_length);
            break;
        case FC_RED:
            for (i = 0; i < 3; ++ i)
                for (j = 0; j < 3; ++ j)
                    cube_pointer_by_position[i][2][j] ->compos_rotation(cube[1][2][1] ->center_position, temp_direction*rotate_step_length);
            break;
        case FC_ORANGE:
            for (i = 0; i < 3; ++ i)
                for (j = 0; j < 3; ++ j)
                    cube_pointer_by_position[i][0][j] ->compos_rotation(cube[1][0][1] ->center_position, temp_direction*rotate_step_length);
            break;
        default:
            return;
        }
        updateGL();
        pass_rotation_angle += (int)rotate_step_length;
        if (pass_rotation_angle >= 90)
        {
            rotating = false;
            flash_rotate_timer ->stop();
            delete flash_rotate_timer;
            pointer_change();
            updateGL();
            key_operation_on = true;
            continue_solve(passed_position);
            if (have_operated > 0)
                emit operationend();
            else
                UpdateSpeed(FlashSpeedWhenUserOperate, FlashSpeedWhenAutoSolve);
        }
    }
}

void MagicCubeGlWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (starting)
    {
        glLoadIdentity();
        selected_texture = bindTexture(QPixmap(":/image/Logo.png"));
        glBindTexture(GL_TEXTURE_2D, selected_texture);
        glBegin(GL_QUADS);
            glColor3f(1.0, 1.0, 1.0);
            glTexCoord2f(1, 1);     glVertex3f(1.5f, 1.0f, -3.0f);
            glTexCoord2f(1, 0);     glVertex3f(1.5f, -1.0f, -3.0f);
            glTexCoord2f(0, 0);     glVertex3f(-1.5f, -1.0f, -3.0f);
            glTexCoord2f(0, 1);     glVertex3f(-1.5f, 1.0f, -3.0f);

        glEnd();
        return;
    }

    int i, j, k;
    for (i = 0; i < 3; ++ i)
        for (j = 0; j < 3; ++ j)
            for (k = 0; k < 3; ++ k)
                BuildCube(cube[i][j][k]);
}

void MagicCubeGlWidget::StartGame()
{
    glClearColor(0.8, 0.8, 0.8, 0.0);
    starting = false;
    key_operation_on = true;
    selected_texture = bindTexture(QPixmap(":/image/tsinghua.bmp"));
    updateGL();
#ifndef TEST
    RandomOperate();
#endif
}

void MagicCubeGlWidget::keyPressEvent(QKeyEvent *event)
{
    if (!key_operation_on)
        return;
    Update_ColorSet();
    selected_face_position = FindPositionByColor(selected_face_color);
    switch (event ->key())
    {
    case Qt::Key_F1:
        full_screen = !full_screen;
        if (full_screen)
        {
            showFullScreen();
        }
        else
        {
            showNormal();
        }
        updateGL();
        break;
    case Qt::Key_F2:
        getMagicCubeForSolve();
        qDebug() << tester ->runCheck();
        break;
    case Qt::Key_F3:
        ShowSpeedSet();
        break;
    case Qt::Key_F4:
        RandomOperate();
        break;
    case Qt::Key_F5:
        tester ->generateTestInput();
        ReadFromFile();
        break;
    case Qt::Key_Escape:
        close();
        break;
    case 'V':
        ResetSelect();
        Rotate_Face_Start(FC_ORANGE, DRT_CLOCKWISE);
        break;
    case 'B':
        ResetSelect();
        Rotate_Face_Start(FC_ORANGE, DRT_COUNTERCLOCKWISE);
        break;
    case 'W':
        ResetSelect();
        selected_face_position = POS_UP;
        SelectByColor(color_set ->up);
        break;
    case 'S':
        ResetSelect();
        selected_face_position = POS_DOWN;
        SelectByColor(color_set ->down);
        break;
    case 'A':
        ResetSelect();
        selected_face_position = POS_LEFT;
        SelectByColor(color_set ->left);
        break;
    case 'D':
        ResetSelect();
        selected_face_position = POS_RIGHT;
        SelectByColor(color_set ->right);
        break;
    case 'Q':
        ResetSelect();
        selected_face_position = POS_FRONT;
        SelectByColor(color_set ->front);
        break;
    case 'E':
        ResetSelect();
        selected_face_position = POS_BACK;
        SelectByColor(color_set ->back);
        break;
    case 'Z':
        ResetSelect();
        updateGL();
        break;
    case Qt::Key_Left:
        if (selected_face_position == POS_FRONT || selected_face_position == POS_DOWN)
            Rotate_Face_Start(selected_face_color, DRT_COUNTERCLOCKWISE);
        else if (selected_face_position == POS_BACK || selected_face_position == POS_UP)
            Rotate_Face_Start(selected_face_color, DRT_CLOCKWISE);
        break;
    case Qt::Key_Right:
        if (selected_face_position == POS_FRONT || selected_face_position == POS_DOWN)
            Rotate_Face_Start(selected_face_color, DRT_CLOCKWISE);
        else if (selected_face_position == POS_BACK || selected_face_position == POS_UP)
            Rotate_Face_Start(selected_face_color, DRT_COUNTERCLOCKWISE);
        break;
    case Qt::Key_Up:
        if (selected_face_position == POS_LEFT)
            Rotate_Face_Start(selected_face_color, DRT_COUNTERCLOCKWISE);
        else if (selected_face_position == POS_RIGHT)
            Rotate_Face_Start(selected_face_color, DRT_CLOCKWISE);
        break;
    case Qt::Key_Down:
        if (selected_face_position == POS_LEFT)
            Rotate_Face_Start(selected_face_color, DRT_CLOCKWISE);
        else if (selected_face_position == POS_RIGHT)
            Rotate_Face_Start(selected_face_color, DRT_COUNTERCLOCKWISE);
        break;
    }
}

void MagicCubeGlWidget::wheelEvent(QWheelEvent *event)
{
    if (event ->delta() > 0)
    {
        environment_zoom -= 1.0f;
        if (environment_zoom < 5.0f)
            environment_zoom = 5.0f;
    }
    else
    {
        environment_zoom += 1.0f;
        if (environment_zoom > 100.0f)
            environment_zoom = 100.0f;
    }
    updateGL();
}

void MagicCubeGlWidget::mousePressEvent(QMouseEvent *event)
{
    if (event ->button() & Qt::RightButton)
    {
        rightbutton_pressed = true;
        temp_mouse_position = event ->pos();
    }
    if (event ->button() & Qt::LeftButton)
    {
        leftbutton_pressed = true;
        temp_mouse_position = event ->pos();
    }
}

void MagicCubeGlWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (starting)
        return;
    if (rightbutton_pressed)
    {
        QPoint new_mouse_position = event ->pos() - temp_mouse_position;
        (*environment_rotate) =
                 Rotation(easy_vector3(1.0f, 0.0f, 0.0f), (GLfloat)new_mouse_position.y()/5.0f)
                * Rotation(easy_vector3(0.0f, 1.0f, 0.0f), (GLfloat)new_mouse_position.x()/5.0f)
                * (*environment_rotate);
        temp_mouse_position = event ->pos();
        updateGL();
    }
    if (leftbutton_pressed)
    {
        QPoint now_mouse_position = event ->pos();
        double center_x = (double)width()/2.0;
        double center_y = (double)height()/2.0;
        double now_x = (double)now_mouse_position.x() - center_x;
        double now_y = (double)now_mouse_position.y() - center_y;
        double temp_x = (double)temp_mouse_position.x() - center_x;
        double temp_y = (double)temp_mouse_position.y() - center_y;
        double temp_angle = temp_x * temp_x + temp_y * temp_y == 0.0 ? 0.0 :
                            acos(temp_x / sqrt(temp_x * temp_x + temp_y * temp_y));
        double now_angle = now_x * now_x + now_y * now_y == 0.0 ? 0.0 :
                            acos(now_x / sqrt(now_x * now_x + now_y * now_y));
        GLfloat rotate_angle = 180.0f / (GLfloat)M_PI * (now_angle - temp_angle);
        (*environment_rotate) =
                Rotation(easy_vector3(0.0f, 0.0f, (now_y < 0 ? 1 : -1) * 1.0f), rotate_angle)
                * (*environment_rotate);
        temp_mouse_position = event ->pos();
        updateGL();
    }
}

void MagicCubeGlWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (starting)
        emit startingend();
    if (event ->button() & Qt::RightButton)
    {
        rightbutton_pressed = false;
    }
    if (event ->button() & Qt::LeftButton)
    {
        leftbutton_pressed = false;
    }
    Update_ColorSet();
}

void MagicCubeGlWidget::BuildCube(SingleCube *to_build)
{
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -environment_zoom);
    glRotatef(environment_rotate ->angle, environment_rotate ->axis.x,
                                          environment_rotate ->axis.y,
                                          environment_rotate ->axis.z);
    glRotatef(to_build ->rotate.angle, to_build ->rotate.axis.x,
                                       to_build ->rotate.axis.y,
                                       to_build ->rotate.axis.z);
    glTranslatef(to_build ->center_position.x,
                 to_build ->center_position.y,
                 to_build ->center_position.z);

    glBindTexture(GL_TEXTURE_2D, selected_texture);

    glBegin(GL_QUADS);

        glColor3f((MCC_BLACK).x, (MCC_BLACK).y, (MCC_BLACK).z);
        if (to_build ->selected)
            glColor3f((MCC_PINK).x, (MCC_PINK).y ,(MCC_PINK).z);
        glVertex3f(HALF_EDGE_LENGTH, HALF_EDGE_LENGTH, HALF_EDGE_LENGTH);
        glVertex3f(-HALF_EDGE_LENGTH, HALF_EDGE_LENGTH, HALF_EDGE_LENGTH);
        glVertex3f(-HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH, HALF_EDGE_LENGTH);
        glVertex3f(HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH, HALF_EDGE_LENGTH);

        glVertex3f(HALF_EDGE_LENGTH, HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH);
        glVertex3f(HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH);
        glVertex3f(-HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH);
        glVertex3f(-HALF_EDGE_LENGTH, HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH);

        glVertex3f(HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH, HALF_EDGE_LENGTH);
        glVertex3f(-HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH, HALF_EDGE_LENGTH);
        glVertex3f(-HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH);
        glVertex3f(HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH);

        glVertex3f(HALF_EDGE_LENGTH, HALF_EDGE_LENGTH, HALF_EDGE_LENGTH);
        glVertex3f(HALF_EDGE_LENGTH, HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH);
        glVertex3f(-HALF_EDGE_LENGTH, HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH);
        glVertex3f(-HALF_EDGE_LENGTH, HALF_EDGE_LENGTH, HALF_EDGE_LENGTH);

        glVertex3f(-HALF_EDGE_LENGTH, HALF_EDGE_LENGTH, HALF_EDGE_LENGTH);
        glVertex3f(-HALF_EDGE_LENGTH, HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH);
        glVertex3f(-HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH);
        glVertex3f(-HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH, HALF_EDGE_LENGTH);

        glVertex3f(HALF_EDGE_LENGTH, HALF_EDGE_LENGTH, HALF_EDGE_LENGTH);
        glVertex3f(HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH, HALF_EDGE_LENGTH);
        glVertex3f(HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH);
        glVertex3f(HALF_EDGE_LENGTH, HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH);

        glColor3f(to_build ->colors.front.x,
                  to_build ->colors.front.y,
                  to_build ->colors.front.z);
        if (to_build == whiteCubePosition && whiteCubePosition == cube_pointer_by_position[1][1][2])
            glTexCoord2f(1.0f, 1.0f);
        glVertex3f(HALF_COLOR_LENGTH, HALF_COLOR_LENGTH, HALF_FLOAT_LENGTH);
        if (to_build == whiteCubePosition && whiteCubePosition == cube_pointer_by_position[1][1][2])
            glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-HALF_COLOR_LENGTH, HALF_COLOR_LENGTH, HALF_FLOAT_LENGTH);
        if (to_build == whiteCubePosition && whiteCubePosition == cube_pointer_by_position[1][1][2])
            glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-HALF_COLOR_LENGTH, -HALF_COLOR_LENGTH, HALF_FLOAT_LENGTH);
        if (to_build == whiteCubePosition && whiteCubePosition == cube_pointer_by_position[1][1][2])
            glTexCoord2f(1.0f, 0.0f);
        glVertex3f(HALF_COLOR_LENGTH, -HALF_COLOR_LENGTH, HALF_FLOAT_LENGTH);

        glColor3f(to_build ->colors.back.x,
                  to_build ->colors.back.y,
                  to_build ->colors.back.z);
        if (to_build == whiteCubePosition && whiteCubePosition == cube_pointer_by_position[1][1][0])
            glTexCoord2f(1.0f, 1.0f);
        glVertex3f(HALF_COLOR_LENGTH, HALF_COLOR_LENGTH, -HALF_FLOAT_LENGTH);
        if (to_build == whiteCubePosition && whiteCubePosition == cube_pointer_by_position[1][1][0])
            glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-HALF_COLOR_LENGTH, HALF_COLOR_LENGTH, -HALF_FLOAT_LENGTH);
        if (to_build == whiteCubePosition && whiteCubePosition == cube_pointer_by_position[1][1][0])
            glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-HALF_COLOR_LENGTH, -HALF_COLOR_LENGTH, -HALF_FLOAT_LENGTH);
        if (to_build == whiteCubePosition && whiteCubePosition == cube_pointer_by_position[1][1][0])
            glTexCoord2f(0.0f, 1.0f);
        glVertex3f(HALF_COLOR_LENGTH, -HALF_COLOR_LENGTH, -HALF_FLOAT_LENGTH);

        glColor3f(to_build ->colors.down.x,
                  to_build ->colors.down.y,
                  to_build ->colors.down.z);
        if (to_build == whiteCubePosition && whiteCubePosition == cube_pointer_by_position[1][0][1])
            glTexCoord2f(1.0f, 1.0f);
        glVertex3f(HALF_COLOR_LENGTH, -HALF_FLOAT_LENGTH, HALF_COLOR_LENGTH);
        if (to_build == whiteCubePosition && whiteCubePosition == cube_pointer_by_position[1][0][1])
            glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-HALF_COLOR_LENGTH, -HALF_FLOAT_LENGTH, HALF_COLOR_LENGTH);
        if (to_build == whiteCubePosition && whiteCubePosition == cube_pointer_by_position[1][0][1])
            glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-HALF_COLOR_LENGTH, -HALF_FLOAT_LENGTH, -HALF_COLOR_LENGTH);
        if (to_build == whiteCubePosition && whiteCubePosition == cube_pointer_by_position[1][0][1])
            glTexCoord2f(1.0f, 0.0f);
        glVertex3f(HALF_COLOR_LENGTH, -HALF_FLOAT_LENGTH, -HALF_COLOR_LENGTH);

        glColor3f(to_build ->colors.up.x,
                  to_build ->colors.up.y,
                  to_build ->colors.up.z);
        if (to_build == whiteCubePosition && whiteCubePosition == cube_pointer_by_position[1][2][1])
            glTexCoord2f(1.0f, 1.0f);
        glVertex3f(HALF_COLOR_LENGTH, HALF_FLOAT_LENGTH, HALF_COLOR_LENGTH);
        if (to_build == whiteCubePosition && whiteCubePosition == cube_pointer_by_position[1][2][1])
            glTexCoord2f(0.0f, 1.0f);
        glVertex3f(HALF_COLOR_LENGTH, HALF_FLOAT_LENGTH, -HALF_COLOR_LENGTH);
        if (to_build == whiteCubePosition && whiteCubePosition == cube_pointer_by_position[1][2][1])
            glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-HALF_COLOR_LENGTH, HALF_FLOAT_LENGTH, -HALF_COLOR_LENGTH);
        if (to_build == whiteCubePosition && whiteCubePosition == cube_pointer_by_position[1][2][1])
            glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-HALF_COLOR_LENGTH, HALF_FLOAT_LENGTH, HALF_COLOR_LENGTH);

        glColor3f(to_build ->colors.left.x,
                  to_build ->colors.left.y,
                  to_build ->colors.left.z);
        if (to_build == whiteCubePosition && whiteCubePosition == cube_pointer_by_position[0][1][1])
            glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-HALF_FLOAT_LENGTH, HALF_COLOR_LENGTH, HALF_COLOR_LENGTH);
        if (to_build == whiteCubePosition && whiteCubePosition == cube_pointer_by_position[0][1][1])
            glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-HALF_FLOAT_LENGTH, HALF_COLOR_LENGTH, -HALF_COLOR_LENGTH);
        if (to_build == whiteCubePosition && whiteCubePosition == cube_pointer_by_position[0][1][1])
            glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-HALF_FLOAT_LENGTH, -HALF_COLOR_LENGTH, -HALF_COLOR_LENGTH);
        if (to_build == whiteCubePosition && whiteCubePosition == cube_pointer_by_position[0][1][1])
            glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-HALF_FLOAT_LENGTH, -HALF_COLOR_LENGTH, HALF_COLOR_LENGTH);

        glColor3f(to_build ->colors.right.x,
                  to_build ->colors.right.y,
                  to_build ->colors.right.z);
        if (to_build == whiteCubePosition && whiteCubePosition == cube_pointer_by_position[2][1][1])
            glTexCoord2f(1.0f, 1.0f);
        glVertex3f(HALF_FLOAT_LENGTH, HALF_COLOR_LENGTH, HALF_COLOR_LENGTH);
        if (to_build == whiteCubePosition && whiteCubePosition == cube_pointer_by_position[2][1][1])
            glTexCoord2f(0.0f, 1.0f);
        glVertex3f(HALF_FLOAT_LENGTH, -HALF_COLOR_LENGTH, HALF_COLOR_LENGTH);
        if (to_build == whiteCubePosition && whiteCubePosition == cube_pointer_by_position[2][1][1])
            glTexCoord2f(0.0f, 0.0f);
        glVertex3f(HALF_FLOAT_LENGTH, -HALF_COLOR_LENGTH, -HALF_COLOR_LENGTH);
        if (to_build == whiteCubePosition && whiteCubePosition == cube_pointer_by_position[2][1][1])
            glTexCoord2f(1.0f, 0.0f);
        glVertex3f(HALF_FLOAT_LENGTH, HALF_COLOR_LENGTH, -HALF_COLOR_LENGTH);
    glEnd();
}

void MagicCubeGlWidget::Rotate_Face_Start(int face_color, int direction)
{
    temp_direction = direction;
    temp_face_color = face_color;
    key_operation_on = false;
    rotating = true;
    pass_rotation_angle = 0;
    flash_rotate_timer = new QTimer(this);
    connect(flash_rotate_timer, SIGNAL(timeout()), this, SLOT(Flash_Rotate_Play()));
    flash_rotate_timer ->start(rotate_angle_once);
}

void MagicCubeGlWidget::pointer_change()
{
    int clock_sequence[8][2] =
    {
        {0, 0}, {0, 1}, {0, 2}, {1, 2},
        {2, 2}, {2, 1}, {2, 0}, {1, 0}
    };
    int_vector3 cube_sequence[8];
    //clock表示顺时针下标序列,cube是按旋转方向的一个方格下标序列
    SingleCube *temp_cube[8];
    int i, k;
    for (i = 0; i < 8; ++ i)
    {
        if (temp_direction * temp_face_color < 0)
            //temp_direction<0代表顺时针，temp_face_color<0代表左手系面
            k = i;
        else
            k = 7 - i;
        switch (temp_face_color)
        {
        case FC_YELLOW:
        case FC_WHITE:
            cube_sequence[i] = int_vector3(clock_sequence[k][0], clock_sequence[k][1],
                    (temp_face_color == FC_YELLOW ? 2 : 0));
            break;
        case FC_BLUE:
        case FC_GREEN:
            cube_sequence[i] = int_vector3((temp_face_color == FC_BLUE ? 2 : 0),
                                           clock_sequence[k][0], clock_sequence[k][1]);
            break;
        case FC_ORANGE:
        case FC_RED:
            cube_sequence[i] = int_vector3(clock_sequence[k][0],
                    (temp_face_color == FC_RED ? 2 : 0), clock_sequence[k][1]);
            break;
        }
    }
    //将方格下标按顺序排成一列
    for (i = 0; i < 8; ++ i)
        temp_cube[i] =
                cube_pointer_by_position[cube_sequence[i].x][cube_sequence[i].y][cube_sequence[i].z];
    for (i = 0; i < 8; ++ i)
    {
        int_vector3 temp_vector = cube_sequence[(i + 2) & 7];
        temp_cube[i] ->center_position = easy_vector3(temp_vector);
        temp_cube[i] ->rotate = Rotation();
        cube_pointer_by_position[temp_vector.x][temp_vector.y][temp_vector.z] = temp_cube[i];
        //接下来做颜色修正
        easy_vector3 temp_color;
        switch (temp_direction * temp_face_color)
        {
        case FC_YELLOW:
            temp_color = temp_cube[i] ->colors.down;
            temp_cube[i] ->colors.down = temp_cube[i] ->colors.left;
            temp_cube[i] ->colors.left = temp_cube[i] ->colors.up;
            temp_cube[i] ->colors.up = temp_cube[i] ->colors.right;
            temp_cube[i] ->colors.right = temp_color;
            break;
        case FC_WHITE:
            temp_color = temp_cube[i] ->colors.down;
            temp_cube[i] ->colors.down = temp_cube[i] ->colors.right;
            temp_cube[i] ->colors.right = temp_cube[i] ->colors.up;
            temp_cube[i] ->colors.up = temp_cube[i] ->colors.left;
            temp_cube[i] ->colors.left = temp_color;
            break;
        case FC_RED:
            temp_color = temp_cube[i] ->colors.front;
            temp_cube[i] ->colors.front = temp_cube[i] ->colors.left;
            temp_cube[i] ->colors.left = temp_cube[i] ->colors.back;
            temp_cube[i] ->colors.back = temp_cube[i] ->colors.right;
            temp_cube[i] ->colors.right = temp_color;
            break;
        case FC_ORANGE:
            temp_color = temp_cube[i] ->colors.front;
            temp_cube[i] ->colors.front = temp_cube[i] ->colors.right;
            temp_cube[i] ->colors.right = temp_cube[i] ->colors.back;
            temp_cube[i] ->colors.back = temp_cube[i] ->colors.left;
            temp_cube[i] ->colors.left = temp_color;
            break;
        case FC_GREEN:
            temp_color = temp_cube[i] ->colors.front;
            temp_cube[i] ->colors.front = temp_cube[i] ->colors.down;
            temp_cube[i] ->colors.down = temp_cube[i] ->colors.back;
            temp_cube[i] ->colors.back = temp_cube[i] ->colors.up;
            temp_cube[i] ->colors.up = temp_color;
            break;
        case FC_BLUE:
            temp_color = temp_cube[i] ->colors.front;
            temp_cube[i] ->colors.front = temp_cube[i] ->colors.up;
            temp_cube[i] ->colors.up = temp_cube[i] ->colors.back;
            temp_cube[i] ->colors.back = temp_cube[i] ->colors.down;
            temp_cube[i] ->colors.down = temp_color;
            break;
        }
    }
}

void MagicCubeGlWidget::Update_ColorSet()
{
    Quaternion rotate_transform_quaternion(*environment_rotate);
    easy_vector3 face_position[6];
    int face_color[6] = {FC_YELLOW, FC_WHITE, FC_ORANGE, FC_RED, FC_GREEN, FC_BLUE};
    //face_position[i]的位置代表face_color[i]颜色的中心块所在位置
    easy_vector3 original_position[6] = {
        easy_vector3(0.0f, 0.0f, 2.0f), easy_vector3(0.0f, 0.0f, -2.0f),
        easy_vector3(0.0f, -2.0f, 0.0f), easy_vector3(0.0f, 2.0f, 0.0f),
        easy_vector3(-2.0f, 0.0f, 0.0f), easy_vector3(2.0f, 0.0f, 0.0f)
    };
    int i;
    for (i = 0; i < 6; ++ i)
        face_position[i] = rotate_transform_quaternion * original_position[i];
    GLfloat temp_position;
    int temp_position_position;
    bool face_used[6];
    memset(face_used, false, sizeof(face_used));
    //以下一一寻找六个面的值
    for (temp_position = -100.0f, temp_position_position = 0, i = 0; i < 6; ++ i)
        if (!face_used[i] && face_position[i].z > temp_position)
        {
            temp_position = face_position[i].z;
            temp_position_position = i;
        }
    face_used[temp_position_position] = true;
    color_set ->front = face_color[temp_position_position];

    for (temp_position = 100.0f, temp_position_position = 0, i = 0; i < 6; ++ i)
        if (!face_used[i] && face_position[i].z < temp_position)
        {
            temp_position = face_position[i].z;
            temp_position_position = i;
        }
    face_used[temp_position_position] = true;
    color_set ->back = face_color[temp_position_position];

    for (temp_position = -100.0f, temp_position_position = 0, i = 0; i < 6; ++ i)
        if (!face_used[i] && face_position[i].x > temp_position)
        {
            temp_position = face_position[i].x;
            temp_position_position = i;
        }
    face_used[temp_position_position] = true;
    color_set ->right = face_color[temp_position_position];

    for (temp_position = 100.0f, temp_position_position = 0, i = 0; i < 6; ++ i)
        if (!face_used[i] && face_position[i].x < temp_position)
        {
            temp_position = face_position[i].x;
            temp_position_position = i;
        }
    face_used[temp_position_position] = true;
    color_set ->left = face_color[temp_position_position];

    for (temp_position = -100.0f, temp_position_position = 0, i = 0; i < 6; ++ i)
        if (!face_used[i] && face_position[i].y > temp_position)
        {
            temp_position = face_position[i].y;
            temp_position_position = i;
        }
    face_used[temp_position_position] = true;
    color_set ->up = face_color[temp_position_position];

    for (temp_position = 100.0f, temp_position_position = 0, i = 0; i < 6; ++ i)
        if (!face_used[i] && face_position[i].y < temp_position)
        {
            temp_position = face_position[i].y;
            temp_position_position = i;
        }
    face_used[temp_position_position] = true;
    color_set ->down = face_color[temp_position_position];
}

void MagicCubeGlWidget::SelectByColor(int face_color)
{
    int i, j;
    switch (face_color)
    {
    case FC_YELLOW:
        for (i = 0; i < 3; ++ i)
            for (j = 0; j < 3; ++ j)
                cube_pointer_by_position[i][j][2] ->selected = true;
        break;
    case FC_WHITE:
        for (i = 0; i < 3; ++ i)
            for (j = 0; j < 3; ++ j)
                cube_pointer_by_position[i][j][0] ->selected = true;
        break;
    case FC_BLUE:
        for (i = 0; i < 3; ++ i)
            for (j = 0; j < 3; ++ j)
                cube_pointer_by_position[2][i][j] ->selected = true;
        break;
    case FC_GREEN:
        for (i = 0; i < 3; ++ i)
            for (j = 0; j < 3; ++ j)
                cube_pointer_by_position[0][i][j] ->selected = true;
        break;
    case FC_RED:
        for (i = 0; i < 3; ++ i)
            for (j = 0; j < 3; ++ j)
                cube_pointer_by_position[i][2][j] ->selected = true;
        break;
    case FC_ORANGE:
        for (i = 0; i < 3; ++ i)
            for (j = 0; j < 3; ++ j)
                cube_pointer_by_position[i][0][j] ->selected = true;
        break;
    default:
        return;
    }
    selected_face_color = face_color;
    updateGL();
}

void MagicCubeGlWidget::ResetSelect()
{
    if (selected_face_color == FC_BLACK)
        return;
    int i, j, k;
    for (i = 0; i < 3; ++ i)
        for (j = 0; j < 3; ++ j)
            for (k = 0; k < 3; ++ k)
                cube[i][j][k] ->selected = false;
    selected_face_color = FC_BLACK;
    selected_face_position = POS_NONE;
}

int MagicCubeGlWidget::FindPositionByColor(int face_color)
{
    if (color_set ->front == face_color) return POS_FRONT;
    if (color_set ->back  == face_color) return POS_BACK;
    if (color_set ->left  == face_color) return POS_LEFT;
    if (color_set ->right == face_color) return POS_RIGHT;
    if (color_set ->up    == face_color) return POS_UP;
    if (color_set ->down  == face_color) return POS_DOWN;
    return POS_NONE;
}

int MagicCubeGlWidget::changecolor_Czk_to_Wyf(easy_vector3 colorForChange)
{
    if (colorForChange == cube_pointer_by_position[1][1][2]->colors.front) return YELLOW;
    if (colorForChange == cube_pointer_by_position[1][1][0] ->colors.back)  return WHITE;
    if (colorForChange == cube_pointer_by_position[0][1][1] ->colors.left)  return GREEN;
    if (colorForChange == cube_pointer_by_position[1][0][1] ->colors.down) return ORANGE;
    if (colorForChange == cube_pointer_by_position[1][2][1]   ->colors.up)    return RED;
    if (colorForChange == cube_pointer_by_position[2][1][1]->colors.right)   return BLUE;
    return BLACK;
}

#ifdef TEST
QString getColorName(int color)
{
    if (color == YELLOW) return QString("Yellow");
    if (color == WHITE) return QString("White");
    if (color == GREEN) return QString("Green");
    if (color == ORANGE) return QString("Orange");
    if (color == RED) return QString("Red");
    if (color == BLUE) return QString("Blue");
    return QString("Black");
}

#endif

easy_vector3 changecolor_Char_to_Czk(char c)
{
    if (c == 'R')   return MCC_RED;
    if (c == 'W')   return MCC_WHITE;
    if (c == 'Y')   return MCC_YELLOW;
    if (c == 'B')   return MCC_BLUE;
    if (c == 'O')   return MCC_ORANGE;
    if (c == 'G')   return MCC_GREEN;
    return MCC_BLACK;
}

void MagicCubeGlWidget::getMagicCubeForSolve()
{
    //传给MagicCubeForSolve数组
    ResetSelect();
    int i, j;
    for (i = 0; i < 3; ++ i)
        for (j = 0; j < 3; ++ j)
        {
            MagicCubeForSolve[YELLOW][i][j] =
                    changecolor_Czk_to_Wyf(cube_pointer_by_position[j][2-i][2]   ->colors.front);
            MagicCubeForSolve[WHITE][i][j]  =
                    changecolor_Czk_to_Wyf(cube_pointer_by_position[2-j][2-i][0] ->colors.back);
            MagicCubeForSolve[RED][i][j]    =
                    changecolor_Czk_to_Wyf(cube_pointer_by_position[j][2][i]     ->colors.up);
            MagicCubeForSolve[ORANGE][i][j] =
                    changecolor_Czk_to_Wyf(cube_pointer_by_position[j][0][2-i]   ->colors.down);
            MagicCubeForSolve[BLUE][i][j]   =
                    changecolor_Czk_to_Wyf(cube_pointer_by_position[2][2-i][2-j] ->colors.right);
            MagicCubeForSolve[GREEN][i][j]  =
                    changecolor_Czk_to_Wyf(cube_pointer_by_position[0][2-i][j]   ->colors.left);
        }
    Solver ->read(MagicCubeForSolve);
   // Solver ->cube.printVisualStatus();
#ifdef TEST
    return;
#endif
   // printf("\n");
    std::ofstream fout("answer.txt");
    std::vector<std::string> temp_method = Solver ->method();
    Solver ->method_for_Czk.clear();
    int temp_method_length = temp_method.size();
    for (i = 0; i < temp_method_length; ++ i)
    {
        int temp_color, temp_clock;
        temp_clock = (temp_method[i].size() == 2 ? DRT_COUNTERCLOCKWISE : DRT_CLOCKWISE);
        switch (temp_method[i][0])
        {
        case 'Y':   temp_color = S_YELLOW;  fout << 'F';break;
        case 'W':   temp_color = S_WHITE;   fout << 'B';break;
        case 'R':   temp_color = S_RED;     fout << 'U';break;
        case 'G':   temp_color = S_GREEN;   fout << 'L';break;
        case 'B':   temp_color = S_BLUE;    fout << 'R';break;
        case 'O':   temp_color = S_ORANGE;  fout << 'D';break;
        default:    temp_clock = S_BLACK;   fout << 'X';break;
        }
        if (temp_method[i].size() == 2) fout << 'i';
        if (i == temp_method_length - 1)
            fout << "\n";
       //  else
       //  fout << " ";
        Solver ->method_for_Czk.push_back(temp_color * temp_clock);
        //qDebug() << QString::fromStdString(temp_method[i]) << Solver ->method_for_Czk[i];
    }
    Flash_AutoSolve_Play();
}

void MagicCubeGlWidget::SetFlashSpeed(int speed)
{
    if (speed <= 100)
    {
        rotate_angle_once = (int)( 50.0 / sqrt((double)speed) );
        rotate_step_length = 3.0f;
    }
    else
    {
        rotate_angle_once = 5;
        int temp_angle = speed * 3 / 100;
        while (temp_angle > 1 && 90 % temp_angle != 0) -- temp_angle;
        rotate_step_length = (GLfloat) temp_angle;
        //有待改进的速度设置模式
    }
}

int changeColorFromSolveToFace(int solvecolor)
{
    switch (solvecolor)
    {
    case S_YELLOW:  return FC_YELLOW;
    case S_RED:     return FC_RED;
    case S_ORANGE:  return FC_ORANGE;
    case S_WHITE:   return FC_WHITE;
    case S_GREEN:   return FC_GREEN;
    case S_BLUE:    return FC_BLUE;
    default:        return FC_BLACK;
    }
}

void MagicCubeGlWidget::continue_solve(int now_position)
{
    if (! autosolve_on)
        return;
    if (now_position + 1 == (int)Solver ->method_for_Czk.size())
    {
        SetFlashSpeed(FlashSpeedWhenUserOperate);
        autosolve_on = false;
        key_operation_on = true;
        return;
    }
    passed_position = now_position + 1;
    int temp_color, temp_clock;
    int temp_operation = Solver ->method_for_Czk[passed_position];
    if (temp_operation > 0) temp_clock = 1, temp_color = temp_operation;
                    else    temp_clock =-1, temp_color =-temp_operation;
    temp_color = changeColorFromSolveToFace(temp_color);
    Rotate_Face_Start(temp_color, temp_clock);
}

void MagicCubeGlWidget::Flash_AutoSolve_Play()
{
    SetFlashSpeed(FlashSpeedWhenAutoSolve);
    passed_position = -1;
    autosolve_on = true;
    continue_solve(-1);
}

void MagicCubeGlWidget::ShowSpeedSet()
{
    SpeedSet = new SetFlashSpeedWidget(this);
    SpeedSet ->resize(400, 250);
    SpeedSet ->setMaximumSize(400, 250);
    SpeedSet ->setMinimumSize(400, 250);
    SpeedSet ->SetAutoSolveSpeed(FlashSpeedWhenAutoSolve);
    SpeedSet ->SetUserOperationSpeed(FlashSpeedWhenUserOperate);
    SpeedSet ->updatenumber();
    connect(SpeedSet, SIGNAL(returnSettings(int,int)), this, SLOT(UpdateSpeed(int,int)));
    SpeedSet ->exec();
}

void MagicCubeGlWidget::UpdateSpeed(int user_operation, int auto_solve)
{
    FlashSpeedWhenUserOperate = user_operation;
    FlashSpeedWhenAutoSolve   = auto_solve;
    if (autosolve_on)
        SetFlashSpeed(auto_solve);
    else
        SetFlashSpeed(user_operation);
}

void MagicCubeGlWidget::RandomOperate()
{
    SetFlashSpeed(1500);
    ResetSelect();
    autosolve_on = false;
    key_operation_on = false;
    srand(time(0));
    have_operated = MAX_RANDOM_OPERATION;
    RandomOperateOnce();
}

void MagicCubeGlWidget::RandomOperateOnce()
{
    -- have_operated;
    int o = rand() % 12;
    int dir = (o < 6 ? DRT_CLOCKWISE : DRT_COUNTERCLOCKWISE);
    int color;
    switch (o % 6)
    {
    case 0:color = FC_YELLOW;   break;
    case 1:color = FC_WHITE;    break;
    case 2:color = FC_GREEN;    break;
    case 3:color = FC_RED;      break;
    case 4:color = FC_ORANGE;   break;
    case 5:color = FC_BLUE;     break;
    }
    Rotate_Face_Start(color, dir);
}

void MagicCubeGlWidget::ReadFromFile()
{
    QString filepath = QFileDialog::getOpenFileName(this, tr("Open MagicCube File"),".");
    if (filepath.length() > 0)
    {
        std::ifstream fin;
        fin.open(filepath.toStdString().c_str());
        char color[6][3][3];
        char temp[100];
        //F,B,R,L,U,D
        int i, j, k;
        for (i = 0; i < 6; ++ i)
        {
            fin.getline(temp, 100);
            for (j = 0; j < 3; ++ j)
                for (k = 0; k < 3; ++ k)
                    color[i][j][k] = temp[j * 3 + k];
        }
        for (i = 0; i < 3; ++ i)
            for (j = 0; j < 3; ++ j)
            {
                cube_pointer_by_position[j][2 - i][2] ->colors.front = changecolor_Char_to_Czk(color[0][i][j]);
                cube_pointer_by_position[2 - j][2 - i][0] ->colors.back = changecolor_Char_to_Czk(color[1][i][j]);
                cube_pointer_by_position[2][2 - i][2 - j] ->colors.right = changecolor_Char_to_Czk(color[2][i][j]);
                cube_pointer_by_position[0][2 - i][j] ->colors.left = changecolor_Char_to_Czk(color[3][i][j]);
                cube_pointer_by_position[j][2][i] ->colors.up = changecolor_Char_to_Czk(color[4][i][j]);
                cube_pointer_by_position[j][0][2 - i] ->colors.down = changecolor_Char_to_Czk(color[5][i][j]);
            }
        if (cube_pointer_by_position[1][1][2] ->colors.front == MCC_WHITE)
            whiteCubePosition = cube_pointer_by_position[1][1][2];
        if (cube_pointer_by_position[1][1][0] ->colors.back == MCC_WHITE)
            whiteCubePosition = cube_pointer_by_position[1][1][0];
        if (cube_pointer_by_position[1][0][1] ->colors.down == MCC_WHITE)
            whiteCubePosition = cube_pointer_by_position[1][0][1];
        if (cube_pointer_by_position[1][2][1] ->colors.up == MCC_WHITE)
            whiteCubePosition = cube_pointer_by_position[1][2][1];
        if (cube_pointer_by_position[0][1][1] ->colors.left == MCC_WHITE)
            whiteCubePosition = cube_pointer_by_position[0][1][1];
        if (cube_pointer_by_position[2][1][1] ->colors.right == MCC_WHITE)
            whiteCubePosition = cube_pointer_by_position[2][1][1];
        fin.close();
        updateGL();
    }
}
