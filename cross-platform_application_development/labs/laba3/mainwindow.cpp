#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QRandomGenerator>
#include <QPropertyAnimation>
#include <QDebug>
#include <QSoundEffect>
#include <QTimer>
#include <QtMath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , rhymeWordIndex(0)
    , currentPersonIndex(0)
{
    // Задаём фиксированный размер окна для простоты
    setFixedSize(800, 600);

    // Создаём центральный виджет с белым фоном
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setStyleSheet("background-color: white;");

    // Ярлык в центре экрана для текущего слова считалки
    wordLabel = new QLabel(this);
    wordLabel->setAlignment(Qt::AlignCenter);
    wordLabel->setStyleSheet("font: bold 24px; color: blue;");
    wordLabel->setGeometry(300, 250, 200, 50);

    // Ярлык для самого длинного слова считалки (правый верхний угол)
    longestWordLabel = new QLabel(this);
    longestWordLabel->setAlignment(Qt::AlignCenter);
    longestWordLabel->setStyleSheet("font: bold 18px; color: red;");
    longestWordLabel->setGeometry(600, 0, 200, 50);

    // Кнопка для перехода к следующему слову
    nextWordButton = new QPushButton("Next Word", this);
    nextWordButton->setGeometry(350, 520, 100, 40);
    connect(nextWordButton, &QPushButton::clicked, this, &MainWindow::nextWord);

    // Инициализируем синтезатор речи
    tts = new QTextToSpeech(this);

    // Загружаем данные
    loadRhyme();
    loadPersons();

    // Располагаем фотографии участников по кругу
    arrangePhotos();

    // Выбираем случайного участника для старта считалки
    if (!persons.isEmpty()) {
        currentPersonIndex = QRandomGenerator::global()->bounded(persons.size());
    }

    // Если имеется хотя бы одна считалка, выбираем случайную и разбиваем её на слова
    if (!rhymeVerses.isEmpty()) {
        int randRhymeIndex = QRandomGenerator::global()->bounded(rhymeVerses.size());
        QString selectedRhyme = rhymeVerses[randRhymeIndex];
        currentRhymeWords = selectedRhyme.split(" ", Qt::SkipEmptyParts).toVector();
        rhymeWordIndex = 0;
        // Определяем самое длинное слово считалки
        longestWord = "";
        for (const QString &word : currentRhymeWords) {
            if (word.length() > longestWord.length())
                longestWord = word;
        }
        longestWordLabel->setText(longestWord);
    }
}

MainWindow::~MainWindow()
{
}

///////////////////////////////////////////////////////////////////////////////
// Загрузка считалок из файла rhyme.txt (файл должен содержать не менее 10 строк)
void MainWindow::loadRhyme() {
    QFile file("rhyme.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Не удалось открыть файл rhyme.txt";
        return;
    }
    QTextStream in(&file);
    while(!in.atEnd()) {
        QString line = in.readLine();
        if (!line.trimmed().isEmpty())
            rhymeVerses.append(line);
    }
    file.close();
}

///////////////////////////////////////////////////////////////////////////////
// Загрузка участников: фотографии из папки "photo" и треки из "tracks".
// Файлы фотографий должны называться согласно ФИО студентов (без расширения трека)
// Пример: photo/ИвановИван.jpg, tracks/ИвановИван.mp3
void MainWindow::loadPersons() {
    QDir photoDir("photo");
    QStringList nameFilters;
    nameFilters << "*.jpg" << "*.png" << "*.bmp";
    QFileInfoList fileList = photoDir.entryInfoList(nameFilters, QDir::Files);
    for (const QFileInfo &fileInfo : fileList) {
        QString fullName = fileInfo.baseName();  // Имя файла – ФИО студента
        QPixmap photo(fileInfo.absoluteFilePath());
        // Формируем путь к звуковому треку (предполагается формат .mp3)
        QString trackPath = "tracks/" + fullName + ".mp3";
        if (!QFile::exists(trackPath)) {
            trackPath = ""; // Если трек не найден – можно использовать значение по умолчанию
        }
        persons.append(Person(fullName, photo, trackPath));
    }
}

