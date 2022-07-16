#include "credit_mode.h"
#include "ui_credit_mode.h"

Credit_mode::Credit_mode(QWidget *parent) : QWidget(parent), ui(new Ui::Credit_mode) {
    ui->setupUi(this);
    credit_mode = ui->QW_Credit;
    valid_total.setPattern("(^[0-9]{0,1}$)|(\\.[0-9]{1,2}$)|(^[0-9]*$)|(^[0-9]+\\.$)");
    valid_term.setPattern("(^[0-9]{0,4}$)");
    valid_rate.setPattern("(^[0-9]{1,2}$)|(^[0-9]+\\.[0-9]{1,2}$)|(^[0-9]{1,2}$)|(^[0-9]{1,2}\\.$)");

    connect(ui->FieldCreditTotal, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(check_input_field()));
    connect(ui->FieldCreditTerm, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(check_input_field()));
    connect(ui->FieldInterestRate, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(check_input_field()));
    connect(ui->BTNcalculate_it, SIGNAL(clicked()), this, SLOT(get_input_data()));
}

Credit_mode::~Credit_mode() {
    delete ui;
}

void Credit_mode::check_input_field() {
    matcher_total = valid_total.match(ui->FieldCreditTotal->text());
    matcher_term = valid_term.match(ui->FieldCreditTerm->text());
    matcher_rate = valid_rate.match(ui->FieldInterestRate->text());

    if (!matcher_total.hasMatch())
        ui->FieldCreditTotal->setText(ui->FieldCreditTotal->
                text().left(ui->FieldCreditTotal->text().length() - 1));

    if (!matcher_term.hasMatch())
        ui->FieldCreditTerm->setText(ui->FieldCreditTerm->
                text().left(ui->FieldCreditTerm->text().length() - 1));

    if (!matcher_rate.hasMatch())
        ui->FieldInterestRate->setText(ui->FieldInterestRate->
                text().left(ui->FieldInterestRate->text().length() - 1));
}

void Credit_mode::get_input_data() {
    credit_total = ui->FieldCreditTotal->text().toDouble();
    credit_term = ui->FieldCreditTerm->text().toDouble();
    interest_rate = ui->FieldInterestRate->text().toDouble();

    if (ui->MenuTerm->currentIndex() == 1) credit_term *= 12;

    if (ui->MenuPayType->currentIndex() == 0) {
        calculate_credit(credit_total, credit_term, interest_rate, &mon_pay_high,
                                            &mon_pay_low, &overpayment, &total_payment, ANNUITY);
        write_values_to_fields(mon_pay_high, mon_pay_low, overpayment, total_payment, ANNUITY);
    } else if (ui->MenuPayType->currentIndex() == 1) {
        calculate_credit(credit_total, credit_term, interest_rate, &mon_pay_high,
                                            &mon_pay_low, &overpayment, &total_payment, DIFFERENTIATED);
        write_values_to_fields(mon_pay_high, mon_pay_low, overpayment, total_payment, DIFFERENTIATED);
    }
}

void Credit_mode::write_values_to_fields(double mon_pay_high, double mon_pay_low,
                                    double overpayment, double total_payment, int mode) {
    if (mon_pay_high != mon_pay_high) mon_pay_high = 0;
    if (mon_pay_low != mon_pay_low) mon_pay_low = 0;
    if (overpayment != overpayment) overpayment = 0;
    if (total_payment != total_payment) total_payment = 0;

    if (mode == ANNUITY) {
        ui->FieldResultMonthlyPayment->setText(QString::number(mon_pay_high, 'f', 2));
        ui->FieldResultOverpayment->setText(QString::number(overpayment, 'f', 2));
        ui->FieldResultTotalPayment->setText(QString::number(total_payment, 'f', 2));
    } else if (mode == DIFFERENTIATED) {
        ui->FieldResultMonthlyPayment->setText(QString::number(mon_pay_high, 'f', 2));
        ui->FieldResultMonthlyPayment->setText(ui->FieldResultMonthlyPayment->text().append("..."));
        ui->FieldResultMonthlyPayment->setText(ui->FieldResultMonthlyPayment->
                            text().append(QString::number(mon_pay_low, 'f', 2)));
        ui->FieldResultOverpayment->setText(QString::number(overpayment, 'f', 2));
        ui->FieldResultTotalPayment->setText(QString::number(total_payment, 'f', 2));
    }
}
