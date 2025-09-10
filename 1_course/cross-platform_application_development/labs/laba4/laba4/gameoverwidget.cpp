#include "gameoverwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <limits>

namespace {
double bestTimes[3] = { std::numeric_limits<double>::max(),
                       std::numeric_limits<double>::max(),
                       std::numeric_limits<double>::max() };
}

GameOverWidget::GameOverWidget(QWidget *parent) : QWidget(parent) {
    QString hearseImagePath = QCoreApplication::applicationDirPath() + "/images/hearse.png";

    qDebug() << "Loading hearse image from:" << hearseImagePath;
    hearsePixmap.load(hearseImagePath);
    if (hearsePixmap.isNull()) {
        qWarning("Failed to load hearse image! Check path: %s", qPrintable(hearseImagePath));
        hearsePixmap = QPixmap(600, 320);
        hearsePixmap.fill(Qt::black);
    }

    hearseLabel = new QLabel(this);
    hearseLabel->setPixmap(hearsePixmap.scaled(600, 320, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    hearseLabel->setFixedSize(hearseLabel->pixmap().size());
    hearseLabel->hide();

    photoLabel = new QLabel(this);
    photoLabel->hide();

    playAgainButton = new QPushButton("Играть снова", this);
    quitButton = new QPushButton("Выход", this);
    playAgainButton->setMinimumSize(150, 50);
    quitButton->setMinimumSize(150, 50);
    playAgainButton->hide();
    quitButton->hide();

    timeLabel = new QLabel(this);
    recordLabel = new QLabel(this);
    gradeLabel = new QLabel(this);
    timeLabel->hide();
    recordLabel->hide();
    gradeLabel->hide();

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(playAgainButton);
    buttonLayout->addWidget(quitButton);
    buttonLayout->addStretch();

    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->addWidget(timeLabel);
    infoLayout->addWidget(recordLabel);
    infoLayout->addWidget(gradeLabel);
    infoLayout->setAlignment(Qt::AlignCenter);

    // Добавляем растяжки сверху и снизу для вертикального центрирования информации
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addLayout(infoLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    connect(playAgainButton, &QPushButton::clicked, this, &GameOverWidget::playAgainRequested);
    connect(quitButton, &QPushButton::clicked, qApp, &QApplication::quit);

    // Инициализируем музыку "funeral-march.wav"
    funeralMusic = new QSoundEffect(this);
    QString musicPath = QCoreApplication::applicationDirPath() + "/sounds/funeral-march.wav";
    funeralMusic->setSource(QUrl::fromLocalFile(musicPath));
    // Можно установить дополнительные параметры, например, volume
    funeralMusic->setVolume(0.2f);

    setupAnimations();
}

void GameOverWidget::setupAnimations() {
    hearseToPhotoAnimation = new QPropertyAnimation(hearseLabel, "pos", this);
    hearseToPhotoAnimation->setDuration(1500);
    hearseToPhotoAnimation->setEasingCurve(QEasingCurve::InOutQuad);

    hearseOffScreenAnimation = new QPropertyAnimation(hearseLabel, "pos", this);
    hearseOffScreenAnimation->setDuration(2250);
    hearseOffScreenAnimation->setEasingCurve(QEasingCurve::InQuad);

    connect(hearseToPhotoAnimation, &QPropertyAnimation::finished, [this]() {
        photoLabel->setParent(hearseLabel);
        photoLabel->move((hearseLabel->width() - photoLabel->width()) / 2,
                         (hearseLabel->height() - photoLabel->height()) / 2);
        photoLabel->show();

        hearseOffScreenAnimation->setEndValue(QPoint(-hearseLabel->width() - photoLabel->width(), hearseLabel->y()));
        hearseOffScreenAnimation->start();
    });

    connect(hearseOffScreenAnimation, &QPropertyAnimation::finished, [this]() {
        photoLabel->hide();
        hearseLabel->hide();
        timeLabel->show();
        recordLabel->show();
        gradeLabel->show();
        playAgainButton->show();
        quitButton->show();
    });
}

void GameOverWidget::startAnimation(const QPoint &photoInitialPos, const QPixmap &photoPixmap,
                                    double attemptTime, int difficulty) {
    currentPhotoPixmap = photoPixmap;
    photoLabel->setPixmap(currentPhotoPixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    photoLabel->setFixedSize(photoLabel->pixmap().size());
    photoLabel->move(photoInitialPos);
    photoLabel->show();

    // Новое распределение оценок:
    // Сложная (difficulty == 2): оценка 10 при 15 сек, линейно уменьшается до 1 при 50 сек.
    // Средняя (difficulty == 1):  оценка 8 при 10 сек, линейно уменьшается до 1 при 40 сек.
    // Лёгкая  (difficulty == 0):  оценка 6 при 5 сек,  линейно уменьшается до 1 при 30 сек.
    double idealTime = 0.0, worstTime = 0.0;
    int maxGrade = 0;
    if (difficulty == 2) {           // Сложная
        maxGrade = 10;
        idealTime = 15.0;
        worstTime = 50.0;
    } else if (difficulty == 1) {    // Средняя
        maxGrade = 8;
        idealTime = 10.0;
        worstTime = 40.0;
    } else if (difficulty == 0) {    // Лёгкая
        maxGrade = 6;
        idealTime = 5.0;
        worstTime = 30.0;
    }

    int grade;
    if (attemptTime <= idealTime)
        grade = maxGrade;
    else if (attemptTime >= worstTime)
        grade = 1;
    else {
        double ratio = (attemptTime - idealTime) / (worstTime - idealTime);
        grade = maxGrade - qRound((maxGrade - 1) * ratio);
        if (grade < 1)
            grade = 1;
    }

    timeLabel->setAlignment(Qt::AlignCenter);
    recordLabel->setAlignment(Qt::AlignCenter);
    gradeLabel->setAlignment(Qt::AlignCenter);
    QFont infoFont = timeLabel->font();
    infoFont.setPointSize(24);
    timeLabel->setFont(infoFont);
    recordLabel->setFont(infoFont);
    gradeLabel->setFont(infoFont);
    // Используем более потусклевший синий цвет (#336699)
    timeLabel->setStyleSheet("color: #336699;");
    recordLabel->setStyleSheet("color: #336699;");
    gradeLabel->setStyleSheet("color: #336699;");

    timeLabel->setText(QString("Время попытки: %1 сек").arg(attemptTime, 0, 'f', 2));
    if (attemptTime < bestTimes[difficulty]) {
        bestTimes[difficulty] = attemptTime;
        recordLabel->setText("Новый рекорд!");
    } else {
        recordLabel->setText(QString("Рекорд: %1 сек").arg(bestTimes[difficulty], 0, 'f', 2));
    }
    gradeLabel->setText(QString("Оценка за зачёт: %1").arg(grade));

    // Запускаем музыку funeral-march.wav перед анимацией
    funeralMusic->play();

    hearseLabel->move(width(), photoInitialPos.y() + photoLabel->height() / 2 - hearseLabel->height() / 2);
    hearseLabel->show();

    QPoint targetPosForHearse = QPoint(photoInitialPos.x() - hearseLabel->width() + 20,
                                       hearseLabel->y());
    hearseToPhotoAnimation->setStartValue(hearseLabel->pos());
    hearseToPhotoAnimation->setEndValue(targetPosForHearse);

    playAgainButton->hide();
    quitButton->hide();
    hearseToPhotoAnimation->start();
}
