#ifndef PHOTOWIDGET_H
#define PHOTOWIDGET_H

#include <QWidget>
#include <QList>

class MovablePhotoLabel; // Предварительное объявление

class PhotoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PhotoWidget(QWidget *parent = nullptr);

    // Функция для генерации бинарного файла из изображений в директории
    void createBinaryFileFromDir(const QString &dirPath, const QString &binaryFileName);
    // Функция для загрузки всех записей (фотографий) из бинарного файла
    void loadAllRecords(const QString &binaryFileName);

private:
    // Список виджетов с фотографиями студентов
    QList<MovablePhotoLabel*> photoLabels;
};

#endif // PHOTOWIDGET_H
