#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "person.h"
#include <QLabel>
#include <QPushButton>
#include <QTextToSpeech>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Главное окно приложения, в котором происходит вся логика игры
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // Методы загрузки данных и обновления расположения фотографий
    void loadRhyme();      // считывание считалок из текстового файла (rhyme.txt)
    void loadPersons();    // загрузка фотографий из папки "photo" и треков из "tracks"
    void arrangePhotos();  // расположение фотографий по кругу на экране

    // Методы работы считалки
    void nextWord();       // переход к следующему слову (вызывается кнопкой)
    void removeCurrentPerson(); // удаление (анимация «выбегания») текущей фотографии, когда считалка завершается
    void crownWinner();    // оформление победителя (один оставшийся студент)

    // Вспомогательные переменные для работы со считалкой
    QVector<QString> rhymeVerses;         // перечень считанных считалок
    QVector<QString> currentRhymeWords;     // выбранная считалка разбитая на слова
    QString longestWord;                  // самое длинное слово из выбранной считалки
    int rhymeWordIndex;                   // индекс текущего слова в считалке

    // Список участников и соответствующих виджетов (фотографий)
    QVector<Person> persons;
    QVector<QLabel*> photoLabels;
    int currentPersonIndex;               // индекс текущего участника (цилический список)

    // Элементы интерфейса
    QLabel *wordLabel;        // ярлык для показа текущего слова (в центре экрана)
    QLabel *longestWordLabel; // ярлык для вывода самого длинного слова (в правом верхнем углу)
    QPushButton *nextWordButton; // кнопка для перехода к следующему слову

    // Озвучка слов
    QTextToSpeech *tts;
};

#endif // MAINWINDOW_H
