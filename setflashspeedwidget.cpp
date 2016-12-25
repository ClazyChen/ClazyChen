#include "setflashspeedwidget.h"
#include <QScrollBar>
#include <QLabel>
#include <QPushButton>

SetFlashSpeedWidget::SetFlashSpeedWidget(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Set Flash Speed"));

    user_operation_speed_bar   = new QScrollBar(Qt::Horizontal, this);
    user_operation_speed_label = new QLabel(this);
    auto_solve_speed_bar       = new QScrollBar(Qt::Horizontal, this);
    auto_solve_speed_label     = new QLabel(this);

    user_operation_speed_bar   ->setRange(1, 1000);
    auto_solve_speed_bar       ->setRange(1, 1000);

    user_operation_speed_bar   ->resize(360, 40);
    user_operation_speed_bar   ->move(20, 55);
    user_operation_speed_bar   ->setStyleSheet("background-color:green");

    auto_solve_speed_bar       ->resize(360, 40);
    auto_solve_speed_bar       ->move(20, 150);
    auto_solve_speed_bar       ->setStyleSheet("background-color:red");

    user_operation_speed_label ->resize(360, 40);
    user_operation_speed_label ->move(20, 5);
    user_operation_speed_label ->setStyleSheet("color:green;font-size:20px;");

    auto_solve_speed_label     ->resize(360, 40);
    auto_solve_speed_label     ->move(20, 100);
    auto_solve_speed_label     ->setStyleSheet("color:red;font-size:20px;");

    button_ok = new QPushButton(this);
    button_cancel = new QPushButton(this);

    button_ok ->resize(120, 40);
    button_ok ->move(40, 200);
    button_ok ->setStyleSheet("font-size:30px;");
    button_ok ->setText(tr("OK"));

    button_cancel ->resize(120, 40);
    button_cancel ->move(240, 200);
    button_cancel ->setStyleSheet("font-size:30px;");
    button_cancel ->setText(tr("cancel"));

    connect(user_operation_speed_bar, SIGNAL(valueChanged(int)), this, SLOT(updatenumber()));
    connect(auto_solve_speed_bar, SIGNAL(valueChanged(int)), this, SLOT(updatenumber()));
    connect(button_ok, SIGNAL(clicked()), this, SLOT(OnOK()));
    connect(button_cancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
}

void SetFlashSpeedWidget::updatenumber()
{
    auto_solve_speed_label ->setText(tr("Auto solve speed: ")+QString::number(auto_solve_speed_bar ->value(), 10));
    user_operation_speed_label ->setText(tr("User operation speed: ")+QString::number(user_operation_speed_bar ->value(), 10));
}

void SetFlashSpeedWidget::SetAutoSolveSpeed(int to_set)
{
    auto_solve_speed_bar ->setValue(to_set);
}

void SetFlashSpeedWidget::SetUserOperationSpeed(int to_set)
{
    user_operation_speed_bar ->setValue(to_set);
}

void SetFlashSpeedWidget::OnOK()
{
    emit returnSettings(user_operation_speed_bar ->value(), auto_solve_speed_bar ->value());
    close();
}

void SetFlashSpeedWidget::OnCancel()
{
    close();
}
