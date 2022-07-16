#include "deposit_mode.h"
#include "ui_deposit_mode.h"

Deposit_mode::Deposit_mode(QWidget *parent) : QWidget(parent), ui(new Ui::Deposit_mode) {
    ui->setupUi(this);
    deposit_mode = ui->QW_Deposit;
    replenish = new Replenishment();

    valid_total.setPattern("(^[0-9]{0,1}$)|(\\.[0-9]{1,2}$)|(^[0-9]*$)|(^[0-9]+\\.$)");
    valid_term.setPattern("(^[0-9]{0,4}$)");
    valid_rate.setPattern("(^[0-9]{1,2}$)|(^[0-9]+\\.[0-9]{1,2}$)|(^[0-9]{1,2}$)|(^[0-9]{1,2}\\.$)");

    replenish_layout = new QGridLayout();
    replenish_layout->setAlignment(Qt::AlignTop);
    replenish_layout->setSpacing(0);
    ui->ReplenWidget->setLayout(replenish_layout);

    withdrawal_layout = new QGridLayout();
    withdrawal_layout->setAlignment(Qt::AlignTop);
    withdrawal_layout->setSpacing(0);
    ui->WithdrawWidget->setLayout(withdrawal_layout);

    connect(ui->BTNReplan, SIGNAL(clicked()), this, SLOT(replenish_button()));
    connect(ui->BTNWithdrawal, SIGNAL(clicked()), this, SLOT(withdrawal_button()));
    connect(ui->FieldDepositTotal, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(check_input_field()));
    connect(ui->FieldDepositTerm, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(check_input_field()));
    connect(ui->FieldInterestRate, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(check_input_field()));
    connect(ui->FieldTaxRate, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(check_input_field()));
    connect(ui->BTNcalculate_it, SIGNAL(clicked()), this, SLOT(get_input_data()));
}

Deposit_mode::~Deposit_mode() {
    delete ui;
}

void Deposit_mode::remove_widget(QObject *obj) {
     for (int i = 0; i < replenish_layout->count(); i++) {
        if (QString::pointer(obj) == QString::pointer(replenish_layout->itemAt(i)->widget())) {
            replenish_layout->itemAt(i)->widget()->hide();
            replenish_layout->removeWidget(replenish_layout->itemAt(i)->widget());
        }
    }
    for (int i = 0; i < withdrawal_layout->count(); i++) {
        if (QString::pointer(obj) == QString::pointer(withdrawal_layout->itemAt(i)->widget())) {
            withdrawal_layout->itemAt(i)->widget()->hide();
            withdrawal_layout->removeWidget(withdrawal_layout->itemAt(i)->widget());
        }
    }
}

void Deposit_mode::replenish_button() {
    if (replenish_layout->count() < 15) {
    new_replenish_form = new Replenishment();
    connect(new_replenish_form, SIGNAL(delete_button(QObject*)), this, SLOT(remove_widget(QObject*)));
    replenish_layout->addWidget(new_replenish_form);
    }
}

void Deposit_mode::withdrawal_button() {
    if (withdrawal_layout->count() < 15) {
    new_withdrawal_form = new Withdrawal();
    connect(new_withdrawal_form, SIGNAL(delete_button(QObject*)), this, SLOT(remove_widget(QObject*)));
    withdrawal_layout->addWidget(new_withdrawal_form);
    }
}

void Deposit_mode::check_input_field() {
    matcher_total = valid_total.match(ui->FieldDepositTotal->text());
    matcher_term = valid_term.match(ui->FieldDepositTerm->text());
    matcher_pec_rate = valid_rate.match(ui->FieldInterestRate->text());
    matcher_tax_rate = valid_rate.match(ui->FieldTaxRate->text());

    if (!matcher_total.hasMatch())
        ui->FieldDepositTotal->setText(ui->FieldDepositTotal->
                    text().left(ui->FieldDepositTotal->text().length() - 1));

    if (!matcher_term.hasMatch())
        ui->FieldDepositTerm->setText(ui->FieldDepositTerm->
                    text().left(ui->FieldDepositTerm->text().length() - 1));

    if (!matcher_pec_rate.hasMatch())
        ui->FieldInterestRate->setText(ui->FieldInterestRate->
                    text().left(ui->FieldInterestRate->text().length() - 1));

    if (!matcher_tax_rate.hasMatch())
        ui->FieldTaxRate->setText(ui->FieldTaxRate->
                    text().left(ui->FieldTaxRate->text().length() - 1));
}

