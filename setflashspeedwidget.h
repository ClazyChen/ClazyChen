#ifndef SETFLASHSPEEDWIDGET_H
#define SETFLASHSPEEDWIDGET_H

#include <QDialog>

class QScrollBar;
class QLabel;
class QPushButton;

class SetFlashSpeedWidget : public QDialog
{
    Q_OBJECT
public:
    explicit SetFlashSpeedWidget(QWidget *parent = 0);
    void SetAutoSolveSpeed(int to_set);
    void SetUserOperationSpeed(int to_set);

signals:
    void returnSettings(int UserOperation, int AutoSolve);

public slots:
    void updatenumber();
    void OnOK();
    void OnCancel();

private:
    QScrollBar *user_operation_speed_bar;
    QScrollBar *auto_solve_speed_bar;
    QLabel *user_operation_speed_label;
    QLabel *auto_solve_speed_label;
    QPushButton *button_ok;
    QPushButton *button_cancel;

};

#endif // SETFLASHSPEEDWIDGET_H
