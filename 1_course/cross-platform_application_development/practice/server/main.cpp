#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QDir>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QRegularExpression>
#include <QPixmap>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

class PhotoDisplay : public QLabel {
    Q_OBJECT
public:
    explicit PhotoDisplay(QWidget* parent = nullptr) : QLabel(parent) {
        setAlignment(Qt::AlignCenter);
        setMinimumSize(400, 400);
        setStyleSheet("background-color: black;");
    }

public slots:
    void showPhoto(const QString& surname) {
        QString path = QString("photos/%1.jpg").arg(surname);

        if(QFile::exists(path)) {
            QPixmap pixmap(path);
            if(!pixmap.isNull()) {
                setPixmap(pixmap.scaled(400, 400, Qt::KeepAspectRatio));
                show();
                QTimer::singleShot(5000, this, &PhotoDisplay::hidePhoto);
            } else {
                setText("Invalid image format");
            }
        } else {
            setText(QString("Photo not found: %1").arg(surname));
        }
    }

private slots:
    void hidePhoto() {
        clear();
        setText("Waiting for students...");
    }
};

class ClientHandler : public QObject {
    Q_OBJECT
public:
    explicit ClientHandler(qintptr socketDescriptor, QObject* parent = nullptr)
        : QObject(parent), socketDescriptor(socketDescriptor) {}

public slots:
    void process() {
        QTcpSocket socket;
        if(!socket.setSocketDescriptor(socketDescriptor)) {
            emit error(socket.error());
            return;
        }

        // Wait for data
        if(socket.waitForReadyRead(3000)) {
            QString message = QString::fromUtf8(socket.readAll()).trimmed();

            // Check message format
            QRegularExpression regex("Hello, Garson, I'm ([A-Za-z]+)!");
            QRegularExpressionMatch match = regex.match(message);

            QString response;
            if(match.hasMatch()) {
                QString surname = match.captured(1);
                response = "I'm not Garson, I'm Server! Go To Sleep To the Garden!";
                emit validRequest(surname);
            } else {
                response = "ERROR: Invalid message format! Use: 'Hello, Garson, I'm [Surname]!'";
            }

            // Send response
            socket.write(response.toUtf8());
            socket.waitForBytesWritten(1000);
        }
        socket.disconnectFromHost();
        deleteLater();
    }

signals:
    void validRequest(const QString& surname);
    void error(QAbstractSocket::SocketError socketError);

private:
    qintptr socketDescriptor;
};

class Server : public QTcpServer {
    Q_OBJECT
public:
    explicit Server(QObject* parent = nullptr) : QTcpServer(parent) {}

signals:
    void validRequest(const QString& surname);

protected:
    void incomingConnection(qintptr socketDescriptor) override {
        QThread* thread = new QThread;
        ClientHandler* handler = new ClientHandler(socketDescriptor);
        handler->moveToThread(thread);

        connect(thread, &QThread::started, handler, &ClientHandler::process);
        connect(handler, &ClientHandler::validRequest, this, &Server::validRequest);
        connect(handler, &ClientHandler::error, this, [](QAbstractSocket::SocketError){});
        connect(handler, &ClientHandler::finished, thread, &QThread::quit);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);

        thread->start();
    }
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr) : QMainWindow(parent) {
        setupUI();
        startServer();
    }

private:
    void setupUI() {
        setWindowTitle("Group 7 Server");
        resize(800, 600);

        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

        // Status label
        statusLabel = new QLabel(this);
        statusLabel->setAlignment(Qt::AlignCenter);
        statusLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
        mainLayout->addWidget(statusLabel);

        // Photo display
        display = new PhotoDisplay(this);
        mainLayout->addWidget(display, 1);

        // Log
        logList = new QListWidget(this);
        logList->setStyleSheet("font-family: monospace;");
        mainLayout->addWidget(logList);

        // Create photos directory
        QDir().mkdir("photos");
    }

    void startServer() {
        server = new Server(this);
        if(!server->listen(QHostAddress::Any, 8080)) {
            QMessageBox::critical(this, "Error", "Could not start server!");
            return;
        }

        // Get server IP
        QString ipAddress;
        const auto ipAddresses = QNetworkInterface::allAddresses();
        for (const QHostAddress &address : ipAddresses) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol &&
                address != QHostAddress::LocalHost) {
                ipAddress = address.toString();
                break;
            }
        }
        if (ipAddress.isEmpty())
            ipAddress = "127.0.0.1";

        statusLabel->setText(QString("Server running on %1:%2").arg(ipAddress).arg(server->serverPort()));
        logMessage("Server started. Waiting for connections...");

        connect(server, &Server::validRequest, this, &MainWindow::handleValidRequest);
    }

private slots:
    void handleValidRequest(const QString& surname) {
        logMessage(QString("Correct request from: %1").arg(surname));
        display->showPhoto(surname);
    }

    void logMessage(const QString& message) {
        QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
        logList->addItem(QString("[%1] %2").arg(timestamp).arg(message));
        logList->scrollToBottom();
    }

private:
    Server* server;
    PhotoDisplay* display;
    QLabel* statusLabel;
    QListWidget* logList;
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
