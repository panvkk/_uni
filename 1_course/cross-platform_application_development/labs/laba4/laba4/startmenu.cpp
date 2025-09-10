#include "startmenu.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QFont>

StartMenu::StartMenu(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    difficultyComboBox = new QComboBox(this);
    difficultyComboBox->addItem("Легко", QVariant::fromValue(GameSettings{5, 3}));
    difficultyComboBox->addItem("Средне", QVariant::fromValue(GameSettings{8, 5}));
    difficultyComboBox->addItem("Сложно", QVariant::fromValue(GameSettings{12, 7}));

    startButton = new QPushButton("Начать игру", this);

    QLabel *difficultyLabel = new QLabel("Выберите сложность:", this);
    QFont font = difficultyLabel->font();
    font.setPointSize(font.pointSize() * 1.5); // Увеличиваем шрифт в 1.5 раза
    difficultyLabel->setFont(font);

    layout->addWidget(difficultyLabel);
    layout->addWidget(difficultyComboBox);
    layout->addWidget(startButton);

    connect(startButton, &QPushButton::clicked, this, &StartMenu::onStartButtonClicked);

    currentSettings = difficultyComboBox->currentData().value<GameSettings>();
    connect(difficultyComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int index){
                currentSettings = difficultyComboBox->itemData(index).value<GameSettings>();
            });

    // Увеличиваем шрифт для кнопки и комбобокса
    QFont buttonFont = startButton->font();
    buttonFont.setPointSize(buttonFont.pointSize() * 1.5);
    startButton->setFont(buttonFont);
    difficultyComboBox->setFont(buttonFont);

    // Увеличиваем размер кнопки
    startButton->setMinimumSize(300, 100); // Увеличиваем размер кнопки
}

GameSettings StartMenu::getSettings() const {
    return currentSettings;
}

void StartMenu::onStartButtonClicked() {
    emit startGameRequested(getSettings());
}
