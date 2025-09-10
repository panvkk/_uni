#ifndef MOVABLEPHOTOLABEL_H
#define MOVABLEPHOTOLABEL_H

#include <QLabel>
#include <QTimer>
#include <QMouseEvent>

class MovablePhotoLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MovablePhotoLabel(QWidget *parent = nullptr);

    void startMoving();
    void stopMoving();
    bool isMoving() const;

protected:
    // При нажатии левой кнопкой мыши происходит переключение движения
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    // Слот для перемещения метки в случайное место
    void moveRandomly();

private:
    QTimer *moveTimer;
};

#endif // MOVABLEPHOTOLABEL_H
