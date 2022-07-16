#include "qcplotic.h"
#include "ui_qcplotic.h"

QCPlotic::QCPlotic(QWidget *parent) : QMainWindow(parent), ui(new Ui::QCPlotic) {
    ui->setupUi(this);
    set_up_graph_style();

    ui->ScaleXField->setValidator(new QIntValidator);
    ui->ScaleYField->setValidator(new QIntValidator);

    ui->ScaleXField->setText("100");
    ui->ScaleYField->setText("100");

    connect(ui->ResetPushButton, SIGNAL(clicked()), this, SLOT(reset_axis_pos()));
    connect(ui->CalcPushButton, SIGNAL(clicked()), this, SLOT(show_calculator()));
    connect(ui->ScaleXField, SIGNAL(editingFinished()), this, SLOT(notice_graph_scale_changed()));
    connect(ui->ScaleYField, SIGNAL(editingFinished()), this, SLOT(notice_graph_scale_changed()));
}

QCPlotic::~QCPlotic() {
    delete ui;
    delete yellow_line;
    delete purple_line;
}

void QCPlotic::notice_graph_scale_changed() {
    ui->ResetPushButton->setStyleSheet("background-color: rgba(200, 40, 30, 0.7;)");
}

void QCPlotic::show_calculator() {
    emit open_calculator();
}

void QCPlotic::reset_axis_pos() {
    int buffX = ui->ScaleXField->text().toInt();
    int buffY = ui->ScaleYField->text().toInt();
    if (buffX < 0) buffX *= -1;
    if (buffY < 0) buffY *= -1;
    ui->GraphWidget->xAxis->setRange(-buffX, buffX);
    ui->GraphWidget->yAxis->setRange(-buffY, buffY);
    ui->GraphWidget->replot(QCustomPlot::rpQueuedRefresh);
    ui->ResetPushButton->setStyleSheet("background-color: rgba(93, 93, 94, 90);");
}

void QCPlotic::graph_data_processing(QString expr1, double *x_vals1, double *y_vals1, int points1,
                                                    QString expr2, double *x_vals2, double *y_vals2,
                                                                        int points2, int from, int to) {
    int buffX = ui->ScaleXField->text().toInt();
    int buffY = ui->ScaleYField->text().toInt();
    if (buffX < 0) buffX *= -1;
    if (buffY < 0) buffY *= -1;
    ui->GraphWidget->xAxis->setRange(-buffX, buffX);
    ui->GraphWidget->yAxis->setRange(-buffY, buffY);

    get_line1_data(expr1, x_vals1, y_vals1, points1);
    get_line2_data(expr2, x_vals2, y_vals2, points2);

    this->hide();
    this->show();
}

void QCPlotic::get_line1_data(QString expr1, double *x_vals1, double *y_vals1, int points1) {
    double angle = 0.0;

    if (graph1 != NULL) ui->GraphWidget->removeGraph(graph1);
    graph1 = NULL;
    x1.clear();
    y1.clear();
    ui->GraphWidget->replot(QCustomPlot::rpQueuedRefresh);
    ui->LegendExpr1->setText("-");
    if (x_vals1 != NULL && y_vals1 != NULL) {
        x1.push_back(x_vals1[0]);
        y1.push_back(y_vals1[0]);
        x1.push_back(x_vals1[1]);
        y1.push_back(y_vals1[1]);
        for (int i = 2; i < points1; i++) {
            angle = vectors_angle(x_vals1[i - 2], y_vals1[i - 2], x_vals1[i - 1],
                                                y_vals1[i - 1], x_vals1[i], y_vals1[i]);
            if (abs(angle) > 0.2) {
                x1.push_back(x_vals1[i]);
                y1.push_back(y_vals1[i]);
            } else {
                x1.push_back(x_vals1[i]);
                y1.push_back(line_gap);
            }
        }
        graph1 = ui->GraphWidget->addGraph();
        graph1->addData(x1, y1);
        graph1->setPen(*yellow_line);
        ui->GraphWidget->replot(QCustomPlot::rpQueuedRefresh);
        ui->LegendExpr1->setText(expr1);
    }
}

