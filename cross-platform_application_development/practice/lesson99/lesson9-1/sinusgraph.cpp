#include <iostream>
#include <vector>
#include <string>
#define _USE_MATH_DEFINES 
#include <cmath>
#include <cstdio>
#include <cstring>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/ioctl.h>
    #include <unistd.h>
#endif

// Функция для получения размеров терминала (число столбцов и строк)
void getTerminalSize(int &cols, int &rows) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    } else {
        cols = 80;
        rows = 24;
    }
#else
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        cols = 80;
        rows = 24;
    } else {
        cols = w.ws_col;
        rows = w.ws_row;
    }
#endif
}

int main() {
    int termWidth, termHeight;
    getTerminalSize(termWidth, termHeight);

    // Определяем отступы, чтобы оставить место для подписей
    int leftMargin = 8;    // для подписей по оси Y
    int rightMargin = 2;
    int topMargin = 1;     // для стрелок оси Y
    int bottomMargin = 2;  // для подписей оси X

    int plotWidth = termWidth - leftMargin - rightMargin;
    int plotHeight = termHeight - topMargin - bottomMargin;

    // Создаём буфер — вектор строк, по одной строке на каждую строку экрана
    std::vector<std::string> buffer(termHeight, std::string(termWidth, ' '));

    // Очистка экрана, установка курсора в начало и применение ANSI-кодов:
    // черный фон (40) и белый текст (37)
    std::cout << "\033[2J\033[H" << "\033[40m\033[37m";

    // Математическая система координат: x ∈ [-π, π], y ∈ [-1, 1].
    // Преобразование:
    //   pixelX = leftMargin + ((x + π) / (2π)) * (plotWidth - 1)
    //   pixelY = topMargin + ((1 - y) / 2) * (plotHeight - 1)
    //
    // Вычисляем положение осей: для y=0 и x=0
    int xAxisRow = topMargin + (int)(0.5 * (plotHeight - 1)); // соответствует y = 0
    int yAxisCol = leftMargin + (int)(0.5 * (plotWidth - 1));   // соответствует x = 0

    // Рисуем ось X (горизонтальную) внутри области графика
    for (int col = leftMargin; col < leftMargin + plotWidth; col++) {
        buffer[xAxisRow][col] = '-';
    }
    // Рисуем ось Y (вертикальную)
    for (int row = topMargin; row < topMargin + plotHeight; row++) {
        buffer[row][yAxisCol] = '|';
    }

    // Добавляем стрелки на концах осей:
    // Для оси X: слева "<", справа ">"
    buffer[xAxisRow][leftMargin] = '<';
    buffer[xAxisRow][leftMargin + plotWidth - 1] = '>';
    // Для оси Y: сверху "^", снизу "v"
    buffer[topMargin][yAxisCol] = '^';
    buffer[topMargin + plotHeight - 1][yAxisCol] = 'v';

    // Тик-метки и подписи для оси X (значения: -π, -π/2, 0, π/2, π)
    double xTicks[] = { -M_PI, -M_PI/2, 0, M_PI/2, M_PI };
    for (double xt : xTicks) {
        int col = leftMargin + (int)(((xt + M_PI) / (2.0 * M_PI)) * (plotWidth - 1));
        buffer[xAxisRow][col] = '+'; // ставим тик
        // Формируем строковую метку
        char label[16];
        if (fabs(xt + M_PI) < 1e-6)
            snprintf(label, sizeof(label), "-π");
        else if (fabs(xt + M_PI/2) < 1e-6)
            snprintf(label, sizeof(label), "-π/2");
        else if (fabs(xt) < 1e-6)
            snprintf(label, sizeof(label), "0");
        else if (fabs(xt - M_PI/2) < 1e-6)
            snprintf(label, sizeof(label), "π/2");
        else if (fabs(xt - M_PI) < 1e-6)
            snprintf(label, sizeof(label), "π");
        else
            snprintf(label, sizeof(label), "%.2f", xt);
        int labelRow = xAxisRow + 1;
        int labelLen = (int)strlen(label);
        int startCol = col - labelLen / 2;
        if (startCol < 0) startCol = 0;
        if (startCol + labelLen > termWidth)
            startCol = termWidth - labelLen;
        for (int i = 0; i < labelLen; i++) {
            if (labelRow >= 0 && labelRow < termHeight && startCol + i < termWidth)
                buffer[labelRow][startCol + i] = label[i];
        }
    }

    // Тик-метки и подписи для оси Y (значения: 1, 0, -1)
    double yTicks[] = { 1.0, 0.0, -1.0 };
    for (double yt : yTicks) {
        int row = topMargin + (int)(((1.0 - yt) / 2.0) * (plotHeight - 1));
        buffer[row][yAxisCol] = '+';
        char label[16];
        snprintf(label, sizeof(label), "%.1f", yt);
        int labelLen = (int)strlen(label);
        int startCol = leftMargin - labelLen - 1; // отступ слева
        if (startCol < 0)
            startCol = 0;
        for (int i = 0; i < labelLen; i++) {
            if (row < termHeight && startCol + i < termWidth)
                buffer[row][startCol + i] = label[i];
        }
    }

    // Отрисовка графика функции sin(x)
    // Для каждого столбца в области графика вычисляем соответствующее значение x и y=sin(x)
    for (int col = leftMargin; col < leftMargin + plotWidth; col++) {
        double x = -M_PI + ((double)(col - leftMargin) / (plotWidth - 1)) * (2 * M_PI);
        double y = sin(x);
        int row = topMargin + (int)(((1.0 - y) / 2.0) * (plotHeight - 1));
        buffer[row][col] = '*';
    }

    // Выводим буфер в консоль
    for (const auto &line : buffer) {
        std::cout << line << "\n";
    }

    // Сброс ANSI-атрибутов
    std::cout << "\033[0m";
    return 0;
}
