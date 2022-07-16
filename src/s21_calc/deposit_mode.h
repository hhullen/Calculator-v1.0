#ifndef DEPOSIT_MODE_H
#define DEPOSIT_MODE_H

#include <QGridLayout>
#include <QComboBox>
#include <QDateEdit>
#include <QWidget>
#include "replenishment.h"
#include "withdrawal.h"

extern "C" {
    #include "../s21_calc_v1_0_header.h"
};

namespace Ui { class Deposit_mode; }

class Deposit_mode : public QWidget {
    Q_OBJECT

public:
    explicit Deposit_mode(QWidget *parent = nullptr);
    ~Deposit_mode();
    QWidget *deposit_mode = new QWidget(this);

private slots:
    void replenish_button();
    void withdrawal_button();
    void check_input_field();
    void get_input_data();
    void remove_widget(QObject *obj);

private:
    Ui::Deposit_mode *ui;
    QGridLayout *replenish_layout;
    QGridLayout *withdrawal_layout;
    Replenishment *new_replenish_form;
    Withdrawal *new_withdrawal_form;
    QRegularExpression valid_total;
    QRegularExpression valid_term;
    QRegularExpression valid_rate;
    QRegularExpressionMatch matcher_total;
    QRegularExpressionMatch matcher_term;
    QRegularExpressionMatch matcher_pec_rate;
    QRegularExpressionMatch matcher_tax_rate;

    deposit_t *deposit_data = NULL;
    deposit_act_t *deposit_action_data = NULL;
    date_t today;
    QWidget *tmp_widget;
    QList <QLineEdit*> form_amount;
    QList <QDateEdit*> form_term;
    QList <QComboBox*> form_switch;
    int yy = 0;
    int mm = 0;
    int dd = 0;
    double result_interest = 0;
    double result_tax = 0;
    double result_end_deposit = 0;

    void get_deposit_fields_data();
    void get_deposit_action_list(deposit_act_t **act_list);
    void get_replenish_form_data();
    void get_withdraw_form_data();
    void output_deposit_calculation_result();

    Replenishment *replenish;
};

#endif // DEPOSIT_MODE_H
