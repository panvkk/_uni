#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QInputDialog>
#include <QPainter>
#include <QPaintEvent>
#include <QtMath>
#include <sstream>
#include <QRegularExpression>
#include <QMap>
#include <QMouseEvent>
#include <QFontMetrics>

// =======================
// GraphWidget
// =======================
GraphWidget::GraphWidget(QWidget *parent)
    : QWidget(parent), m_dataAvailable(false)
{
    setMinimumSize(600, 400);
    // Дополнительно можно включить явное обновление всего содержимого
    //setAttribute(Qt::WA_OpaquePaintEvent, true);
}

void GraphWidget::setDates(const Date& birthDate, const Date& calcDate)
{
    m_birthDate = birthDate;
    m_calcDate = calcDate;

    // Строим точки графика для диапазона ±30 дней от calcDate (всего 61 точка)
    m_dates.clear();
    m_physical.clear();
    m_emotional.clear();
    m_intellectual.clear();
    const int range = 30;
    for (int i = -range; i <= range; i++){
        Date currentDate = calcDate + i; // Используется перегруженный оператор +
        m_dates.push_back(currentDate);
    }
    calculateBiorhythms();
    m_dataAvailable = true;
    update(); // Перерисовать виджет
}

void GraphWidget::calculateBiorhythms()
{
    m_physical.clear();
    m_emotional.clear();
    m_intellectual.clear();

    // Для каждой даты вычисляем разницу в днях между текущей датой и датой рождения.
    // Функция биоритмов: sin(2π * diff / период)
    for (const Date &d : m_dates) {
        int diff = d - m_birthDate; // Разница в днях
        double physical    = qSin(2 * M_PI * diff / 23.0);
        double emotional   = qSin(2 * M_PI * diff / 28.0);
        double intellectual= qSin(2 * M_PI * diff / 33.0);
        m_physical.push_back(physical);
        m_emotional.push_back(emotional);
        m_intellectual.push_back(intellectual);
    }
}

QString GraphWidget::getCentralBiorhythms() const
{
    if (!m_dataAvailable || m_dates.isEmpty())
        return "Нет данных.";

    int center = m_dates.size() / 2;
    std::ostringstream oss;
    oss.precision(1);
    oss << std::fixed;
    oss << "Дата расчёта: " << m_dates[center] << "\n"
        << "Физический: "    << m_physical[center] * 100 << "%\n"
        << "Эмоциональный: " << m_emotional[center] * 100 << "%\n"
        << "Интеллектуальный: " << m_intellectual[center] * 100 << "%";
    return QString::fromStdString(oss.str());
}

void GraphWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(event->rect(), Qt::white);

    int w = width();
    int h = height();
    int margin = 40;
    int availableWidth = w - 2 * margin;

    // Рисуем оси
    painter.setPen(Qt::black);
    painter.drawLine(margin, h/2, w - margin, h/2);    // Ось X
    painter.drawLine(margin, margin, margin, h - margin); // Ось Y

    if (!m_dataAvailable)
    {
        painter.drawText(rect(), Qt::AlignCenter, "Нажмите Solve для построения графика");
        return;
    }

    // Используем весь набор данных для построения кривых
    const int dataCount = m_dates.size(); // например, 61 точка
    double xStep = availableWidth / double(dataCount - 1);
    double yScale = (h - 2 * margin) / 2.0;

    // Отрисовка вертикальных пунктирных линий и подписей – оставляем только для подмножества,
    // чтобы надписи не накладывались.
    int minLabelSpacing = 50;
    int labelStep = qMax(1, int(qCeil(minLabelSpacing / xStep)));
    QPen dashPen(Qt::DashLine);
    dashPen.setColor(Qt::lightGray);
    painter.setPen(dashPen);
    QFont baseFont = painter.font();
    for (int j = 0; j < dataCount; j++)
    {
        double x = margin + j * xStep;
        if(j % labelStep == 0)
        {
            painter.drawLine(QPointF(x, margin), QPointF(x, h - margin));
            // Формирование подписи в виде строки(date)
            std::ostringstream oss;
            oss << m_dates[j];
            QString dateStr = QString::fromStdString(oss.str());
            // Если ширина имени больше, чем отведённое пространство, уменьшаем размер шрифта
            QFontMetrics fm(baseFont);
            int textWidth = fm.horizontalAdvance(dateStr);
            QFont drawFont = baseFont;
            if(textWidth > xStep)
            {
                int newPointSize = int(baseFont.pointSize() * (xStep / double(textWidth)));
                newPointSize = qMax(newPointSize, 6);
                drawFont.setPointSize(newPointSize);
            }
            painter.save();
            painter.setFont(drawFont);
            painter.setPen(Qt::black);
            painter.drawText(QPointF(x - 20, h - margin + 15), dateStr);
            painter.restore();
            painter.setPen(dashPen);
        }
    }

    // Формируем QPolygonF для трех кривых, что обеспечивает непрерывность линии
    QPolygonF polyPhys, polyEmot, polyIntel;
    for (int j = 0; j < dataCount; j++)
    {
        double x = margin + j * xStep;
        double yPhys = h/2 - m_physical[j] * yScale;
        double yEmot = h/2 - m_emotional[j] * yScale;
        double yIntel = h/2 - m_intellectual[j] * yScale;
        polyPhys << QPointF(x, yPhys);
        polyEmot << QPointF(x, yEmot);
        polyIntel << QPointF(x, yIntel);
    }

    // Устанавливаем перья с округлёнными концами и соединениями для сглаживания
    QPen penPhys(QColor(0, 0, 255), 2);
    penPhys.setCapStyle(Qt::RoundCap);
    penPhys.setJoinStyle(Qt::RoundJoin);

    QPen penEmot(QColor(255, 0, 0), 2);
    penEmot.setCapStyle(Qt::RoundCap);
    penEmot.setJoinStyle(Qt::RoundJoin);

    QPen penIntel(QColor(0, 128, 0), 2);
    penIntel.setCapStyle(Qt::RoundCap);
    penIntel.setJoinStyle(Qt::RoundJoin);

    // Отрисовываем кривые плавно одной командой
    painter.setPen(penPhys);
    painter.drawPolyline(polyPhys);
    painter.setPen(penEmot);
    painter.drawPolyline(polyEmot);
    painter.setPen(penIntel);
    painter.drawPolyline(polyIntel);

    // Отрисовка кризисных дней: если хоть один показатель по модулю менее порога
    double crisisThreshold = 0.1;
    painter.setBrush(Qt::red);
    for (int j = 0; j < dataCount; j++)
    {
        bool crisis = (qAbs(m_physical[j]) < crisisThreshold) ||
                      (qAbs(m_emotional[j]) < crisisThreshold) ||
                      (qAbs(m_intellectual[j]) < crisisThreshold);
        if (crisis)
        {
            double x = margin + j * xStep;
            QPointF pt(x, h/2);
            painter.drawEllipse(pt, 5, 5);
        }
    }

    // Отрисовка легенды. Вычисляем прямоугольник, позволяющий перенос строк,
    // и добавляем отступы, чтобы весь текст помещался.
    QString legendText =
        "Синий: Физический\n"
        "Красный: Эмоциональный\n"
        "Зелёный: Интеллектуальный\n"
        "Красная точка: Кризисный день";
    QFont legendFont = baseFont;
    QFontMetrics lm(legendFont);
    QRect legendRect = lm.boundingRect(0, 0, 300, 100, Qt::TextWordWrap, legendText);
    legendRect.adjust(-5, -5, 5, 5);
    legendRect.moveTopRight(QPoint(w - margin, margin));
    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(legendRect);
    painter.drawText(legendRect, Qt::AlignLeft | Qt::AlignTop, legendText);
}


