#include <QCoreApplication>
#include "client.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    if (argc < 4) {
        qWarning() << "Usage:" << argv[0] << "<host> <port> \"<message>\"";
        return 1;
    }

    QString host    = argv[1];
    quint16 port    = QString(argv[2]).toUShort();
    QString message = argv[3];

    MyClient client(host, port);
    client.start(message);

    return app.exec();
}
