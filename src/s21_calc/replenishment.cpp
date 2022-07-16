#include "replenishment.h"
#include "ui_replenishment.h"

Replenishment::Replenishment(QWidget *parent) : QWidget(parent), ui(new Ui::Replenishment) {
    ui->setupUi(this);
    valid.setPattern("(^[0-9]{0,1}$)|(\\.[0-9]{1,2}$)|(^[0-9]*$)|(^[0-9]+\\.$)");
    connect(ui->BTNclose_form, SIGNAL(clicked()), this, SLOT(close_form()));
    connect(ui->DepositAmount, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(check_input()));
}

Replenishment::~Replenishment() {
    delete ui;
}

void Replenishment::close_form() {
    emit delete_button(sender()->parent());
}

void Replenishment::check_input() {
    matcher_valid = valid.match(ui->DepositAmount->text());
    if (!matcher_valid.hasMatch()) {
        ui->DepositAmount->setText(ui->DepositAmount->
            text().left(ui->DepositAmount->text().length() - 1));
    }
}
