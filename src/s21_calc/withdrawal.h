#ifndef WITHDRAWAL_H
#define WITHDRAWAL_H

#include <QWidget>
#include <QRegularExpression>

namespace Ui { class Withdrawal; }

class Withdrawal : public QWidget {
    Q_OBJECT

signals:
    void delete_button(QObject *);

public:
    explicit Withdrawal(QWidget *parent = nullptr);
    ~Withdrawal();

private slots:
    void close_form();
    void check_input();

private:
    Ui::Withdrawal *ui;
    QRegularExpression valid;
    QRegularExpressionMatch matcher_valid;
};

#endif // WITHDRAWAL_H