///////////////////////////////////////////////////////////////////////////////
// Раскладка фотографий на круге. Позиция каждого виджета вычисляется на основе
// центра окна и заданного радиуса, чтобы между фотографиями были равные промежутки.
void MainWindow::arrangePhotos() {
    // Удаляем старые ярлыки, если они уже созданы
    for (QLabel* lbl : photoLabels) {
        lbl->deleteLater();
    }
    photoLabels.clear();

    int count = persons.size();
    if(count == 0)
        return;
    int radius = 200;  // радиус кругового расположения
    QPoint center(width()/2, height()/2);
    for (int i = 0; i < count; i++) {
        double angle = (2 * M_PI * i) / count;
        int x = center.x() + radius * cos(angle) - 50; // смещение для центрирования (ширина ярлыка – 100)
        int y = center.y() + radius * sin(angle) - 50; // высота ярлыка – 100
        QLabel *photoLabel = new QLabel(this);
        photoLabel->setPixmap(persons[i].photo.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        photoLabel->setGeometry(x, y, 100, 100);
        photoLabel->setFrameStyle(QFrame::Box);
        photoLabel->setLineWidth(2);
        photoLabel->setAlignment(Qt::AlignCenter);
        // Выводим ФИО под фотографией (можно также создать отдельный ярлык)
        photoLabel->setText(persons[i].fullName);
        photoLabel->setStyleSheet("background: lightgray;");

        photoLabels.append(photoLabel);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Обработчик кнопки "Next Word". При каждом нажатии выводится следующее слово считалки,
// озвучивается, а текущая фотография подсвечивается изменением текста. Если слово
// является последним, вызывается функция удаления участника.
void MainWindow::nextWord() {
    if (persons.size() <= 1) {
        // Если остаётся один участник, игра уже завершена.
        return;
    }
    // Если ещё не достигнут конец считалки – выводим и озвучиваем слово
    if (rhymeWordIndex < currentRhymeWords.size()) {
        QString word = currentRhymeWords[rhymeWordIndex];
        wordLabel->setText(word);
        tts->say(word); // озвучивание слова
        // Можно отобразить слово прямо под фотографией участника
        photoLabels[currentPersonIndex]->setText(persons[currentPersonIndex].fullName + "\n" + word);
        rhymeWordIndex++;
    }
    // Если считалка закончилась – удаляем текущего участника
    if (rhymeWordIndex == currentRhymeWords.size()) {
        removeCurrentPerson();
        rhymeWordIndex = 0; // сбрасываем индекс для новой итерации считалки
    } else {
        // Переходим к следующему участнику по кругу
        currentPersonIndex = (currentPersonIndex + 1) % persons.size();
    }
}

///////////////////////////////////////////////////////////////////////////////
// Удаление участника с анимацией «выбегания» фотографии в правый верхний угол,
// а также проигрывание звуковой дорожки, соответствующей студенту.
// После удаления фотографий происходит перерасчёт позиций по кругу.
void MainWindow::removeCurrentPerson() {
    if (persons.isEmpty())
        return;

    QLabel* label = photoLabels[currentPersonIndex];
    QPropertyAnimation *anim = new QPropertyAnimation(label, "pos");
    anim->setDuration(1000);  // длительность анимации 1 секунда
    anim->setStartValue(label->pos());
    anim->setEndValue(QPoint(width(), 0)); // перемещение в правый верхний угол
    anim->start(QAbstractAnimation::DeleteWhenStopped);

    // Проигрываем звуковой трек участника (если он указан)
    if (!persons[currentPersonIndex].trackPath.isEmpty()){
        QSoundEffect *effect = new QSoundEffect(this);
        effect->setSource(QUrl::fromLocalFile(persons[currentPersonIndex].trackPath));
        effect->setVolume(0.5);
        effect->play();
        connect(effect, &QSoundEffect::playingChanged, effect, [effect](){
            if (!effect->isPlaying()){
                effect->deleteLater();
            }
        });
    }

    // Через небольшую задержку (после завершения анимации) удаляем данные участника и
    // перестраиваем расположение фотографий
    QTimer::singleShot(1100, this, [this]() {
        persons.removeAt(currentPersonIndex);
        QLabel* removedLabel = photoLabels.takeAt(currentPersonIndex);
        removedLabel->deleteLater();

        arrangePhotos();
        if (currentPersonIndex >= persons.size())
            currentPersonIndex = 0;

        // Если остался только один участник – он объявляется победителем
        if (persons.size() == 1) {
            crownWinner();
        }
    });
}

///////////////////////////////////////////////////////////////////////////////
// Оформление победителя: на оставшуюся фотографию накладывается «венок»
// (здесь для простоты используется смайлик), выводится текст "won!" и проигрывается
// звуковая дорожка фейерверков (файл fireworks.mp3 должен быть доступен).
void MainWindow::crownWinner() {
    QLabel *winnerLabel = photoLabels[0];
    winnerLabel->setText(winnerLabel->text() + "\n 👑 won!");
    QSoundEffect *fireworksSound = new QSoundEffect(this);
    fireworksSound->setSource(QUrl::fromLocalFile("fireworks.mp3"));
    fireworksSound->setVolume(0.7);
    fireworksSound->play();
}