double QCPlotic::vectors_angle(double vx1, double vy1, double vx2, double vy2, double vx3, double vy3) {
    double vxy = 0.0, l1x= 0.0, l2x = 0.0, l1y = 0.0, l2y = 0.0, l1 = 0.0, l2 = 0.0;
    double returnable = 0;

    l1x = 0.0;
    l1y = 1000;
    l2x = vx3 - vx2;
    l2y = vy3 - vy2;
    vxy = l1x * l2x + l1y * l2y;
    l1 = sqrt((l1x * l1x) + (l1y * l1y));
    l2 = sqrt((l2x * l2x) + (l2y * l2y));
    returnable = acos(vxy / (l1 * l2)) * 180 / M_PI;

    return returnable;
}

void QCPlotic::get_line2_data(QString expr2, double *x_vals2, double *y_vals2, int points2) {
    double angle = 0.0;

    if (graph2 != NULL) ui->GraphWidget->removeGraph(graph2);
    graph2 = NULL;
    x2.clear();
    y2.clear();
    ui->GraphWidget->replot(QCustomPlot::rpQueuedRefresh);
    ui->LegendExpr2->setText("-");
    if (x_vals2 != NULL && y_vals2 != NULL) {
        x2.push_back(x_vals2[0]);
        y2.push_back(y_vals2[0]);
        x2.push_back(x_vals2[1]);
        y2.push_back(y_vals2[1]);
        for (int i = 2; i < points2; i++) {
            angle = vectors_angle(x_vals2[i - 2], y_vals2[i - 2], x_vals2[i - 1],
                                                y_vals2[i - 1], x_vals2[i], y_vals2[i]);
            if (abs(angle) > 0.2) {
                x2.push_back(x_vals2[i]);
                y2.push_back(y_vals2[i]);
            } else {
                x2.push_back(x_vals2[i]);
                y2.push_back(line_gap);
            }
        }
        graph2 = ui->GraphWidget->addGraph();
        graph2->addData(x2, y2);
        graph2->setPen(*purple_line);
        ui->GraphWidget->replot(QCustomPlot::rpQueuedRefresh);
        ui->LegendExpr2->setText(expr2);
    }
}
void QCPlotic::closeEvent(QCloseEvent *event) {
    emit graph_view_closed();
}

void QCPlotic::set_up_graph_style() {
    ui->GraphWidget->setInteraction(QCP::iRangeZoom, true);
    ui->GraphWidget->setInteraction(QCP::iRangeDrag, true);
    ui->GraphWidget->setBackground(QBrush(QColor(43, 82, 165, 255)));
    ui->GraphWidget->xAxis->grid()->setPen (QPen(QColor(255, 255, 255), 1, Qt::SolidLine));
    ui->GraphWidget->yAxis->grid()->setPen (QPen(QColor(255, 255, 255), 1, Qt::SolidLine));
    ui->GraphWidget->xAxis->grid()->setSubGridPen(QPen(QColor(255, 255, 255), 0.5, Qt :: DotLine));
    ui->GraphWidget->yAxis->grid()->setSubGridPen(QPen(QColor(255, 255, 255), 0.5, Qt::DotLine));
    ui->GraphWidget->xAxis->grid()->setSubGridVisible (true);
    ui->GraphWidget->yAxis->grid()->setSubGridVisible (true);
    ui->GraphWidget->xAxis->grid()->setZeroLinePen(QPen(QColor(200, 0, 0)));
    ui->GraphWidget->yAxis->grid()->setZeroLinePen(QPen(QColor(200, 0, 0)));
    ui->GraphWidget->xAxis->setBasePen(QPen(Qt::white, 1));
    ui->GraphWidget->yAxis->setBasePen(QPen(Qt::white, 1));
    ui->GraphWidget->xAxis->setTickPen(QPen(Qt::white, 1));
    ui->GraphWidget->yAxis->setTickPen(QPen(Qt::white, 1));
    ui->GraphWidget->xAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->GraphWidget->yAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->GraphWidget->xAxis->setTickLabelColor(Qt::white);
    ui->GraphWidget->yAxis->setTickLabelColor(Qt::white);
    ui->GraphWidget->xAxis->setLabel ("X axis");
    ui->GraphWidget->yAxis->setLabel ("Y axis");
    ui->GraphWidget->xAxis->setLabelColor(Qt::white);
    ui->GraphWidget->yAxis->setLabelColor(Qt::white);
    ui->GraphWidget->xAxis->pixelOrientation();
    yellow_line = new QPen();
    yellow_line->setColor(QColor(253, 242, 2));
    yellow_line->setWidth(5);
    yellow_line->setStyle(Qt::SolidLine);
    purple_line = new QPen();
    purple_line->setColor(QColor(255, 82, 172));
    purple_line->setWidth(5);
    purple_line->setStyle(Qt::SolidLine);
}

