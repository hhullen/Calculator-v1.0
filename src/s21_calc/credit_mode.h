#ifndef CREDIT_MODE_H
#define CREDIT_MODE_H

#include <QRegularExpression>
#include <QWidget>

extern "C" {
    #include "../s21_calc_v1_0_header.h"
};

namespace Ui {
class Credit_mode;
}

class Credit_mode : public QWidget {
    Q_OBJECT

public:
    explicit Credit_mode(QWidget *parent = nullptr);
    ~Credit_mode();
     QWidget *credit_mode = new QWidget(this);

private slots:
     void check_input_field();
      void get_input_data();

private:
    Ui::Credit_mode *ui;
    void write_values_to_fields(double mon_pay_high, double mon_pay_low,
                                    double overpayment, double total_payment, int mode);

    QRegularExpression valid_total;
    QRegularExpression valid_term;
    QRegularExpression valid_rate;
    QRegularExpressionMatch matcher_total;
    QRegularExpressionMatch matcher_term;
    QRegularExpressionMatch matcher_rate;
    double credit_total = 0;
    double credit_term = 0;
    double interest_rate = 0;
    double mon_pay_high = 0;
    double mon_pay_low = 0;
    double overpayment = 0;
    double total_payment = 0;
    QString buffer;
};

#endif // CREDIT_MODE_H
