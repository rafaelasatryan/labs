#include "mainwindow.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QFont>
#include <QTextCursor>
#include <QTextCharFormat>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    buildUi();
    buildMenu();
    setWindowTitle("Лабораторная 7");
    resize(800, 600);
}

void MainWindow::buildUi()
{
    auto *central = new QWidget(this);
    auto *layout = new QVBoxLayout(central);

    textEdit = new QTextEdit(central);
    textEdit->setPlaceholderText("Введите текст...");

    btnSave = new QPushButton("Сохранить", central);
    btnIncFont = new QPushButton("Увеличить шрифт выделенного", central);

    layout->addWidget(textEdit);
    layout->addWidget(btnIncFont);
    layout->addWidget(btnSave);

    setCentralWidget(central);

    connect(btnSave, &QPushButton::clicked, this, &MainWindow::onSave);
    connect(btnIncFont, &QPushButton::clicked, this, &MainWindow::onIncreaseSelectedFont);
}

void MainWindow::buildMenu()
{
    QMenu *fileMenu = menuBar()->addMenu("Файл");

    QAction *actOpen = fileMenu->addAction("Открыть");
    QAction *actSave = fileMenu->addAction("Сохранить");
    QAction *actSaveAs = fileMenu->addAction("Сохранить как...");

    connect(actOpen, &QAction::triggered, this, &MainWindow::onOpen);
    connect(actSave, &QAction::triggered, this, &MainWindow::onSave);
    connect(actSaveAs, &QAction::triggered, this, &MainWindow::onSaveAs);
}

void MainWindow::onOpen()
{
    QString path = QFileDialog::getOpenFileName(
        this,
        "Открыть txt файл",
        QString(),
        "Text files (*.txt);;All files (*.*)"
        );

    if (path.isEmpty())
        return;

    if (!loadFromFile(path))
        return;

    currentFilePath = path;
    setWindowTitle(QString("Лабораторная 7 — %1").arg(currentFilePath));
}

void MainWindow::onSaveAs()
{
    QString path = QFileDialog::getSaveFileName(
        this,
        "Сохранить как...",
        currentFilePath.isEmpty() ? QString("document.txt") : currentFilePath,
        "Text files (*.txt);;All files (*.*)"
        );

    if (path.isEmpty())
        return;

    // если пользователь не указал расширение, добавим .txt
    if (!path.endsWith(".txt", Qt::CaseInsensitive))
        path += ".txt";

    if (!saveToFile(path))
        return;

    currentFilePath = path;
    setWindowTitle(QString("Лабораторная 7 — %1").arg(currentFilePath));
}

void MainWindow::onSave()
{
    if (currentFilePath.isEmpty()) {
        onSaveAs();
        return;
    }

    saveToFile(currentFilePath);
}

bool MainWindow::loadFromFile(const QString &path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл для чтения.");
        return false;
    }

    QTextStream in(&f);
    in.setEncoding(QStringConverter::Utf8); // Qt6
    textEdit->setPlainText(in.readAll());
    return true;
}

bool MainWindow::saveToFile(const QString &path)
{
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл для записи.");
        return false;
    }

    QTextStream out(&f);
    out.setEncoding(QStringConverter::Utf8); // Qt6
    out << textEdit->toPlainText();
    return true;
}

void MainWindow::onIncreaseSelectedFont()
{
    QTextCursor cursor = textEdit->textCursor();
    QTextCharFormat fmt = cursor.charFormat();

    qreal size = fmt.fontPointSize();
    if (size <= 0.0) {
        size = textEdit->fontPointSize();
        if (size <= 0.0) {
            size = textEdit->font().pointSizeF();
        }
        if (size <= 0.0) size = 12.0;
    }

    fmt.setFontPointSize(size + 2.0);

    if (cursor.hasSelection()) {
        cursor.mergeCharFormat(fmt);
        textEdit->mergeCurrentCharFormat(fmt);
    } else {
        textEdit->mergeCurrentCharFormat(fmt);
    }
}
