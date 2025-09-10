
#include "movablephotolabel.h"
#include <QRandomGenerator>
#include <QWidget>

MovablePhotoLabel::MovablePhotoLabel(QWidget *parent)
    : QLabel(parent)
{
    moveTimer = new QTimer(this);
    moveTimer->setInterval(100); // каждые 100 мс обновляем позицию
    connect(moveTimer, &QTimer::timeout, this, &MovablePhotoLabel::moveRandomly);
}

void MovablePhotoLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // При каждом клике левой кнопкой переключаем состояние движения:
        if (!moveTimer->isActive())
            startMoving();
        else
            stopMoving();
    }
    QLabel::mousePressEvent(event);
}

void MovablePhotoLabel::startMoving()
{
    moveTimer->start();
}

void MovablePhotoLabel::stopMoving()
{
    moveTimer->stop();
}

bool MovablePhotoLabel::isMoving() const
{
    return moveTimer->isActive();
}

void MovablePhotoLabel::moveRandomly()
{
    if (!parentWidget()) return;
    int maxX = parentWidget()->width() - width();
    int maxY = parentWidget()->height() - height();
    if(maxY > 50 && maxX > 50) {
        int newX = QRandomGenerator::global()->bounded(maxX);
        int newY = QRandomGenerator::global()->bounded(maxY);
        move(newX, newY);
    } else if(maxY > 50) {
        int newY = QRandomGenerator::global()->bounded(maxY);
        move(maxX, newY);
    } else if(maxX > 50) {
        int newX = QRandomGenerator::global()->bounded(maxX);
        move(newX, maxY);
    }
}
