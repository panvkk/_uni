#ifndef PHOTOWIDGET_H
#define PHOTOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QTimer>

class PhotoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PhotoWidget(QWidget *parent = nullptr);

    // Функция для генерации бинарного файла из изображений в директории
    void createBinaryFileFromDir(const QString &dirPath, const QString &binaryFileName);
    // Функция для загрузки случайной записи (фотографии) из бинарного файла
    void loadRandomRecord(const QString &binaryFileName);

protected:
    // По нажатию левой кнопки мыши запускается/останавливается таймер
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    // Слот, который перемещает фотографию в случайную позицию (анимация)
    void movePhotoRandomly();

private:
    QLabel *photoLabel;  // Метка для отображения изображения
    QPixmap photoPixmap;
    QTimer *moveTimer;   // Таймер для движения изображения
};

#endif // PHOTOWIDGET_H
