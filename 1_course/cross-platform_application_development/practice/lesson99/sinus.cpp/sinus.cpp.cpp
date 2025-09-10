// Обязательно определяем макрос до любых включений заголовков
#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

// Функция для получения размеров терминала (число столбцов и строк)
void getTerminalSize(int& cols, int& rows) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
    else {
        cols = 80;
        rows = 24;
    }
#else
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        cols = 80;
        rows = 24;
    }
    else {
        cols = w.ws_col;
        rows = w.ws_row;
    }
#endif
}

int main() {
    int termWidth, termHeight;
    getTerminalSize(termWidth, termHeight);

    // Определяем отступы для подписей осей
    int leftMargin = 8;   // для подписей по оси Y
    int rightMargin = 2;
    int topMargin = 1;   // для стрелок оси Y
    int bottomMargin = 2;   // для подписей оси X

    int plotWidth = termWidth - leftMargin - rightMargin;
    int plotHeight = termHeight - topMargin - bottomMargin;

    // Создаём буфер – вектор строк, по одной строке на каждую строку экрана
    std::vector<std::string> buffer(termHeight, std::string(termWidth, ' '));

    // Очистка экрана, установка курсора в начало и применение ANSI-кодов:
    // чёрный фон (40) и белый текст (37)
    std::cout << "\033[2J\033[H" << "\033[40m\033[37m";

    // Математическая система координат:
    // x ∈ [-π, π], y ∈ [-1, 1].
    // Преобразование:
    //   pixelX = leftMargin + ((x + π) / (2π)) * (plotWidth - 1)
    //   pixelY = topMargin + ((1 - y) / 2) * (plotHeight - 1)
    //
    // Вычисляем положение осей (соответствует x=0 и y=0).
    int xAxisRow = topMargin + static_cast<int>(0.5 * (plotHeight - 1)); // y = 0
    int yAxisCol = leftMargin + static_cast<int>(0.5 * (plotWidth - 1));   // x = 0

    // Рисуем ось X (горизонтальную)
    for (int col = leftMargin; col < leftMargin + plotWidth; col++) {
        buffer[xAxisRow][col] = '-';
    }
    // Рисуем ось Y (вертикальную)
    for (int row = topMargin; row < topMargin + plotHeight; row++) {
        buffer[row][yAxisCol] = '|';
    }

    // Добавляем стрелки на концах осей:
    // На оси X: слева "<", справа ">"
    buffer[xAxisRow][leftMargin] = '<';
    buffer[xAxisRow][leftMargin + plotWidth - 1] = '>';
    // На оси Y: сверху "^", снизу "v"
    buffer[topMargin][yAxisCol] = '^';
    buffer[topMargin + plotHeight - 1][yAxisCol] = 'v';

    // Тик-метки и подписи для оси X (значения: -π, -π/2, 0, π/2, π)
    const int xTicksCount = 5;
    const double xTicks[xTicksCount] = { -M_PI, -M_PI / 2, 0, M_PI / 2, M_PI };
    for (int i = 0; i < xTicksCount; i++) {
        double xt = xTicks[i];
        int col = leftMargin + static_cast<int>(((xt + M_PI) / (2.0 * M_PI)) * (plotWidth - 1));
        buffer[xAxisRow][col] = '+'; // ставим тик

        // Формируем строковую метку
        char label[16];
        if (std::fabs(xt + M_PI) < 1e-6)
            std::snprintf(label, sizeof(label), "-π");
        else if (std::fabs(xt + M_PI / 2) < 1e-6)
            std::snprintf(label, sizeof(label), "-π/2");
        else if (std::fabs(xt) < 1e-6)
            std::snprintf(label, sizeof(label), "0");
        else if (std::fabs(xt - M_PI / 2) < 1e-6)
            std::snprintf(label, sizeof(label), "π/2");
        else if (std::fabs(xt - M_PI) < 1e-6)
            std::snprintf(label, sizeof(label), "π");
        else
            std::snprintf(label, sizeof(label), "%.2f", xt);

        int labelRow = xAxisRow + 1;
        int labelLen = static_cast<int>(std::strlen(label));
        int startCol = col - labelLen / 2;
        if (startCol < 0)
            startCol = 0;
        if (startCol + labelLen > termWidth)
            startCol = termWidth - labelLen;
        for (int j = 0; j < labelLen; j++) {
            if (labelRow >= 0 && labelRow < termHeight && (startCol + j) < termWidth)
                buffer[labelRow][startCol + j] = label[j];
        }
    }

    // Тик-метки и подписи для оси Y (значения: 1.0, 0.0, -1.0)
    const int yTicksCount = 3;
    const double yTicks[yTicksCount] = { 1.0, 0.0, -1.0 };
    for (int i = 0; i < yTicksCount; i++) {
        double yt = yTicks[i];
        int row = topMargin + static_cast<int>(((1.0 - yt) / 2.0) * (plotHeight - 1));
        buffer[row][yAxisCol] = '+';
        char label[16];
        std::snprintf(label, sizeof(label), "%.1f", yt);
        int labelLen = static_cast<int>(std::strlen(label));
        int startCol = leftMargin - labelLen - 1; // отступ слева
        if (startCol < 0)
            startCol = 0;
        for (int j = 0; j < labelLen; j++) {
            if (row < termHeight && (startCol + j) < termWidth)
                buffer[row][startCol + j] = label[j];
        }
    }

    int last_row = -1;
    int current_row, current_col;

    for (int col = leftMargin; col < leftMargin + plotWidth; col++) {
        double x = -M_PI + ((double)(col - leftMargin) / (plotWidth - 1)) * (2 * M_PI);
        double y = sin(x);
        current_row = topMargin + static_cast<int>(((1.0 - y) / 2.0) * (plotHeight - 1));
        current_col = col;

        // Если пытаемся поставить в ту же строку - смещаемся вниз
        if (current_row == last_row) {
            current_row++;
            // Корректируем позицию если вышли за границы
            if (current_row >= termHeight) current_row = termHeight - 1;
        }

        // Обновляем последнюю позицию
        last_row = current_row;

        // Проверяем границы и рисуем
        if (current_row >= 0 && current_row < termHeight &&
            current_col >= 0 && current_col < termWidth) {
            buffer[current_row][current_col] = '*';
        }
    }

    // Выводим сформированный буфер в консоль
    for (const auto& line : buffer) {
        std::cout << line << "\n";
    }

    // Сброс ANSI-атрибутов (цвета и т.д.)
    std::cout << "\033[0m";
    //std::cout << "Масштаб по оси X: 2П\nМасштаб по оси Y: 2\n";
    std::cin.get();
    return 0;
}