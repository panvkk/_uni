#include "gamewidget.h"
#include <QApplication>
#include <QRandomGenerator>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QMouseEvent>
#include <QEventLoop>
#include <QElapsedTimer>
#include <QCursor>
#include <QtMath>

static const int MARGIN = 75;

GameWidget::GameWidget(const GameSettings &settings, QWidget *parent)
    : QWidget(parent),
    currentHp(settings.initialHp),
    imageSpeed(settings.imageSpeed)
{
    // Определяем уровень сложности по начальному количеству HP:
    // 3 HP → легко (0), 5 HP → средне (1), 7 HP → сложно (2)
    if (settings.initialHp == 3)
        difficulty = 0;
    else if (settings.initialHp == 5)
        difficulty = 1;
    else if (settings.initialHp == 7)
        difficulty = 2;
    else
        difficulty = 1; // значение по умолчанию

    setMouseTracking(true);

    // Запускаем таймер попытки
    attemptTimer.start();

    // Устанавливаем кастомный курсор из bat.png, увеличенный в 2,5 раза (80x80)
    QString batCursorPath = QCoreApplication::applicationDirPath() + "/images/bat.png";
    QPixmap batPixmap;
    batPixmap.load(batCursorPath);
    if (!batPixmap.isNull()) {
        QPixmap scaledBat = batPixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QCursor batCursor(scaledBat);
        setCursor(batCursor);
    }

    // Создаем секундомер, который отображается в верхней части по центру
    stopwatchLabel = new QLabel(this);
    QFont swFont = stopwatchLabel->font();
    swFont.setPointSize(24);
    stopwatchLabel->setFont(swFont);
    stopwatchLabel->setStyleSheet("color: red;");
    stopwatchLabel->setAlignment(Qt::AlignCenter);
    stopwatchLabel->setText("0.00");
    stopwatchLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    QString imagePath = QCoreApplication::applicationDirPath() + "/images/target.png";
    QString soundPath = QCoreApplication::applicationDirPath() + "/sounds/hit.wav";

    qDebug() << "Loading target image from:" << imagePath;
    targetPixmap.load(imagePath);
    if (targetPixmap.isNull()) {
        qWarning("Failed to load target image! Check path: %s", qPrintable(imagePath));
        targetPixmap = QPixmap(100, 100);
        targetPixmap.fill(Qt::red);
    }

    // Увеличиваем изображение цели до 140×140
    targetImageLabel = new QLabel(this);
    targetImageLabel->setPixmap(targetPixmap.scaled(140, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    targetImageLabel->setFixedSize(targetImageLabel->pixmap().size());
    targetImageLabel->setMouseTracking(true);
    targetImageLabel->installEventFilter(this);

    hpLabel = new QLabel(this);
    updateHpLabel();
    hpLabel->move(10, 10);

    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &GameWidget::moveImage);
    moveTimer->start(20);

    hitSound = new QSoundEffect(this);
    qDebug() << "Loading hit sound from:" << QUrl::fromLocalFile(soundPath).toString();
    hitSound->setSource(QUrl::fromLocalFile(soundPath));
    if (hitSound->status() == QSoundEffect::Error) {
        qWarning("Failed to load hit sound! Check path: %s", qPrintable(soundPath));
    } else if (hitSound->status() == QSoundEffect::Loading) {
        QEventLoop loop;
        QObject::connect(hitSound, &QSoundEffect::loadedChanged, &loop, &QEventLoop::quit);
        loop.exec();
    }
    hitSound->setVolume(0.32f);

    // --- Новая логика: выбрать случайную начальную позицию внутри допустимой области
    int maxX = width() - targetImageLabel->width() - MARGIN;
    int maxY = height() - targetImageLabel->height() - MARGIN;
    int randX = QRandomGenerator::global()->bounded(MARGIN, (maxX >= MARGIN ? maxX + 1 : MARGIN + 1));
    int randY = QRandomGenerator::global()->bounded(MARGIN, (maxY >= MARGIN ? maxY + 1 : MARGIN + 1));
    imagePos = QPoint(randX, randY);
    targetImageLabel->move(imagePos);
    // --- Конец новой логики

    // Если необходимо, можно установить позицию секундомера здесь или в resizeEvent.
}



GameWidget::~GameWidget() { }

void GameWidget::mouseMoveEvent(QMouseEvent *event) {
    mousePos = event->pos();
    QWidget::mouseMoveEvent(event);
}

bool GameWidget::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            onImageClicked();
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void GameWidget::onImageClicked() {
    if (currentHp > 0) {
        currentHp--;
        updateHpLabel();
        hitSound->play();
        qDebug() << "Image clicked! HP:" << currentHp;

        if (currentHp <= 0) {
            moveTimer->stop();
            double elapsed = attemptTimer.elapsed() / 1000.0;
            qDebug() << "Game Over! Final image position:" << targetImageLabel->pos() << "Time:" << elapsed;
            emit gameOver(targetImageLabel->pos(), elapsed, difficulty);
        } else {
            // Перемещаем цель в случайную область с учётом отступов
            int maxX = width() - MARGIN - targetImageLabel->width();
            int maxY = height() - MARGIN - targetImageLabel->height();
            int randX = QRandomGenerator::global()->bounded(MARGIN, maxX + 1);
            int randY = QRandomGenerator::global()->bounded(MARGIN, maxY + 1);
            imagePos = QPoint(randX, randY);
            targetImageLabel->move(imagePos);
        }
    }
}

void GameWidget::moveImage() {
    // Обновляем секундомер
    double elapsedTime = attemptTimer.elapsed() / 1000.0;
    stopwatchLabel->setText(QString::number(elapsedTime, 'f', 2));

    // Убираем проверку "если курсор над фото, не двигаться"
    // if (targetImageLabel->geometry().contains(mousePos))
    //     return;

    int speedMultiplier = 2;
    // Увеличиваем базовую скорость в 1.5 раза
    int effectiveSpeed = static_cast<int>(imageSpeed * speedMultiplier * 1.5);

    int dx = 0, dy = 0;
    // Определяем центр цели
    QPoint center = imagePos + QPoint(targetImageLabel->width() / 2, targetImageLabel->height() / 2);
    double distance = qSqrt(qPow(mousePos.x() - center.x(), 2) + qPow(mousePos.y() - center.y(), 2));
    double repulsionThreshold = 100.0;

    if (distance < repulsionThreshold) {
        // Если курсор ближе 100px, рассчитываем отталкивающий вектор
        double factor = (repulsionThreshold - distance) / repulsionThreshold;
        dx = factor * effectiveSpeed * (center.x() - mousePos.x()) / (distance + 1);
        dy = factor * effectiveSpeed * (center.y() - mousePos.y()) / (distance + 1);
    } else {
        // Стандартная логика движения от курсора
        if (mousePos.x() < center.x())
            dx = effectiveSpeed;
        else if (mousePos.x() > center.x())
            dx = -effectiveSpeed;
        if (mousePos.y() < center.y())
            dy = effectiveSpeed;
        else if (mousePos.y() > center.y())
            dy = -effectiveSpeed;

        // Добавляем случайное смещение для разнообразия
        if (QRandomGenerator::global()->bounded(10) < 2) {
            dx += QRandomGenerator::global()->bounded(-effectiveSpeed / 2, effectiveSpeed / 2 + 1);
            dy += QRandomGenerator::global()->bounded(-effectiveSpeed / 2, effectiveSpeed / 2 + 1);
        }
    }

    int targetX = imagePos.x() + dx;
    int targetY = imagePos.y() + dy;

    // Ограничиваем перемещение так, чтобы объект не заходил ближе 75 пикселей от краёв (MARGIN)
    static const int MARGIN = 75;
    if (targetX < MARGIN)
        targetX = MARGIN;
    if (targetX + targetImageLabel->width() > width() - MARGIN)
        targetX = width() - MARGIN - targetImageLabel->width();
    if (targetY < MARGIN)
        targetY = MARGIN;
    if (targetY + targetImageLabel->height() > height() - MARGIN)
        targetY = height() - MARGIN - targetImageLabel->height();

    // Плавное смещение через интерполяцию
    float alpha = 0.3f;
    int smoothX = imagePos.x() + static_cast<int>(alpha * (targetX - imagePos.x()));
    int smoothY = imagePos.y() + static_cast<int>(alpha * (targetY - imagePos.y()));

    imagePos = QPoint(smoothX, smoothY);
    targetImageLabel->move(imagePos);
}


void GameWidget::updateHpLabel() {
    hpLabel->setText(QString("HP: %1").arg(currentHp));
    hpLabel->adjustSize();
}

void GameWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    hpLabel->move(10, 10);
    // Центрируем секундомер вверху
    stopwatchLabel->setGeometry((width() - 200) / 2, 10, 200, 50);

    // Корректируем imagePos с учетом отступов
    if (imagePos.x() + targetImageLabel->width() > width() - MARGIN)
        imagePos.setX(width() - MARGIN - targetImageLabel->width());
    if (imagePos.y() + targetImageLabel->height() > height() - MARGIN)
        imagePos.setY(height() - MARGIN - targetImageLabel->height());
    if (imagePos.x() < MARGIN)
        imagePos.setX(MARGIN);
    if (imagePos.y() < MARGIN)
        imagePos.setY(MARGIN);
    targetImageLabel->move(imagePos);
}
