#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPropertyAnimation>
#include <QMainWindow>
#include <QMouseEvent>
#include <QGridLayout>
#include "calculator_mode.h"
#include "deposit_mode.h"
#include "credit_mode.h"
#include "qcplotic.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int windowW = 0;

public slots:
    void main_menu(int index);
    void open_up();
    void update_time();


private:
    Ui::MainWindow *ui;
    Calculator_mode *calc_widget;
    Credit_mode *credit_widget;
    Deposit_mode *deposit_widget;
    QPropertyAnimation *calc_anim;
    QPropertyAnimation *credit_anim;
    QPropertyAnimation *deposit_anim;
    QDateTime *date_time;
    QTimer *timer;
    int calcX = 0;
    int creditX = 0;
    int depositX = 0;
    QPoint zero;
    void animate_widget(QPropertyAnimation *widget, int *current, int target);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    int mouseOldX = 0;
    int mouseOldY = 0;
    int dX = 0;
    int dY = 0;
};

#endif // MAINWINDOW_H
