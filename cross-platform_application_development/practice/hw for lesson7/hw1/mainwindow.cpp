#include "mainwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include "Date.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Создаем центральный виджет и вертикальный layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // Поле ввода первой даты
    dateInput1 = new QLineEdit(this);
    dateInput1->setPlaceholderText("Введите первую дату (dd.mm.yyyy)");
    layout->addWidget(dateInput1);

    // Метка для текстового представления первой даты
    textDate1 = new QLabel(this);
    layout->addWidget(textDate1);

    // Поле ввода второй даты
    dateInput2 = new QLineEdit(this);
    dateInput2->setPlaceholderText("Введите вторую дату (dd.mm.yyyy)");
    layout->addWidget(dateInput2);

    // Метка для текстового представления второй даты
    textDate2 = new QLabel(this);
    layout->addWidget(textDate2);

    // Кнопка для вычисления разности
    calculateButton = new QPushButton("Вычислить разность", this);
    layout->addWidget(calculateButton);

    // Метка для отображения разности
    differenceLabel = new QLabel(this);
    layout->addWidget(differenceLabel);

    // Устанавливаем центральный виджет
    setCentralWidget(centralWidget);

    // Подключаем сигналы и слоты
    connect(calculateButton, &QPushButton::clicked, this, &MainWindow::calculateDifference);
    connect(dateInput1, &QLineEdit::textChanged, [this](const QString &text) {
        textDate1->setText(dateToText(text));
    });
    connect(dateInput2, &QLineEdit::textChanged, [this](const QString &text) {
        textDate2->setText(dateToText(text));
    });
}

MainWindow::~MainWindow()
{
}

void MainWindow::calculateDifference()
{
    QString dateStr1 = dateInput1->text();
    QString dateStr2 = dateInput2->text();

    int day1, month1, year1;
    int day2, month2, year2;

    // Проверяем корректность введенных дат
    if (!parseDate(dateStr1, day1, month1, year1) || !parseDate(dateStr2, day2, month2, year2)) {
        if(!parseDate(dateStr1, day1, month1, year1) && !parseDate(dateStr2, day2, month2, year2)) {
            differenceLabel->setText("Ошибка: неверный формат даты в обеих полях.");
        } else if(!parseDate(dateStr1, day1, month1, year1)) {
            differenceLabel->setText("Ошибка: неверный формат даты в 1 поле.");
        } else {
            differenceLabel->setText("Ошибка: неверный формат даты во 2 поле.");
        }
        return;
    }

    try {
        // Создаем объекты Date
        Date date1(day1, month1, year1);
        Date date2(day2, month2, year2);
        // Вычисляем разность в днях
        int difference = date1 - date2;
        differenceLabel->setText(QString("Разность: %1 дней").arg(qAbs(difference)));
    } catch (const std::exception &e) {
        differenceLabel->setText(QString("Ошибка: %1").arg(e.what()));
    }
}

QString MainWindow::dateToText(const QString &dateStr)
{
    int day, month, year;
    if (!parseDate(dateStr, day, month, year)) {
        return "Неверная дата";
    }

    // Массивы для текстового представления
    const QString months[] = {
        "января", "февраля", "марта", "апреля", "мая", "июня",
        "июля", "августа", "сентября", "октября", "ноября", "декабря"
    };

    const QString days[] = {
        "первое", "второе", "третье", "четвертое", "пятое", "шестое", "седьмое", "восьмое", "девятое", "десятое",
        "одиннадцатое", "двенадцатое", "тринадцатое", "четырнадцатое", "пятнадцатое", "шестнадцатое", "семнадцатое",
        "восемнадцатое", "девятнадцатое", "двадцатое", "двадцать первое", "двадцать второе", "двадцать третье",
        "двадцать четвертое", "двадцать пятое", "двадцать шестое", "двадцать седьмое", "двадцать восьмое",
        "двадцать девятое", "тридцатое", "тридцать первое"
    };

    if (day < 1 || day > 31 || month < 1 || month > 12) {
        return "Неверная дата";
    }

    return QString("%1 %2 %3 года").arg(days[day - 1]).arg(months[month - 1]).arg(year);
}

bool MainWindow::parseDate(const QString &dateStr, int &day, int &month, int &year)
{
    QStringList parts = dateStr.split('.');
    if (parts.size() != 3) {
        return false;
    }

    bool ok;
    day = parts[0].toInt(&ok);
    if (!ok || day < 1 || day > 31) return false;

    month = parts[1].toInt(&ok);
    if (!ok || month < 1 || month > 12) return false;

    year = parts[2].toInt(&ok);
    if (!ok || year < 1) return false;

    return true;
}