void GraphWidget::mousePressEvent(QMouseEvent *event)
{
    if (!m_dataAvailable)
        return;
    int w = width();
    int margin = 40;
    int availableWidth = w - 2 * margin;
    int dataCount = m_dates.size();
    double xStep = availableWidth / double(dataCount - 1);

    double clickX = event->x();
    double minDiff = 1e9;
    int selectedIndex = -1;
    for (int j = 0; j < dataCount; j++){
        double x = margin + j * xStep;
        double diff = fabs(x - clickX);
        if (diff < minDiff){
            minDiff = diff;
            selectedIndex = j;
        }
    }
    // Если клик не около одной из точек (расстояние более 20 пикселей) – игнорируем.
    if (minDiff > 20)
        return;
    Date clickedDate = m_dates[selectedIndex];
    emit dateClicked(clickedDate);
}

// =======================
// MainWindow
// =======================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), inputMode(ClassicMode)
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Поля ввода дат
    QLabel *birthLabel = new QLabel("Дата рождения:", this);
    birthDateEdit = new QLineEdit(this);
    birthDateEdit->setPlaceholderText("например, 29.02.2020");
    birthDateEdit->setText("29.02.2020");

    QLabel *calcLabel = new QLabel("Дата расчёта:", this);
    calcDateEdit = new QLineEdit(this);
    calcDateEdit->setPlaceholderText("например, 15.03.2020");
    calcDateEdit->setText("15.03.2020");

    solveButton = new QPushButton("Solve", this);
    connect(solveButton, &QPushButton::clicked, this, &MainWindow::onSolveClicked);

    infoLabel = new QLabel("Введите даты и нажмите Solve для построения графика", this);
    resultLabel = new QLabel("", this);
    resultLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    graphWidget = new GraphWidget(this);
    // При клике по графику обновляем график для выбранной даты (с датой рождения, полученной в Solve)
    connect(graphWidget, &GraphWidget::dateClicked, this, &MainWindow::onDateClicked);

    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->addWidget(birthLabel);
    inputLayout->addWidget(birthDateEdit);
    inputLayout->addWidget(calcLabel);
    inputLayout->addWidget(calcDateEdit);
    inputLayout->addWidget(solveButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(inputLayout);
    mainLayout->addWidget(infoLabel);
    mainLayout->addWidget(graphWidget);
    mainLayout->addWidget(resultLabel);

    centralWidget->setLayout(mainLayout);

    createMenus();
}

MainWindow::~MainWindow()
{
}

void MainWindow::createMenus()
{
    // Меню Файл
    QMenu *fileMenu = menuBar()->addMenu("Файл");
    QAction *exitAction = fileMenu->addAction("Выход");
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

    // Меню Настройки
    QMenu *settingsMenu = menuBar()->addMenu("Настройки");
    // Вместо отдельного меню "Ввод" теперь добавляем один пункт "Формат ввода дат"
    QAction *formatDateAction = settingsMenu->addAction("Формат ввода дат");
    connect(formatDateAction, &QAction::triggered, this, &MainWindow::onMenuFormat);
}

Date MainWindow::parseClassicDate(const QString &input)
{
    QStringList parts = input.trimmed().split('.', Qt::SkipEmptyParts);
    if (parts.size() != 3)  throw QString("Неверный формат даты. Ожидается dd.MM.yyyy");

    bool ok1, ok2, ok3;
    int d = parts[0].toInt(&ok1);
    int m = parts[1].toInt(&ok2);
    //  leading zeros (&laquo;0000&raquo;) конвертируются в 0 — это нормально
    int y = parts[2].toInt(&ok3);

    if (!ok1 || !ok2 || !ok3)           throw QString("Неверные числовые значения в дате.");
    if (d < 1 || d > 31 || m < 1 || m > 12)  throw QString("Такого дня или месяца не существует.");

    return Date(d, m, y);    //  год 0 допускается
}

// =====================================================================

