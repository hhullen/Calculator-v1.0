#include "withdrawal.h"
#include "ui_withdrawal.h"

Withdrawal::Withdrawal(QWidget *parent) : QWidget(parent), ui(new Ui::Withdrawal) {
    ui->setupUi(this);
    valid.setPattern("(^[0-9]{0,1}$)|(\\.[0-9]{1,2}$)|(^[0-9]*$)|(^[0-9]+\\.$)");
    connect(ui->BTNclose_form, SIGNAL(clicked()), this, SLOT(close_form()));
    connect(ui->SeizureAmount, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(check_input()));
}

Withdrawal::~Withdrawal() {
    delete ui;
}

void Withdrawal::close_form() {
    emit delete_button(sender()->parent());
}

void Withdrawal::check_input() {
    matcher_valid = valid.match(ui->SeizureAmount->text());
    if (!matcher_valid.hasMatch()) {
        ui->SeizureAmount->setText(ui->SeizureAmount->
            text().left(ui->SeizureAmount->text().length() - 1));
    }
}
