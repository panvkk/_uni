#ifndef GAMEOVERWIDGET_H
#define GAMEOVERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QSoundEffect>  // Не забудьте подключить QSoundEffect!

class GameOverWidget : public QWidget {
    Q_OBJECT
public:
    explicit GameOverWidget(QWidget *parent = nullptr);
signals:
    void playAgainRequested();
public slots:
    // Слот принимает позицию фото, само фото, время попытки и уровень сложности.
    void startAnimation(const QPoint &photoInitialPos, const QPixmap &photoPixmap, double attemptTime, int difficulty);
private:
    void setupAnimations();

    QLabel *hearseLabel;
    QLabel *photoLabel;
    QLabel *timeLabel;
    QLabel *recordLabel;
    QLabel *gradeLabel;
    QPushButton *playAgainButton;
    QPushButton *quitButton;
    QPixmap hearsePixmap;
    QPropertyAnimation *hearseToPhotoAnimation;
    QPropertyAnimation *hearseOffScreenAnimation;
    QPixmap currentPhotoPixmap;
    QSoundEffect *funeralMusic;   // Новый указатель для музыки funeral-march.wav
};

#endif // GAMEOVERWIDGET_H