static const QMap<QString,int> monthMap = {
    {"января", 1}, {"январь", 1},
    {"февраля", 2}, {"февраль", 2},
    {"марта", 3}, {"март", 3},
    {"апреля", 4}, {"апрель", 4},
    {"мая", 5},
    {"июня", 6}, {"июнь", 6},
    {"июля", 7}, {"июль", 7},
    {"августа", 8}, {"август", 8},
    {"сентября", 9}, {"сентябрь", 9},
    {"октября", 10}, {"октябрь", 10},
    {"ноября", 11}, {"ноябрь", 11},
    {"декабря", 12}, {"декабрь", 12}
};

static const QMap<QString,int> ordinalMap = {
    /* 0–20 */ {"нулевого",0},{"первого",1},{"второго",2},{"третьего",3},
    {"четвертого",4},{"пятого",5},{"шестого",6},{"седьмого",7},
    {"восьмого",8},{"девятого",9},{"десятого",10},{"одиннадцатого",11},
    {"двенадцатого",12},{"тринадцатого",13},{"четырнадцатого",14},
    {"пятнадцатого",15},{"шестнадцатого",16},{"семнадцатого",17},
    {"восемнадцатого",18},{"девятнадцатого",19},{"двадцатого",20},
    /* 21‑31 */
    {"двадцать первого",21},{"двадцать второго",22},{"двадцать третьего",23},
    {"двадцать четвертого",24},{"двадцать пятого",25},{"двадцать шестого",26},
    {"двадцать седьмого",27},{"двадцать восьмого",28},{"двадцать девятого",29},
    {"тридцатого",30},{"тридцать первого",31},
    /* <добавьте при желании 32–99> */
};

static const QMap<QString,int> cardinalMap = {
    {"ноль",0},{"один",1},{"два",2},{"две",2},{"три",3},{"четыре",4},{"пять",5},
    {"шесть",6},{"семь",7},{"восемь",8},{"девять",9},{"десять",10},
    {"одиннадцать",11},{"двенадцать",12},{"тринадцать",13},{"четырнадцать",14},
    {"пятнадцать",15},{"шестнадцать",16},{"семнадцать",17},{"восемнадцать",18},
    {"девятнадцать",19},{"двадцать",20},{"тридцать",30},{"сорок",40},{"пятьдесят",50},
    {"шестьдесят",60},{"семьдесят",70},{"восемьдесят",80},{"девяносто",90}
};
// =====================================================================

///  вспомогательная &laquo;сложилка&raquo; кардинальных числительных (до 99)
auto tokensToInt = [](const QStringList &toks)->int {
    int result = 0;
    for (const QString &w : toks) {
        if (!cardinalMap.contains(w)) return -1;
        result += cardinalMap[w];
    }
    return result;
};

Date MainWindow::parseTextualDate(const QString &input)
{
    QStringList tok = input.trimmed().toLower()
    .replace(QRegularExpression("[,\\.]"),"")  // убираем запятые/точки
        .split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

    // ---------- день (слово или два) ---------------------------------
    int pos = 0;
    QString dayPhrase = tok[pos];
    if (pos+1 < tok.size() && ordinalMap.contains(dayPhrase + " " + tok[pos+1])) {
        dayPhrase += " " + tok[++pos];
    }
    if (!ordinalMap.contains(dayPhrase))
        throw QString("Неверный день: ") + dayPhrase;
    int d = ordinalMap[dayPhrase];
    pos++;

    // ---------- месяц -------------------------------------------------
    if (pos >= tok.size() || !monthMap.contains(tok[pos]))
        throw QString("Неверный месяц: ") + tok.value(pos);
    int m = monthMap[tok[pos++]];

    // ---------- год ---------------------------------------------------
    if (pos >= tok.size()) throw QString("Отсутствует годовая часть.");
    // убираем &laquo;года / году / год&raquo;
    if (tok.back() == "года" || tok.back() == "году" || tok.back() == "год")
        tok.removeLast();

    //  поддерживаем две большие схемы:
    //  1) &laquo;две тысячи <N>&raquo;   &rarr; 2000 + N
    //  2) &laquo;тысяча девятьсот <N>&raquo; &rarr; 1900 + N
    int y = -1;
    if (tok[pos] == "две" && pos+1 < tok.size() && tok[pos+1] == "тысячи") {
        pos += 2;
        int tail = tokensToInt(tok.mid(pos));
        if (tail < 0 || tail > 99)  throw QString("Непонятный остаток после 'две тысячи'");
        y = 2000 + tail;
    }
    else if (tok[pos] == "тысяча" && pos+1 < tok.size()) {
        static const QMap<QString,int> century = {
            {"девятьсот", 900}, {"восемьсот", 800}, {"семьсот", 700}, {"шестьсот", 600}
            /* доп. столетия при необходимости */
        };
        if (!century.contains(tok[pos+1]))
            throw QString("Ожидается столетие после 'тысяча'");
        int base   = century[tok[pos+1]];
        int tail   = tokensToInt(tok.mid(pos+2));
        if (tail < 0 || tail > 99)  throw QString("Непонятный остаток после столетия");
        y = 1000 + base + tail;
    }
    else {
        //  пробуем простую форму &laquo;2025&raquo;
        bool ok;
        y = tok[pos].toInt(&ok);
        if (!ok || y < 0) throw QString("Неверный год.");
    }

    return Date(d, m, y);
}

