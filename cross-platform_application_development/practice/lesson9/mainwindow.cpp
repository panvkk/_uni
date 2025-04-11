#include "mainwindow.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath> // Для sin() и константы M_PI

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Устанавливаем размеры окна и заголовок.
    resize(800, 600);
    setWindowTitle("График функции sin(x)");
}

MainWindow::~MainWindow() { }

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Задаём чёрный фон.
    painter.fillRect(rect(), Qt::black);

    // Вводим отступ и запас для стрелок.
    int margin = 20;   // внешний отступ
    int arrowSize = 10; // размер стрелочных "лапок"

    // Вместо использования всего окна, задаём область для отрисовки осей так,
    // чтобы на краях оставить место для стрелок и подписей.
    int leftDraw   = margin + arrowSize;
    int rightDraw  = width() - margin - arrowSize;
    int topDraw    = margin + arrowSize;
    int bottomDraw = height() - margin - arrowSize;

    double widthEffective  = rightDraw - leftDraw;
    double heightEffective = bottomDraw - topDraw;

    // Задаём математические границы координат.
    double xMin = -M_PI, xMax = M_PI;
    double yMin = -1.0,  yMax = 1.0;

    // Функция переобразования:
    // Для x: pixelX = leftDraw + ((x - xMin)/(xMax - xMin)) * widthEffective.
    // Для y: pixelY = topDraw + ((yMax - y)/(yMax - yMin)) * heightEffective
    // (т.к. в системе координат Qt ось Y направлена вниз).
    double originX = leftDraw + ((0 - xMin) / (xMax - xMin)) * widthEffective;
    double originY = topDraw + ((yMax - 0) / (yMax - yMin)) * heightEffective;

    // Устанавливаем белый цвет для осей, тиков и подписей.
    QPen axisPen(Qt::white, 2);
    painter.setPen(axisPen);

    // Рисуем оси:
    // Горизонтальная ось X от x = -π до π
    painter.drawLine(QPointF(leftDraw, originY), QPointF(rightDraw, originY));
    // Вертикальная ось Y от y = -1 до 1
    painter.drawLine(QPointF(originX, bottomDraw), QPointF(originX, topDraw));

    // Рисуем стрелки для осей.
    // Для оси X:
    // Правый конец (x = π).
    QPointF tipRight(rightDraw, originY);
    QPointF baseRight1(rightDraw - arrowSize, originY - arrowSize/2.0);
    QPointF baseRight2(rightDraw - arrowSize, originY + arrowSize/2.0);
    painter.drawLine(tipRight, baseRight1);
    painter.drawLine(tipRight, baseRight2);

    // Для оси Y:
    // Верхний конец (y = 1).
    QPointF tipTop(originX, topDraw);
    QPointF baseTop1(originX - arrowSize/2.0, topDraw + arrowSize);
    QPointF baseTop2(originX + arrowSize/2.0, topDraw + arrowSize);
    painter.drawLine(tipTop, baseTop1);
    painter.drawLine(tipTop, baseTop2);

    // Рисуем тик-метки и подписи на оси X.
    int tickLength = 5;
    QFont font = painter.font();
    font.setPointSize(10);
    painter.setFont(font);
    // Задаём набор значений для оси X: -π, -π/2, 0, π/2, π.
    QList<double> xTicks { -M_PI, -M_PI/2, 0, M_PI/2, M_PI };
    for (double xVal : xTicks) {
        double pixelX = leftDraw + ((xVal - xMin) / (xMax - xMin)) * widthEffective;
        painter.drawLine(QPointF(pixelX, originY - tickLength),
                         QPointF(pixelX, originY + tickLength));
        // Формируем текстовую метку.
        QString label;
        if (qFuzzyCompare(xVal, -M_PI))
            label = QStringLiteral("-π");
        else if (qFuzzyCompare(xVal, -M_PI/2))
            label = QStringLiteral("-π/2");
        else if (qFuzzyCompare(xVal, 0.0))
            label = QStringLiteral("0");
        else if (qFuzzyCompare(xVal, M_PI/2))
            label = QStringLiteral("π/2");
        else if (qFuzzyCompare(xVal, M_PI))
            label = QStringLiteral("π");
        else
            label = QString::number(xVal, 'f', 2);
        // Рисуем подпись – смещаем текст немного относительно тика.
        painter.drawText(QPointF(pixelX - 10, originY + 20), label);
    }

    // Рисуем тик-метки и подписи на оси Y.
    QList<double> yTicks { 1.0, 0.0, -1.0 };
    for (double yVal : yTicks) {
        double pixelY = topDraw + ((yMax - yVal) / (yMax - yMin)) * heightEffective;
        painter.drawLine(QPointF(originX - tickLength, pixelY),
                         QPointF(originX + tickLength, pixelY));
        QString label = QString::number(yVal, 'f', 1);
        // Смещаем подпись влево от тика.
        painter.drawText(QPointF(originX - 30, pixelY + 5), label);
    }

    // Рисуем график функции sin(x) по области от -π до π.
    QPen graphPen(Qt::white, 2);
    painter.setPen(graphPen);
    QPainterPath path;
    bool firstPoint = true;
    // Итерируем по горизонтальным пикселям в нашей области от leftDraw до rightDraw.
    for (int pixelX = leftDraw; pixelX <= rightDraw; ++pixelX) {
        // Обратное преобразование: из pixelX получаем значение x в интервале [xMin, xMax].
        double xVal = xMin + ((pixelX - leftDraw) / widthEffective) * (xMax - xMin);
        double yVal = sin(xVal);
        double pixelY = topDraw + ((yMax - yVal) / (yMax - yMin)) * heightEffective;
        if (firstPoint) {
            path.moveTo(pixelX, pixelY);
            firstPoint = false;
        } else {
            path.lineTo(pixelX, pixelY);
        }
    }
    painter.drawPath(path);
}
