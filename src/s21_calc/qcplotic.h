#ifndef QCPLOTIC_H
#define QCPLOTIC_H

#include "QCustomPlot-library-master/lib/qcustomplot.h"
#include <QMainWindow>
#include <QVector>

#define RESCALE QChar(0x21BA);

namespace Ui {
class QCPlotic;
}

class QCPlotic : public QMainWindow {
    Q_OBJECT

public:
    explicit QCPlotic(QWidget *parent = nullptr);
    ~QCPlotic();

private slots:
    void graph_data_processing(QString expr1, double *x_vals1, double *y_vals1, int points1,
                                    QString expr2, double *x_vals2, double *y_vals2, int points2,
                                                                                    int from, int to);
    void closeEvent(QCloseEvent *event);
    void reset_axis_pos();
    void show_calculator();
    void notice_graph_scale_changed();

signals:
    void graph_view_closed();
    void open_calculator();

private:
    Ui::QCPlotic *ui;
    QCPGraph *graph1;
    QCPGraph *graph2;
    QVector<double> x1,y1;
    QVector<double> x2,y2;
    QPen *yellow_line;
    QPen *purple_line;
    double line_gap = NAN;
    void set_up_graph_style();
    void get_line1_data(QString expr1, double *x_vals1, double *y_vals1, int points1);
    void get_line2_data(QString expr2, double *x_vals2, double *y_vals2, int points2);
    double vectors_angle(double vx1, double vy1, double vx2, double vy2, double vx3, double vy3);
};

#endif // QCPLOTIC_H
