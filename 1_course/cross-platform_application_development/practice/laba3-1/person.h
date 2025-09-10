#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include <QPixmap>

// Класс Person содержит информацию о студенте: ФИО, фотографию и путь к его звуковому треку.
class Person {
public:
    QString fullName;
    QPixmap photo;
    QString trackPath; // путь к треку студента

    Person(const QString& name, const QPixmap& photo, const QString& trackPath)
        : fullName(name), photo(photo), trackPath(trackPath) {}
};

#endif // PERSON_H
