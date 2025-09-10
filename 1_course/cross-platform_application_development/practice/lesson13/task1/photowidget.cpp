#include "photowidget.h"
#include "movablephotolabel.h"

#include <QDir>
#include <QFile>
#include <QDataStream>
#include <QRandomGenerator>
#include <QDebug>
#include <QCoreApplication>

PhotoWidget::PhotoWidget(QWidget *parent)
    : QWidget(parent)
{
    // Устанавливаем фиксированный размер главного виджета
    setFixedSize(800, 600);

    // Задаём пути:
    // Используем директорию, где находится исполняемый файл, как базовую.
    QString appDir = QCoreApplication::applicationDirPath();
    QString photoDirPath = appDir + "/photo";          // папка с фотографиями студентов
    QString binaryFileName = appDir + "/students.info";  // имя бинарного файла

    // Генерируем бинарный файл из фотографий в директории
    createBinaryFileFromDir(photoDirPath, binaryFileName);

    // Загружаем все записи (фотографии) из бинарного файла и отображаем их на экране
    loadAllRecords(binaryFileName);
}

void PhotoWidget::createBinaryFileFromDir(const QString &dirPath, const QString &binaryFileName)
{
    QDir directory(dirPath);
    if (!directory.exists()) {
        qWarning() << "Directory" << dirPath << "does not exist.";
        return;
    }

    // Фильтруем файлы по расширениям изображений
    QStringList nameFilters;
    nameFilters << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp";
    directory.setNameFilters(nameFilters);

    // Получаем список файлов в директории
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

    // Для каждого файла записываем:
    // [QString URI] -> абсолютный путь к файлу,
    // [qint32 dataSize] -> размер данных изображения,
    // [dataSize байт] -> содержимое файла.
    for (const QString &fileName : fileList) {
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

void PhotoWidget::loadAllRecords(const QString &binaryFileName)
{
    QFile file(binaryFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open binary file:" << binaryFileName;
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_0);

    int recordCount;
    in >> recordCount;
    if (recordCount <= 0) {
        qWarning() << "No records in binary file!";
        file.close();
        return;
    }

    qDeleteAll(photoLabels);
    photoLabels.clear();

    for (int i = 0; i < recordCount; i++) {
        QString fileUri;
        in >> fileUri;
        qint32 dataSize;
        in >> dataSize;
        QByteArray imageData;
        imageData.resize(dataSize);
        in.readRawData(imageData.data(), dataSize);

        QPixmap pixmap;
        if (!pixmap.loadFromData(imageData)) {
            qWarning() << "Failed to load image from data for" << fileUri;
            continue;
        }

        MovablePhotoLabel *label = new MovablePhotoLabel(this);
        label->setPixmap(pixmap);
        label->setScaledContents(true);
        label->resize(150, 150);

        int maxX = width() - label->width();
        int maxY = height() - label->height();
        int randX = QRandomGenerator::global()->bounded(maxX);
        int randY = QRandomGenerator::global()->bounded(maxY);
        label->move(randX, randY);
        label->show();

        photoLabels.append(label);
    }

    file.close();
    qDebug() << "Loaded" << photoLabels.size() << "photo(s) from binary file.";
}
