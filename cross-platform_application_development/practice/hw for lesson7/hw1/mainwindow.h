#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void calculateDifference();

private:
    QLineEdit *dateInput1;      // Поле ввода первой даты
    QLineEdit *dateInput2;      // Поле ввода второй даты
    QLabel *textDate1;          // Текстовое представление первой даты
    QLabel *textDate2;          // Текстовое представление второй даты
    QPushButton *calculateButton; // Кнопка для вычисления разности
    QLabel *differenceLabel;    // Отображение разности в днях

    QString dateToText(const QString &dateStr); // Преобразование даты в текст
    bool parseDate(const QString &dateStr, int &day, int &month, int &year); // Парсинг строки даты
};

#endif // MAINWINDOW_H
