#ifndef STARTMENU_H
#define STARTMENU_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include "settings.h"

class StartMenu : public QWidget {
    Q_OBJECT

public:
    explicit StartMenu(QWidget *parent = nullptr);
    GameSettings getSettings() const;

signals:
    void startGameRequested(const GameSettings &settings);

private slots:
    void onStartButtonClicked();

private:
    QComboBox *difficultyComboBox;
    QPushButton *startButton;
    GameSettings currentSettings;
};

#endif // STARTMENU_H
