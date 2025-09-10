#include "mainwindow.h"
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    stackedWidget = new QStackedWidget(this);

    startMenu = new StartMenu(this);
    connect(startMenu, &StartMenu::startGameRequested, this, &MainWindow::startGame);

    gameWidget = nullptr;

    gameOverWidget = new GameOverWidget(this);
    connect(gameOverWidget, &GameOverWidget::playAgainRequested, this, &MainWindow::showStartMenu);

    stackedWidget->addWidget(startMenu);

    setCentralWidget(stackedWidget);
    setWindowTitle("На зачёте по РК-ПП");
    resize(1000, 750); // Увеличен размер окна для начального меню

    QString targetImagePath = QCoreApplication::applicationDirPath() + "/images/target.png";
    qDebug() << "Пытаюсь загрузить lastTargetPixmap из:" << targetImagePath;
    if (!lastTargetPixmap.load(targetImagePath)) {
        qWarning("Не удалось загрузить изображение цели (lastTargetPixmap)! Проверьте путь: %s", qPrintable(targetImagePath));
        lastTargetPixmap = QPixmap(80, 80);
        lastTargetPixmap.fill(Qt::red);
    } else {
        lastTargetPixmap = lastTargetPixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    showStartMenu();
}

MainWindow::~MainWindow()
{
}

void MainWindow::showStartMenu() {
    if (gameWidget) {
        stackedWidget->removeWidget(gameWidget);
        delete gameWidget;
        gameWidget = nullptr;
    }
    if (gameOverWidget && stackedWidget->indexOf(gameOverWidget) != -1) {
        stackedWidget->removeWidget(gameOverWidget);
    }
    if (stackedWidget->indexOf(startMenu) == -1) {
        stackedWidget->addWidget(startMenu);
    }
    stackedWidget->setCurrentWidget(startMenu);
}

void MainWindow::startGame(const GameSettings &settings) {
    if (gameWidget) {
        stackedWidget->removeWidget(gameWidget);
        delete gameWidget;
    }
    gameWidget = new GameWidget(settings, this);
    // Обратите внимание – теперь сигнал gameOver передаёт 3 параметра: позицию, время и уровень сложности.
    connect(gameWidget, &GameWidget::gameOver, this, &MainWindow::handleGameOver);

    if (stackedWidget->indexOf(gameWidget) == -1) {
        stackedWidget->addWidget(gameWidget);
    }
    stackedWidget->setCurrentWidget(gameWidget);
    gameWidget->setFocus();
}

// Новый слот, принимающий три параметра: lastImagePos, attemptTime и difficulty.
void MainWindow::handleGameOver(const QPoint &lastImagePos, double attemptTime, int difficulty) {
    if (gameWidget) {
        stackedWidget->removeWidget(gameWidget);
    }
    if (stackedWidget->indexOf(gameOverWidget) == -1) {
        stackedWidget->addWidget(gameOverWidget);
    }
    stackedWidget->setCurrentWidget(gameOverWidget);
    gameOverWidget->startAnimation(lastImagePos, lastTargetPixmap, attemptTime, difficulty);
}
