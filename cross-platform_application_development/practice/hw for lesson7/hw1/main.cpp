#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.setWindowTitle("Преобразователь даты");
    w.resize(500, 300);
    w.show();
    return app.exec();
}
