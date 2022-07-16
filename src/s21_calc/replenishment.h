#ifndef REPLENISHMENT_H
#define REPLENISHMENT_H

#include <QWidget>
#include <QRegularExpression>

namespace Ui { class Replenishment; }

class Replenishment : public QWidget {
    Q_OBJECT

signals:
    void delete_button(QObject *);

public:
    explicit Replenishment(QWidget *parent = nullptr);
    ~Replenishment();

private slots:
    void close_form();
    void check_input();

private:
    Ui::Replenishment *ui;
    QRegularExpression valid;
    QRegularExpressionMatch matcher_valid;
};

#endif // REPLENISHMENT_H
