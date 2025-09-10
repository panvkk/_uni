#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QSoundEffect>
#include <QPixmap>
#include <QElapsedTimer>
#include "settings.h"   // Структура GameSettings

class GameWidget : public QWidget {
    Q_OBJECT
public:
    explicit GameWidget(const GameSettings &settings, QWidget *parent = nullptr);
    ~GameWidget();

signals:
    // Сигнал теперь передаёт конечную позицию, затраченное время (сек) и уровень сложности.
    void gameOver(const QPoint &finalPos, double attemptTime, int difficulty);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void moveImage();
    void onImageClicked();

private:
    void updateHpLabel();

    QLabel *targetImageLabel;
    QLabel *hpLabel;
    QLabel *stopwatchLabel;   // Отображает время попытки
    QTimer *moveTimer;
    QSoundEffect *hitSound;
    QPixmap targetPixmap;
    QPoint imagePos;
    QPoint mousePos;
    int currentHp;
    int imageSpeed;
    int difficulty;          // 0 – легко, 1 – средне, 2 – сложно (определяется по initialHp)
    QElapsedTimer attemptTimer;  // Измеряет время попытки
};

#endif // GAMEWIDGET_H
