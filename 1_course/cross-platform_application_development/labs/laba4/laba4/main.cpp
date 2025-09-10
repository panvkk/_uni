#include "mainwindow.h"
#include <QApplication>
#include <QMetaType> // Для регистрации GameSettings
#include "settings.h" // Для GameSettings

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Регистрация пользовательского типа для использования в QVariant (для ComboBox)
    qRegisterMetaType<GameSettings>("GameSettings");

    MainWindow w;
    w.show();
    return a.exec();
}


/*
Напиши программу на Qt(Qt Widget Application, C++), которое будет выглядеть как игра, в которой курсор должен гнаться за картинкой, которая от него убегает. Если игрок сможет догнать картинку и нажать на неё, то списывается 1 хп. Когда игрок нанесёт все 5 урона картинке, должен появиться финальный экран. Финальный экран выглядит так: появляется катафалк, который едет за фото из одного конца экрана, забирает фото и везёт его до экрана.

*/
