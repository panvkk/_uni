#include "mainwindow.h"

#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QRandomGenerator>
#include <QPropertyAnimation>
#include <QDebug>
#include <QTimer>
#include <QCoreApplication>
#include <QVBoxLayout>
#include <QLayout>
#include <QtMath>
#include <QPushButton>
#include <QTextToSpeech>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_isEliminationInProgress(false),
    rhymeWordIndex(0),
    currentPersonIndex(0),
    eliminatedItem(nullptr)
{
    setFixedSize(1000, 800);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setStyleSheet("background-color: white;");

    wordLabel = new QLabel(this);
    wordLabel->setAlignment(Qt::AlignCenter);
    wordLabel->setStyleSheet("font: bold 24px; color: blue;");
    wordLabel->setGeometry(400, 350, 200, 50);

    longestWordDisplayLabel = new QLabel(this);
    longestWordDisplayLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    longestWordDisplayLabel->setStyleSheet("font: 16px; color: black;");
    longestWordDisplayLabel->setGeometry(10, 10, 380, 50);
    longestWordDisplayLabel->setWordWrap(true);

    nextWordButton = new QPushButton("Start Game", this);
    nextWordButton->setGeometry((width() - 100) / 2, height() - 60, 100, 40);
    nextWordButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #CCCCCC;"
        "  color: black;"
        "  border: 1px solid gray;"
        "}"
        "QPushButton:hover {"
        "  background-color: #BBBBBB;"
        "}"
        );
    connect(nextWordButton, &QPushButton::clicked, this, &MainWindow::startGame);

    tts = new QTextToSpeech(this);
    fireworksPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    fireworksPlayer->setAudioOutput(audioOutput);

    loadRhyme();
    loadPersons();
    arrangePhotos();

    wordLabel->raise();
    longestWordDisplayLabel->raise();
    nextWordButton->raise();

    if (!persons.isEmpty()) {
        currentPersonIndex = QRandomGenerator::global()->bounded(persons.size());
    }

    bool canStart = !persons.isEmpty() && !rhymeVerses.isEmpty();

    if (canStart) {
        int randRhymeIndex = QRandomGenerator::global()->bounded(rhymeVerses.size());
        currentRhymeWords = rhymeVerses[randRhymeIndex].split(" ", Qt::SkipEmptyParts).toVector();
        if (currentRhymeWords.isEmpty()) {
            wordLabel->setText("Ошибка: пустая считалка!");
            nextWordButton->setEnabled(false);
            canStart = false;
        } else {
            rhymeWordIndex = 0;
        }
    }

    updateLongestWordDisplay();

    if (persons.isEmpty()) {
        wordLabel->setText("Нет участников!");
        nextWordButton->setEnabled(false);
    } else if (rhymeVerses.isEmpty()) {
        wordLabel->setText("Нет считалок!");
        nextWordButton->setEnabled(false);
    } else if (!canStart) {
        // Сообщение уже установлено, кнопка выключена
    }
}

MainWindow::~MainWindow() {
    for (PhotoItem* item : photoItems) {
        if (item->container) {
            item->container->deleteLater();
        }
        delete item;
    }
    photoItems.clear();

    if (eliminatedItem) {
        if (eliminatedItem->container) {
            eliminatedItem->container->deleteLater();
        }
        delete eliminatedItem;
        eliminatedItem = nullptr;
    }
    delete fireworksPlayer;
    delete audioOutput;
}

void MainWindow::updateLongestWordDisplay() {
    longestWord = "";
    if (!currentRhymeWords.isEmpty()) {
        for (const QString &word : currentRhymeWords) {
            if (word.length() > longestWord.length()) {
                longestWord = word;
            }
        }
    }

    if (longestWord.isEmpty()) {
        longestWordDisplayLabel->setText("Cамое длинное слово считалки:<br>-");
    } else {
        longestWordDisplayLabel->setText(QString("Cамое длинное слово считалки:<br><span style='font-weight:bold; color:red; font-size:18px;'>%1</span>").arg(longestWord));
    }
}

