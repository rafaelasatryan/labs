#pragma once

#include <QMainWindow>

class QLineEdit;
class QRadioButton;
class QCheckBox;
class QPushButton;
class QDateEdit;
class QLabel;
class QDate;
class Person;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onClearClicked();
    void onSaveClicked();
    void onBirthDateChanged(const QDate& date);

private:
    QLineEdit *leLastName;
    QLineEdit *leFirstName;

    QRadioButton *rbMale;
    QRadioButton *rbFemale;

    QCheckBox *cbWorking;
    QCheckBox *cbStudent;

    QDateEdit *deBirthDate;
    QLabel *lblAgeValue;
    QLabel *lblBirthError;

    QPushButton *btnClear;
    QPushButton *btnSave;

    int calcAgeManual(const QDate& birth) const;
    bool validateAge(int age) const;
    bool saveToFile(const Person& p, const QString& filePath) const;
};
