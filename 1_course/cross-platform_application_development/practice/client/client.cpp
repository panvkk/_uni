#include "client.h"
#include <QDebug>
#include <QCoreApplication>

MyClient::MyClient(const QString& host, quint16 port, QObject* parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
    , m_host(host)
    , m_port(port)
{
    connect(m_socket, &QTcpSocket::connected,     this, &MyClient::onConnected);
    connect(m_socket, &QTcpSocket::readyRead,     this, &MyClient::onReadyRead);
    connect(m_socket, &QTcpSocket::disconnected,  this, &MyClient::onDisconnected);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &MyClient::onErrorOccurred);
}

void MyClient::start(const QString& message)
{
    m_message = message;
    qDebug() << "Connecting to" << m_host << ":" << m_port;
    m_socket->connectToHost(m_host, m_port);
}

void MyClient::onConnected()
{
    qDebug() << "Connected to server. Sending message:";
    qDebug() << "   " << m_message;
    // Отправляем и ждём ответ
    m_socket->write(m_message.toUtf8() + '\n');
}

void MyClient::onReadyRead()
{
    QByteArray data = m_socket->readAll().trimmed();
    qDebug() << "Received from server:" << data;
    // После получения закрываем
    m_socket->disconnectFromHost();
}

void MyClient::onDisconnected()
{
    qDebug() << "Disconnected.";
    QCoreApplication::quit();
}

void MyClient::onErrorOccurred(QAbstractSocket::SocketError err)
{
    Q_UNUSED(err);
    qWarning() << "Socket error:" << m_socket->errorString();
    QCoreApplication::quit();
}
