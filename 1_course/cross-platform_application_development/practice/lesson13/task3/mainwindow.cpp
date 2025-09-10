#include "mainwindow.h"

#include <QFile>
#include <QDataStream>
#include <QRandomGenerator>
#include <QMouseEvent>
#include <QCoreApplication>
#include <QDebug>
#include <QtMath>
#include <QCursor>

PhotoWidget::PhotoWidget(QWidget *parent)
    : QWidget(parent),
    photoLabel(new QLabel(this))
{
    // setFixedSize(800, 600);


    setMouseTracking(true);

    QString appDir = QCoreApplication::applicationDirPath();
    QString baseballBatDir = appDir + "/bat1.png";
    QString binaryFileName = appDir + "/students.info";

    QPixmap batPixmap(baseballBatDir);

    batPixmap.scaled((width() - photoLabel->width())/10, (height() - photoLabel->height())/10, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    if (!batPixmap.isNull()) {
        QCursor batCursor(batPixmap, batPixmap.width()/2, batPixmap.height()/2);
        setCursor(batCursor);
    } else {
        qWarning() << "Bat cursor image not found!";
    }


    loadRandomRecord(binaryFileName);

    if (!photoPixmap.isNull()) {
        photoLabel->setPixmap(photoPixmap);
    }
    photoLabel->setScaledContents(true);
    // Задаём размер метки (например, 200x200) и размещаем по центру окна
    photoLabel->resize(200, 200);
    photoLabel->move((width() - photoLabel->width()) / 2,
                     (height() - photoLabel->height()) / 2);
}

void PhotoWidget::loadRandomRecord(const QString &binaryFileName)
{
    QFile file(binaryFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open binary file:" << binaryFileName;
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_0);

    int recordCount = 0;
    in >> recordCount;
    if (recordCount <= 0) {
        qWarning() << "No records found in binary file!";
        file.close();
        return;
    }

    // Выбираем случайный индекс от 0 до recordCount-1
    int randomIndex = QRandomGenerator::global()->bounded(recordCount);

    for (int i = 0; i < recordCount; i++) {
        QString fileUri;
        in >> fileUri;
        qint32 dataSize;
        in >> dataSize;

        QByteArray imageData;
        imageData.resize(dataSize);
        in.readRawData(imageData.data(), dataSize);

        if (i == randomIndex) {
            qDebug() << "Selected photo:" << fileUri;
            if (!photoPixmap.loadFromData(imageData)) {
                qWarning() << "Failed to load image from data in record" << i;
            }
            break;
        }
    }
    file.close();
}

// При перемещении мыши фото убегает от курсора
void PhotoWidget::mouseMoveEvent(QMouseEvent *event)
{
    // Координаты курсора в системе координат окна
    QPoint mousePos = event->pos();

    // Получаем текущую геометрию метки и находим её центр
    QRect labelGeom = photoLabel->geometry();
    QPoint labelCenter = labelGeom.center();

    // Вычисляем вектор от курсора к центру метки: (labelCenter - mousePos)
    QPoint diff = labelCenter - mousePos;
    if (diff.manhattanLength() == 0) {
        diff = QPoint(1, 1); // Если курсор точно у центра, выбираем произвольное направление
    }

    // Нормализуем вектор и умножаем на фиксированный шаг перемещения (например, 10 пикселей)
    double length = qSqrt(diff.x()*diff.x() + diff.y()*diff.y());
    double step = 10.0; // шаг перемещения
    int dx = int(diff.x() / length * step);
    int dy = int(diff.y() / length * step);

    // Вычисляем новое положение метки (смещая её от текущего положения)
    int newX = photoLabel->x() + dx;
    int newY = photoLabel->y() + dy;

    // Ограничиваем новое положение, чтобы метка не выходила за границы окна
    if (newX < 50) newX = 50;
    if (newY < 50) newY = 50;
    if (newX > width() - photoLabel->width() - 50) newX = width() - photoLabel->width() - 50;
    if (newY > height() - photoLabel->height() - 50) newY = height() - photoLabel->height() - 50;

    photoLabel->move(newX, newY);

    QWidget::mouseMoveEvent(event);
}


/*
    QPixmap batPixmap(":/bat.png"); // Загрузка изображения курсора
    if (!batPixmap.isNull()) {
        // Масштабируем изображение до 64x64 пикселей, сохраняя пропорции
        batPixmap = batPixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // Устанавливаем курсор, задавая его горячую точку (например, центр)
        QCursor batCursor(batPixmap, batPixmap.width()/2, batPixmap.height()/2);
        setCursor(batCursor);
    } else {
        qWarning() << "Bat cursor image not found!";
    }

 */