void MainWindow::loadRhyme() {
    QString filePath = QCoreApplication::applicationDirPath() + "/rhyme.txt";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Не удалось открыть файл:" << filePath;
        wordLabel->setText("Ошибка: rhyme.txt не найден!");
        rhymeVerses.clear();
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty())
            rhymeVerses.append(line);
    }
    file.close();
    if (rhymeVerses.isEmpty()){
        wordLabel->setText("Файл rhyme.txt пуст!");
    }
}

void MainWindow::loadPersons() {
    QString photoDirPath = QCoreApplication::applicationDirPath() + "/photo";
    QDir photoDir(photoDirPath);
    if (!photoDir.exists()) {
        qDebug() << "Папка не найдена:" << photoDirPath;
        wordLabel->setText("Ошибка: папка /photo не найдена!");
        persons.clear();
        return;
    }
    QStringList nameFilters;
    nameFilters << "*.jpg" << "*.png";
    QFileInfoList fileList = photoDir.entryInfoList(nameFilters, QDir::Files);

    for (const QFileInfo &fileInfo : fileList) {
        QString fullName = fileInfo.baseName();
        QPixmap photo(fileInfo.absoluteFilePath());
        QString trackPath = QCoreApplication::applicationDirPath() + "/tracks/" + fullName + ".mp3";
        if (!QFile::exists(trackPath)) {
            trackPath = "";
        }
        persons.append(Person(fullName, photo, trackPath));
    }
    if (persons.isEmpty()){
        wordLabel->setText("В папке /photo нет участников!");
    }
}

