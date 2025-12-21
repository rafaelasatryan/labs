#include "mainwindow.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>

#include <QFileDialog>
#include <QFile>
#include <QTextStream>

#include <QMessageBox>

#include <QWidget>
#include <QVBoxLayout>

#include <QTextCursor>
#include <QTextCharFormat>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    buildUi();
    buildMenu();

    setWindowTitle("Лабораторная 7 — Текстовый редактор (Qt5)");
    resize(800, 600);
}

void MainWindow::buildUi()
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    textEdit = new QTextEdit(central);
    textEdit->setPlaceholderText("Введите текст...");

    btnIncFont = new QPushButton("Увеличить шрифт выделенного", central);
    btnSave = new QPushButton("Сохранить", central);

    layout->addWidget(textEdit);
    layout->addWidget(btnIncFont);
    layout->addWidget(btnSave);

    setCentralWidget(central);

    connect(btnSave, SIGNAL(clicked()), this, SLOT(onSave()));
    connect(btnIncFont, SIGNAL(clicked()), this, SLOT(onIncreaseSelectedFont()));
}

void MainWindow::buildMenu()
{
    QMenu *fileMenu = menuBar()->addMenu("Файл");

    QAction *actOpen   = fileMenu->addAction("Открыть");
    QAction *actSave   = fileMenu->addAction("Сохранить");
    QAction *actSaveAs = fileMenu->addAction("Сохранить как...");

    connect(actOpen, SIGNAL(triggered()), this, SLOT(onOpen()));
    connect(actSave, SIGNAL(triggered()), this, SLOT(onSave()));
    connect(actSaveAs, SIGNAL(triggered()), this, SLOT(onSaveAs()));
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

void MainWindow::onSave()
{
    if (currentFilePath.isEmpty()) {
        onSaveAs();
        return;
    }

    saveToFile(currentFilePath);
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

    if (!path.endsWith(".txt", Qt::CaseInsensitive))
        path += ".txt";

    if (!saveToFile(path))
        return;

    currentFilePath = path;
    setWindowTitle(QString("Лабораторная 7 — %1").arg(currentFilePath));
}

bool MainWindow::loadFromFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл для чтения");
        return false;
    }

    QTextStream in(&file);
    textEdit->setPlainText(in.readAll());
    return true;
}

bool MainWindow::saveToFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл для записи");
        return false;
    }

    QTextStream out(&file);
    out << textEdit->toPlainText();
    return true;
}

void MainWindow::onIncreaseSelectedFont()
{
    QTextCursor cursor = textEdit->textCursor();
    QTextCharFormat format = cursor.charFormat();

    qreal size = format.fontPointSize();
    if (size <= 0) {
        size = textEdit->font().pointSizeF();
        if (size <= 0)
            size = 12;
    }

    format.setFontPointSize(size + 2);

    if (cursor.hasSelection()) {
        cursor.mergeCharFormat(format);
        textEdit->mergeCurrentCharFormat(format);
    } else {
        textEdit->mergeCurrentCharFormat(format);
    }
}
