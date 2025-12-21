#include "mainwindow.h"
#include "person.h"

#include <QWidget>
#include <QLineEdit>
#include <QRadioButton>
#include <QCheckBox>
#include <QPushButton>
#include <QDateEdit>
#include <QLabel>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QButtonGroup>

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDate>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    leLastName(nullptr),
    leFirstName(nullptr),
    rbMale(nullptr),
    rbFemale(nullptr),
    cbWorking(nullptr),
    cbStudent(nullptr),
    deBirthDate(nullptr),
    lblAgeValue(nullptr),
    lblBirthError(nullptr),
    btnClear(nullptr),
    btnSave(nullptr)
{
    auto *central = new QWidget(this);
    setCentralWidget(central);

    leLastName = new QLineEdit(this);
    leFirstName = new QLineEdit(this);

    rbMale = new QRadioButton("Мужской", this);
    rbFemale = new QRadioButton("Женский", this);
    rbMale->setChecked(true);

    auto *genderGroup = new QButtonGroup(this);
    genderGroup->addButton(rbMale);
    genderGroup->addButton(rbFemale);

    cbWorking = new QCheckBox("Работает", this);
    cbStudent = new QCheckBox("Студент", this);

    deBirthDate = new QDateEdit(this);
    deBirthDate->setCalendarPopup(true);
    deBirthDate->setDisplayFormat("dd.MM.yyyy");
    deBirthDate->setDate(QDate::currentDate());

    lblAgeValue = new QLabel("0", this);

    lblBirthError = new QLabel("", this);
    lblBirthError->setStyleSheet("color: red;");
    lblBirthError->setMinimumWidth(220);

    btnClear = new QPushButton("Очистить", this);
    btnSave  = new QPushButton("Сохранить", this);

    auto *root = new QVBoxLayout(central);

    auto *form = new QFormLayout();
    form->addRow("Фамилия:", leLastName);
    form->addRow("Имя:", leFirstName);

    auto *genderRow = new QHBoxLayout();
    genderRow->addWidget(rbMale);
    genderRow->addWidget(rbFemale);
    genderRow->addStretch(1);
    form->addRow("Пол:", genderRow);

    auto *flagsRow = new QHBoxLayout();
    flagsRow->addWidget(cbWorking);
    flagsRow->addWidget(cbStudent);
    flagsRow->addStretch(1);
    form->addRow("Статус:", flagsRow);

    auto *birthRow = new QHBoxLayout();
    birthRow->addWidget(deBirthDate);
    birthRow->addSpacing(12);
    birthRow->addWidget(new QLabel("Возраст:", this));
    birthRow->addWidget(lblAgeValue);
    birthRow->addSpacing(12);
    birthRow->addWidget(lblBirthError);
    birthRow->addStretch(1);
    form->addRow("Дата рождения:", birthRow);

    root->addLayout(form);

    auto *buttons = new QHBoxLayout();
    buttons->addStretch(1);
    buttons->addWidget(btnClear);
    buttons->addWidget(btnSave);
    root->addLayout(buttons);

    connect(btnClear, &QPushButton::clicked, this, &MainWindow::onClearClicked);
    connect(btnSave,  &QPushButton::clicked, this, &MainWindow::onSaveClicked);
    connect(deBirthDate, &QDateEdit::dateChanged, this, &MainWindow::onBirthDateChanged);

    onBirthDateChanged(deBirthDate->date());

    setWindowTitle("Лабораторная 6 — Qt Widgets");
    resize(700, 240);
}

int MainWindow::calcAgeManual(const QDate &birth) const
{
    if (!birth.isValid())
        return 0;

    const QDate today = QDate::currentDate();
    int age = today.year() - birth.year();

    QDate birthdayThisYear(today.year(), birth.month(), birth.day());
    if (!birthdayThisYear.isValid())
        birthdayThisYear = QDate(today.year(), 3, 1); // 29.02 -> 01.03

    if (birthdayThisYear > today)
        age--;

    return age;
}

bool MainWindow::validateAge(int age) const
{
    return age >= 0 && age <= 100;
}

void MainWindow::onBirthDateChanged(const QDate &date)
{
    const int age = calcAgeManual(date);
    lblAgeValue->setText(QString::number(age));

    if (!validateAge(age))
        lblBirthError->setText("Некорректный возраст (0..100)");
    else
        lblBirthError->clear();
}

void MainWindow::onClearClicked()
{
    leLastName->clear();
    leFirstName->clear();

    rbMale->setChecked(true);

    cbWorking->setChecked(false);
    cbStudent->setChecked(false);

    deBirthDate->setDate(QDate::currentDate());
}

bool MainWindow::saveToFile(const Person &p, const QString &filePath) const
{
    QFile f(filePath);
    if (!f.open(QIODevice::Append | QIODevice::Text))
        return false;

    QTextStream out(&f);

    out << "Фамилия: " << p.lastName << "\n";
    out << "Имя: " << p.firstName << "\n";
    out << "Пол: " << (p.gender == Person::Gender::Male ? "Мужской" : "Женский") << "\n";
    out << "Работает: " << (p.isWorking ? "Да" : "Нет") << "\n";
    out << "Студент: " << (p.isStudent ? "Да" : "Нет") << "\n";
    out << "Дата рождения: " << p.birthDate.toString("dd.MM.yyyy") << "\n";
    out << "Возраст: " << p.age << "\n";
    out << "-----------------------------\n";

    return true;
}

void MainWindow::onSaveClicked()
{
    const int age = calcAgeManual(deBirthDate->date());
    if (!validateAge(age)) {
        lblBirthError->setText("Некорректный возраст (0..100)");
        return;
    }
    lblBirthError->clear();

    Person p;
    p.lastName = leLastName->text().trimmed();
    p.firstName = leFirstName->text().trimmed();
    p.gender = rbMale->isChecked() ? Person::Gender::Male : Person::Gender::Female;
    p.isWorking = cbWorking->isChecked();
    p.isStudent = cbStudent->isChecked();
    p.birthDate = deBirthDate->date();
    p.age = age;

    const QString filePath = "people.txt";
    if (!saveToFile(p, filePath)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл для записи: " + filePath);
        return;
    }

    QMessageBox::information(this, "Готово", "Данные сохранены в файл: " + filePath);
}