void MainWindow::arrangePhotos() {
    for (PhotoItem* item : photoItems) {
        if (item->container) {
            item->container->deleteLater();
        }
        delete item;
    }
    photoItems.clear();

    int count = persons.size();
    if (count == 0) {
        return;
    }

    int radius = 300;
    QPoint center(width() / 2, height() / 2 - 20);

    for (int i = 0; i < count; ++i) {
        double angle = (2 * M_PI * i) / count - (M_PI / 2.0);
        int x = center.x() + radius * cos(angle) - 50;
        int y = center.y() + radius * sin(angle) - 60;

        QWidget *container = new QWidget(this);
        container->setGeometry(x, y, 100, 120);

        QVBoxLayout *layout = new QVBoxLayout(container);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(2);
        layout->setAlignment(Qt::AlignCenter);

        QLabel *photoLabel = new QLabel(container);
        QPixmap pix = persons[i].photo;
        if (!pix.isNull()) {
            photoLabel->setPixmap(pix.scaled(90, 90, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            photoLabel->setText("Нет фото");
            photoLabel->setAlignment(Qt::AlignCenter);
            photoLabel->setFixedSize(90, 90);
        }
        photoLabel->setAlignment(Qt::AlignCenter);

        QLabel *nameLabel = new QLabel(container);
        nameLabel->setText(persons[i].fullName);
        nameLabel->setAlignment(Qt::AlignCenter);
        nameLabel->setStyleSheet("color: black; font: 9pt;");
        nameLabel->setWordWrap(true);

        layout->addWidget(photoLabel);
        layout->addWidget(nameLabel);
        layout->setSizeConstraint(QLayout::SetFixedSize);

        container->setLayout(layout);
        container->show();

        PhotoItem *item = new PhotoItem();
        item->container = container;
        item->photoLabel = photoLabel;
        item->nameLabel = nameLabel;
        photoItems.append(item);
    }

    wordLabel->raise();
    longestWordDisplayLabel->raise();
    nextWordButton->raise();
    if (eliminatedItem && eliminatedItem->container) {
        eliminatedItem->container->raise();
    }
}

void MainWindow::startGame() {
    if (m_isEliminationInProgress) {
        return;
    }

    if (persons.size() <= 1 || currentRhymeWords.isEmpty()) {
        return;
    }

    // Сбрасываем интерфейс перед началом
    for (int i = 0; i < persons.size(); ++i) {
        if (i < photoItems.size() && photoItems[i] && photoItems[i]->nameLabel) {
            photoItems[i]->nameLabel->setText(persons[i].fullName);
        }
    }

    nextWordButton->setEnabled(false);
    processNextWord();
}

void MainWindow::processNextWord() {
    if (persons.size() <= 1 || currentRhymeWords.isEmpty()) {
        if (persons.size() == 1) {
            crownWinner();
        }
        return;
    }

    // Обновляем интерфейс: сбрасываем форматирование для всех студентов
    for (int i = 0; i < persons.size(); ++i) {
        if (i < photoItems.size() && photoItems[i] && photoItems[i]->nameLabel) {
            photoItems[i]->nameLabel->setText(persons[i].fullName);
        }
    }

    if (rhymeWordIndex < currentRhymeWords.size()) {
        QString word = currentRhymeWords[rhymeWordIndex];
        wordLabel->setText(word);

        // Проверяем текущего студента
        if (currentPersonIndex >= 0 && currentPersonIndex < persons.size() && currentPersonIndex < photoItems.size()) {
            PhotoItem *currentItem = photoItems[currentPersonIndex];
            if (currentItem && currentItem->nameLabel) {
                QString formattedText = persons[currentPersonIndex].fullName +
                                        "<br><font color='red'>" + word + "</font>";
                currentItem->nameLabel->setText(formattedText);
            }
        }

        // Произносим слово
        tts->say(word);
        rhymeWordIndex++;

        // Переходим к следующему студенту сразу после слова
        currentPersonIndex = (currentPersonIndex + 1) % persons.size();

        // Если это последнее слово считалки, вызываем выбывание
        if (rhymeWordIndex >= currentRhymeWords.size()) {
            connect(tts, &QTextToSpeech::stateChanged, this, [this](QTextToSpeech::State state) {
                if (state == QTextToSpeech::Ready) {
                    disconnect(tts, &QTextToSpeech::stateChanged, this, nullptr);
                    removeCurrentPerson();
                }
            });
        } else {
            // Ждем завершения произнесения слова, затем переходим к следующему слову
            connect(tts, &QTextToSpeech::stateChanged, this, [this](QTextToSpeech::State state) {
                if (state == QTextToSpeech::Ready) {
                    disconnect(tts, &QTextToSpeech::stateChanged, this, nullptr);
                    processNextWord();
                }
            });
        }
    }
}

void MainWindow::removeCurrentPerson() {
    if (persons.isEmpty() || currentPersonIndex < 0 || currentPersonIndex >= persons.size() || m_isEliminationInProgress) {
        return;
    }

    m_isEliminationInProgress = true;

    int person_to_remove_idx = (currentPersonIndex - 1 + persons.size()) % persons.size();
    if (person_to_remove_idx >= photoItems.size()) {
        qDebug() << "Error: person_to_remove_idx out of bounds for photoItems.";
        m_isEliminationInProgress = false;
        return;
    }
    PhotoItem *photo_item_to_animate = photoItems[person_to_remove_idx];

    QPropertyAnimation *anim = new QPropertyAnimation(photo_item_to_animate->container, "pos");
    anim->setDuration(1000);
    anim->setStartValue(photo_item_to_animate->container->pos());
    anim->setEndValue(QPoint(850, 10));
    anim->setEasingCurve(QEasingCurve::InOutQuad);
    anim->start(QAbstractAnimation::DeleteWhenStopped);

    // Заменяем QSoundEffect на QMediaPlayer для трека участника
    if (!persons[person_to_remove_idx].trackPath.isEmpty()) {
        QMediaPlayer *effectPlayer = new QMediaPlayer(this);
        QAudioOutput *effectAudioOutput = new QAudioOutput(this);
        effectPlayer->setAudioOutput(effectAudioOutput);
        effectPlayer->setSource(QUrl::fromLocalFile(persons[person_to_remove_idx].trackPath));
        effectAudioOutput->setVolume(0.5);
        if (effectPlayer->error() == QMediaPlayer::NoError) {
            effectPlayer->play();
            connect(effectPlayer, &QMediaPlayer::playbackStateChanged, this, [effectPlayer, effectAudioOutput]() {
                if (effectPlayer->playbackState() == QMediaPlayer::StoppedState) {
                    delete effectPlayer;
                    delete effectAudioOutput;
                }
            });
        } else {
            qDebug() << "Ошибка воспроизведения трека:" << effectPlayer->errorString();
            delete effectPlayer;
            delete effectAudioOutput;
        }
    }

    QTimer::singleShot(1100, this, [this, person_to_remove_idx, photo_item_to_animate]() {
        if (person_to_remove_idx >= persons.size()) {
            qDebug() << "Error: person_to_remove_idx became invalid for persons list during timer.";
            m_isEliminationInProgress = false;
            return;
        }

        QString eliminatedLabelText = persons[person_to_remove_idx].fullName +
                                      "<br><font color='red'>Выбыл!</font>";
        photo_item_to_animate->nameLabel->setText(eliminatedLabelText);

        if (eliminatedItem != nullptr) {
            if (eliminatedItem->container) {
                eliminatedItem->container->hide();
                eliminatedItem->container->deleteLater();
            }
            delete eliminatedItem;
            eliminatedItem = nullptr;
        }

        eliminatedItem = new PhotoItem();
        eliminatedItem->container = photo_item_to_animate->container;
        eliminatedItem->photoLabel = photo_item_to_animate->photoLabel;
        eliminatedItem->nameLabel = photo_item_to_animate->nameLabel;
        eliminatedItem->container->raise();

        persons.removeAt(person_to_remove_idx);

        PhotoItem* removed_wrapper = photoItems.takeAt(person_to_remove_idx);
        if (removed_wrapper != photo_item_to_animate) {
            qDebug() << "Warning: removed_wrapper is not photo_item_to_animate!";
        }
        removed_wrapper->container = nullptr;
        removed_wrapper->photoLabel = nullptr;
        removed_wrapper->nameLabel = nullptr;
        delete removed_wrapper;

        if (this->currentPersonIndex > person_to_remove_idx) {
            this->currentPersonIndex--;
        }
        if (this->currentPersonIndex >= persons.size()) {
            this->currentPersonIndex = 0;
        }
        if (persons.isEmpty()) {
            this->currentPersonIndex = 0;
        }

        arrangePhotos();

        currentRhymeWords.clear();
        if (!persons.isEmpty() && !rhymeVerses.isEmpty()) {
            int newRhymeIndex = QRandomGenerator::global()->bounded(rhymeVerses.size());
            currentRhymeWords = rhymeVerses[newRhymeIndex].split(" ", Qt::SkipEmptyParts).toVector();
            if (currentRhymeWords.isEmpty() && !rhymeVerses.isEmpty()) {
                qDebug() << "Выбрана пустая считалка после выбывания.";
            }
            rhymeWordIndex = 0;
            processNextWord();
        } else if (persons.size() == 1) {
            crownWinner();
        } else {
            wordLabel->setText("Все выбыли!");
            nextWordButton->setEnabled(true);
        }
        m_isEliminationInProgress = false;
    });
}

void MainWindow::crownWinner() {
    if (persons.size() != 1 || photoItems.isEmpty()) {
        return;
    }

    m_isEliminationInProgress = true;
    nextWordButton->setEnabled(false);
    tts->stop();

    PhotoItem *winnerItem = photoItems[0];
    if (winnerItem && winnerItem->nameLabel) {
        winnerItem->nameLabel->setText(persons[0].fullName + "<br><font color='green'>👑 ПОБЕДИТЕЛЬ!</font>");
        winnerItem->nameLabel->setStyleSheet("color: black; font: bold 14pt;");
        winnerItem->container->setGeometry(400, 300, 200, 240);
        if (!persons[0].photo.isNull()) {
            winnerItem->photoLabel->setPixmap(persons[0].photo.scaled(180, 180, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
    wordLabel->setText("");

    QString fireworksPath = QCoreApplication::applicationDirPath() + "/fireworks.wav";
    if (!QFile::exists(fireworksPath)) {
        qDebug() << "Файл звука победителя не найден:" << fireworksPath;
        return;
    }
    fireworksPlayer->setSource(QUrl::fromLocalFile(fireworksPath));
    audioOutput->setVolume(0.7);
    if (fireworksPlayer->error() == QMediaPlayer::NoError) {
        fireworksPlayer->play();
    } else {
        qDebug() << "Ошибка воспроизведения звука фейерверков:" << fireworksPlayer->errorString();
    }

}
