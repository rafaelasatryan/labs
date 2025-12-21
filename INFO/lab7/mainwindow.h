#pragma once

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QString>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onOpen();
    void onSave();
    void onSaveAs();
    void onIncreaseSelectedFont();

private:
    void buildUi();
    void buildMenu();
    bool loadFromFile(const QString &path);
    bool saveToFile(const QString &path);

private:
    QTextEdit *textEdit = nullptr;
    QPushButton *btnSave = nullptr;
    QPushButton *btnIncFont = nullptr;

    QString currentFilePath;
};
