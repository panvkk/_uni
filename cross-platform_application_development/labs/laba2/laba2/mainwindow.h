#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVector>
#include "Date.h" // Подключаем ваш класс Date

// Виджет для отрисовки графика биоритмов
class GraphWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GraphWidget(QWidget *parent = nullptr);
    // Устанавливаем дату рождения и дату расчёта
    void setDates(const Date& birthDate, const Date& calcDate);
    // Возвращает текстовое описание центрального дня с процентами
    QString getCentralBiorhythms() const;

signals:
    // Сигнал, посылаемый при клике на дату
    void dateClicked(Date clickedDate);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    bool m_dataAvailable;
    Date m_birthDate;
    Date m_calcDate;
    QVector<double> m_physical;     // физический цикл
    QVector<double> m_emotional;    // эмоциональный цикл
    QVector<double> m_intellectual; // интеллектуальный цикл
    QVector<Date> m_dates;          // точки графика (например, 61 точка)

    // Вычисление значений биоритмов по датам в m_dates
    void calculateBiorhythms();
};


// Главное окно программы
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Режимы ввода даты
    enum InputMode { ClassicMode, TextualMode };

private slots:
    void onSolveClicked();
    void onMenuFormat();
    void setInputModeClassic();
    void setInputModeTextual();
    // Слот, реагирующий на выбор даты по клику на графике
    void onDateClicked(Date newCalcDate);

private:
    QLineEdit* birthDateEdit;
    QLineEdit* calcDateEdit;
    QPushButton* solveButton;
    GraphWidget* graphWidget;
    QLabel* infoLabel;
    QLabel* resultLabel;  // Для отображения значений центрального дня

    // Текущий выбранный режим ввода даты
    InputMode inputMode;
    // Сохранённая дата рождения, полученная при нажатии "Solve"
    Date currentBirthDate;

    void createMenus();

    // Функции для парсинга даты в двух режимах
    Date parseClassicDate(const QString &input);
    Date parseTextualDate(const QString &input);
};

#endif // MAINWINDOW_H