void MainWindow::onSolveClicked()
{
    try {
        Date birthDate, calcDate;
        if(inputMode == ClassicMode) {
            birthDate = parseClassicDate(birthDateEdit->text());
            calcDate  = parseClassicDate(calcDateEdit->text());
        } else { // TextualMode
            birthDate = parseTextualDate(birthDateEdit->text());
            calcDate  = parseTextualDate(calcDateEdit->text());
        }
        currentBirthDate = birthDate; // сохраняем дату рождения для последующих операций
        graphWidget->setDates(birthDate, calcDate);
        QString resultStr = graphWidget->getCentralBiorhythms();
        resultLabel->setText(resultStr);
        infoLabel->setText("График биоритмов построен.");
    }
    catch(const QString &err) {
        QMessageBox::critical(this, "Ошибка", err);
    }
    catch(...) {
        QMessageBox::critical(this, "Ошибка", "Непредвиденная ошибка при разборе даты.");
    }
}

void MainWindow::onMenuFormat()
{
    // Используем QInputDialog для выбора формата ввода дат.
    QStringList options;
    options << "Классический" << "Словесный";
    bool ok;
    // Выбор по умолчанию – 0 (Классический) если сейчас ClassicMode, иначе 1.
    int current = (inputMode == ClassicMode) ? 0 : 1;
    QString choice = QInputDialog::getItem(this, "Формат ввода дат",
                                           "Выберите формат ввода дат:",
                                           options, current, false, &ok);
    if(ok && !choice.isEmpty()){
        if(choice == "Классический"){
            setInputModeClassic();
        } else {
            setInputModeTextual();
        }
        QMessageBox::information(this, "Подтверждение", "Формат ввода дат изменен на: " + choice);
    }
}

void MainWindow::setInputModeClassic()
{
    inputMode = ClassicMode;
    birthDateEdit->setPlaceholderText("например, 29.02.2020");
    calcDateEdit->setPlaceholderText("например, 15.03.2020");
}

void MainWindow::setInputModeTextual()
{
    inputMode = TextualMode;
    birthDateEdit->setPlaceholderText("например, первое января две тысячи первого года");
    calcDateEdit->setPlaceholderText("например, двадцать пятое марта две тысячи второго года");
}

void MainWindow::onDateClicked(Date newCalcDate)
{
    // Обновляем график с сохранённой датой рождения и выбранной датой
    graphWidget->setDates(currentBirthDate, newCalcDate);
    QString resultStr = graphWidget->getCentralBiorhythms();
    resultLabel->setText(resultStr);
    infoLabel->setText("График обновлён для выбранной даты");
    graphWidget->update();
}
