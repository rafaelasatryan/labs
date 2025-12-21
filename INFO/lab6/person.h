#pragma once
#include <QString>
#include <QDate>

class Person
{
public:
    enum class Gender { Male, Female };

    QString lastName;
    QString firstName;
    Gender gender = Gender::Male;
    bool isWorking = false;
    bool isStudent = false;

    QDate birthDate;
    int age = 0;
};
