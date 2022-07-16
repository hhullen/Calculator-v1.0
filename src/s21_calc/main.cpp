#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "s21_calc" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w;
    w.setWindowIcon(QIcon("CFE.icns"));
    w.setWindowFlag(Qt::FramelessWindowHint, Qt::WA_TranslucentBackground);
    w.show();
    w.windowW = w.size().width();
    return a.exec();
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    mouseOldX =  QCursor::pos().x();
    mouseOldY =  QCursor::pos().y();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
        dX = QCursor::pos().x() - mouseOldX;
        dY = QCursor::pos().y() - mouseOldY;
        MainWindow::move(MainWindow::pos().x() + dX, MainWindow::pos().y() + dY);
        mouseOldX =  QCursor::pos().x();
        mouseOldY =  QCursor::pos().y();
}
