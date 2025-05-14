#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QLabel>
#include <QPushButton>
#include <QTextToSpeech>
#include <QMediaPlayer> // Для QMediaPlayer
#include <QAudioOutput> // Для управления звуком
#include "person.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    bool m_isEliminationInProgress;
    int rhymeWordIndex;
    int currentPersonIndex;
    struct PhotoItem {
        QWidget *container;
        QLabel *photoLabel;
        QLabel *nameLabel;
    };
    QVector<QString> rhymeVerses;
    QVector<QString> currentRhymeWords;
    QString longestWord;
    QVector<Person> persons;
    QVector<PhotoItem*> photoItems;
    PhotoItem *eliminatedItem;
    QLabel *wordLabel;
    QLabel *longestWordDisplayLabel;
    QPushButton *nextWordButton;
    QTextToSpeech *tts;
    QMediaPlayer *fireworksPlayer; // Игрок для звука
    QAudioOutput *audioOutput;     // Выход аудио для управления громкостью

    void loadRhyme();
    void loadPersons();
    void arrangePhotos();
    void updateLongestWordDisplay();
    void startGame();
    void processNextWord();
    void removeCurrentPerson();
    void crownWinner();
};

#endif // MAINWINDOW_H
