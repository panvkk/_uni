#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "startmenu.h"
#include "gamewidget.h"
#include "gameoverwidget.h"
#include "settings.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showStartMenu();
    void startGame(const GameSettings &settings);
    // Обновлённая сигнатура слота — теперь принимает 3 параметра:
    void handleGameOver(const QPoint &lastImagePos, double attemptTime, int difficulty);

private:
    QStackedWidget *stackedWidget;
    StartMenu *startMenu;
    GameWidget *gameWidget;         // будет создан при старте игры
    GameOverWidget *gameOverWidget;

    QPixmap lastTargetPixmap; // для передачи на экран Game Over
};

#endif // MAINWINDOW_H
