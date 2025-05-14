#include "photowidget.h"

#include <QDir>
#include <QFile>
#include <QDataStream>
#include <QRandomGenerator>
#include <QMouseEvent>
#include <QDebug>

/*
Задача 1.
Ранее на занятиях статично по экрану гоняли месседж бокс и формировали бинарный файл с инфой про студентов и выводили на экран фотографии
студентов группы. Необходимо случайным образом из бинарного файла достать инфу и фотографию, вывести фото на экран и по нажатию
лкм заставить фотографию хаотично двигаться по экрану.
Задача 2.
Вывести на экран фотографии всех студентов группы из файла(бинарного) и последовательно нажимая на фотографии студентов
заставить эти фотографии скакать по экрану.
Задача 3.
Вывести на экран случайным образом выбранную фотографию и двигая мышью заставить фотографию убегать от курсора
Задача 4.
Изменить курсор мыши на бейсбольную биту и проделать то что сделано в задаче номер 3.

Д/З
Найти где и как угодно, но желательно на сайте ывалоыап инфу о том, что такое регулярные выражения и как используются
и что такое сериализация
 */

PhotoWidget::PhotoWidget(QWidget *parent)
    : QWidget(parent),
    photoLabel(new QLabel(this)),
    moveTimer(new QTimer(this))
{
    // Устанавливаем размер окна
    setFixedSize(800, 600);

    // Путь к директории с фотографиями.
    // В этом примере используется относительный путь "photo".
    // Положите в папку "photo" несколько изображений (png, jpg, jpeg, bmp).
    QString dirPath = QCoreApplication::applicationDirPath() + "/photo";
    QString binaryFileName = QCoreApplication::applicationDirPath() + "students.info";

    // Генерация бинарного файла из изображений
    createBinaryFileFromDir(dirPath, binaryFileName);

    // Загрузка случайной фотографии из бинарного файла
    loadRandomRecord(binaryFileName);

    // Если изображение успешно загружено, установить его в метку
    if (!photoPixmap.isNull()) {
        photoLabel->setPixmap(photoPixmap);
    }
    photoLabel->setScaledContents(true);
    // Задаем размер метки (например, 200x200) и размещаем по центру окна
    photoLabel->resize(200, 200);
    photoLabel->move((width() - photoLabel->width()) / 2,
                     (height() - photoLabel->height()) / 2);

    // Настраиваем таймер для перемещения изображения каждые 100 мс
    moveTimer->setInterval(100);
    connect(moveTimer, &QTimer::timeout, this, &PhotoWidget::movePhotoRandomly);
}

void PhotoWidget::createBinaryFileFromDir(const QString &dirPath, const QString &binaryFileName)
{
    QDir directory(dirPath);
    if (!directory.exists()) {
        qWarning() << "Directory" << dirPath << "does not exist.";
        return;
    }
    // Фильтр для изображений
    QStringList nameFilters;
    nameFilters << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp";
    directory.setNameFilters(nameFilters);

    // Получаем список файлов из директории
    QStringList fileList = directory.entryList(QDir::Files);
    int recordCount = fileList.size();

    QFile binFile(binaryFileName);
    if (!binFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot open" << binaryFileName << "for writing.";
        return;
    }

    QDataStream out(&binFile);
    out.setVersion(QDataStream::Qt_5_0);

    // Записываем количество записей
    out << recordCount;

    // Для каждого файла записываем: URI, размер данных и сами байты изображения
    for (const QString &fileName : fileList) {
        // Абсолютный путь к файлу
        QString fileUri = directory.absoluteFilePath(fileName);
        out << fileUri;

        QFile imgFile(fileUri);
        if (!imgFile.open(QIODevice::ReadOnly)) {
            qWarning() << "Cannot open image file:" << fileUri;
            continue;
        }
        QByteArray imageData = imgFile.readAll();
        imgFile.close();

        qint32 dataSize = imageData.size();
        out << dataSize;
        out.writeRawData(imageData.constData(), dataSize);
    }

    binFile.close();
    qDebug() << "Binary file" << binaryFileName << "created with" << recordCount << "records.";
}

void PhotoWidget::loadRandomRecord(const QString &binaryFileName)
{
    QFile file(binaryFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open" << binaryFileName;
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_0);

    int recordCount;
    in >> recordCount;
    if (recordCount <= 0) {
        qWarning() << "No records in" << binaryFileName;
        file.close();
        return;
    }

    // Выбираем случайный индекс записи от 0 до recordCount-1
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
            qDebug() << "Selected image:" << fileUri;
            // Загружаем изображение из байтов в QPixmap
            if (!photoPixmap.loadFromData(imageData)) {
                qWarning() << "Failed to load image from data!";
            }
            break;
        }
    }
    file.close();
}

void PhotoWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // При нажатии левой кнопкой мыши запускается (или останавливается) таймер
        if (!moveTimer->isActive())
            moveTimer->start();
        else
            moveTimer->stop();
    }
    QWidget::mousePressEvent(event);
}

void PhotoWidget::movePhotoRandomly()
{
    // Рассчитываем допустимые координаты, чтобы метка не выходила за границы окна
    int maxX = width() - photoLabel->width();
    int maxY = height() - photoLabel->height();
    int randX = QRandomGenerator::global()->bounded(maxX);
    int randY = QRandomGenerator::global()->bounded(maxY);
    photoLabel->move(randX, randY);
}
