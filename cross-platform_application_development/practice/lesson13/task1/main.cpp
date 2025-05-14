#include <QApplication>
#include "photowidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    PhotoWidget widget;
    widget.show();

    return app.exec();
}