void Deposit_mode::get_input_data() {
    deposit_data = (deposit_t *)malloc(sizeof(deposit_t));
    today.YY = QDate::currentDate().year();
    today.MM = QDate::currentDate().month();
    today.DD = QDate::currentDate().day();

    get_deposit_fields_data();
    get_deposit_action_list(&deposit_data->act_list);
    result_interest = 0.0;
    result_tax = 0.0;
    result_end_deposit = 0.0;
    if (deposit_data->deposit_term > 0) {
        calculate_deposit(&deposit_data, &deposit_action_data, today,
                            &result_interest, &result_tax, &result_end_deposit);
    }
    output_deposit_calculation_result();

    clear_deposit_action(&deposit_action_data);
    free(deposit_data);
}

void Deposit_mode::output_deposit_calculation_result() {
    QString temp_str;

    temp_str = QString::number(result_interest * 100, 'f', 2);
    temp_str.append("  %");
    ui->LabelResultAccuredInterestValue->setText(temp_str);
    temp_str = QString::number(result_tax,  'f', 2);
    temp_str.append("  RUB.");
    ui->LabelResultTaxAmountValue->setText(temp_str);
    temp_str = QString::number(result_end_deposit,  'f', 2);
    temp_str.append("  RUB.");
    ui->LabelResultEndDepositValue->setText(temp_str);
}

void Deposit_mode::get_deposit_fields_data() {
    deposit_data->deposit_total = ui->FieldDepositTotal->text().toDouble();
    deposit_data->deposit_term = ui->FieldDepositTerm->text().toInt();
    deposit_data->interest_rate = ui->FieldInterestRate->text().toDouble() / 100;
    deposit_data->tax_rate = ui->FieldTaxRate->text().toDouble() / 100;
    deposit_data->act_list = deposit_action_data;

    if (ui->MenuDeopsitTermUnits->currentIndex() == 1)
        deposit_data->deposit_term *= 12;

    if (ui->MenuPaymentsFrequency->currentIndex() == 0)
        deposit_data->payments_frequency = MONTHLY;
    else if (ui->MenuPaymentsFrequency->currentIndex() == 1)
        deposit_data->payments_frequency = QUARTERLY;
    else if (ui->MenuPaymentsFrequency->currentIndex() == 2)
        deposit_data->payments_frequency = ANNUALY;

    if (ui->checkInterestCapitalization->isChecked())
        deposit_data->interest_capitalization = INTEREST_CAPITALIZATION;
    else
        deposit_data->interest_capitalization = NONE_OF;
}

void Deposit_mode::get_deposit_action_list(deposit_act_t **act_list) {
    get_replenish_form_data();
    get_withdraw_form_data();
}

void Deposit_mode::get_replenish_form_data() {
    if (replenish_layout->count() > 0) {
        for (int i = 0; i < replenish_layout->count(); i++) {
            tmp_widget = replenish_layout->itemAt(i)->widget();
            form_amount = tmp_widget->findChildren<QLineEdit*>("DepositAmount");
            form_term = tmp_widget->findChildren<QDateEdit*>("ReplenishmentDateEdit");
            form_switch = tmp_widget->findChildren<QComboBox*>("PeriodicityMenu");
            form_term.at(0)->date().getDate(&yy, &mm, &dd);
            push_deposit_action(&deposit_action_data);
            deposit_action_data->act_type = REPLENISH;
            deposit_action_data->amount = form_amount.at(0)->text().toDouble();
            deposit_action_data->dd = dd;
            deposit_action_data->mm = mm;
            deposit_action_data->yy = yy;
            if (form_switch.at(0)->currentIndex() == 0) {
                deposit_action_data->periodicity = AT_ONCE;
            } else if (form_switch.at(0)->currentIndex() == 1) {
                deposit_action_data->periodicity = EVERY_MONTH;
            } else if (form_switch.at(0)->currentIndex() == 2) {
                deposit_action_data->periodicity = EVERY_YEAR;
            }

            form_amount.clear();
            form_term.clear();
            form_switch.clear();
        }
    }
}

void Deposit_mode::get_withdraw_form_data() {
    if (withdrawal_layout->count() > 0) {
        for (int i = 0; i < withdrawal_layout->count(); i++) {
            tmp_widget = withdrawal_layout->itemAt(i)->widget();
            form_amount = tmp_widget->findChildren<QLineEdit*>("SeizureAmount");
            form_term = tmp_widget->findChildren<QDateEdit*>("AeizureDateEdit");
            form_term.at(0)->date().getDate(&yy, &mm, &dd);
            push_deposit_action(&deposit_action_data);
            deposit_action_data->act_type = WITHDRAW;
            deposit_action_data->amount = form_amount.at(0)->text().toDouble();
            deposit_action_data->periodicity = NONE_OF;
            deposit_action_data->dd = dd;
            deposit_action_data->mm = mm;
            deposit_action_data->yy = yy;
            form_amount.clear();
            form_term.clear();
            form_switch.clear();
        }
    }
}
