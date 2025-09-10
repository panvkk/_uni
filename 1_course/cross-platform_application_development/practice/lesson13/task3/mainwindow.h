#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>

class PhotoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PhotoWidget(QWidget *parent = nullptr);
    // Функция загрузки случайной записи (фото) из бинарного файла
    void loadRandomRecord(const QString &binaryFileName);

protected:
    // Обрабатываем перемещения мыши – фото будет убегать от курсора
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QLabel *photoLabel; // Метка для отображения изображения
    QPixmap photoPixmap;
};

#endif // MAINWINDOW_H
