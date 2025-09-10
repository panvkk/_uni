#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

class MyClient : public QObject
{
    Q_OBJECT
public:
    MyClient(const QString& host, quint16 port, QObject* parent = nullptr);

    // Запуск соединения
    void start(const QString& message);

private slots:
    void onConnected();
    void onReadyRead();
    void onDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket* m_socket;
    QString       m_host;
    quint16       m_port;
    QString       m_message;
};

#endif // CLIENT_H
