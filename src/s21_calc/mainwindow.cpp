#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    calc_widget = new Calculator_mode();
    credit_widget = new Credit_mode();
    deposit_widget = new Deposit_mode();
    ui->layout->addWidget(calc_widget->calculator);
    ui->layout->addWidget(credit_widget->credit_mode);
    ui->layout->addWidget(deposit_widget->deposit_mode);

    zero.setX(0);
    zero.setY(0);
    calc_anim = new QPropertyAnimation(calc_widget->calculator, "geometry");
    credit_anim = new QPropertyAnimation(credit_widget->credit_mode, "geometry");
    deposit_anim = new QPropertyAnimation(deposit_widget->deposit_mode, "geometry");
    calc_anim->setDuration(100);
    credit_anim->setDuration(100);
    deposit_anim->setDuration(100);

    date_time = new QDateTime();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_time()));
    timer->start(1000);
    update_time();

    connect(ui->Main_menu, SIGNAL(activated(int)), this, SLOT(main_menu(int)));
    connect(calc_widget, SIGNAL(open_the_window()), this, SLOT(open_up()));
}

MainWindow::~MainWindow() {
    delete ui;
    delete calc_widget;
    delete credit_widget;
    delete deposit_widget;
    delete calc_anim;
    delete credit_anim;
    delete deposit_anim;
}

void MainWindow::open_up() {
    this->hide();
    this->show();
    this->setVisible(true);
}

void MainWindow::update_time() {
    ui->TimeDateNow->setText(date_time->currentDateTime().toString("yyyy MMMM dd hh:mm"));
}

// modes menu
void MainWindow::main_menu(int index) {
    if (index == 0) {
        if (calc_widget->calculator->pos() != zero) {
            calcX = windowW;
            animate_widget(calc_anim, &calcX, 0);
        }
        if(credit_widget->credit_mode->pos() == zero)
            animate_widget(credit_anim, &creditX, -windowW);
        if (deposit_widget->deposit_mode->pos() == zero)
            animate_widget(deposit_anim, &depositX, -windowW);
    } else if (index == 1) {
        if (credit_widget->credit_mode->pos() != zero) {
            creditX = windowW;
            animate_widget(credit_anim, &creditX, 0);
        }
        if (calc_widget->calculator->pos() == zero)
            animate_widget(calc_anim, &calcX, -windowW);
        if (deposit_widget->deposit_mode->pos() == zero)
            animate_widget(deposit_anim, &depositX, -windowW);
    } else if (index == 2) {
        if (deposit_widget->deposit_mode->pos() != zero) {
            depositX = windowW;
            animate_widget(deposit_anim, &depositX, 0);
        }
        if (calc_widget->calculator->pos() == zero)
            animate_widget(calc_anim, &calcX, -windowW);
        if(credit_widget->credit_mode->pos() == zero)
            animate_widget(credit_anim, &creditX, -windowW);
    }
}

void MainWindow::animate_widget(QPropertyAnimation *widget, int *current, int target) {
    widget->setStartValue(QRect(*current, 0, 0, 0));
    widget->setEndValue(QRect(target, 0, 0, 0));
    widget->start();
    *current = target;
}
