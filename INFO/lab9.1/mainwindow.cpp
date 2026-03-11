#include "mainwindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTableWidget>
#include <cmath>
#include <QColorDialog>
#include <QColor>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    resize(1200,700);

    table = new QTableWidget(this);
    table->setColumnCount(8);
    table->setRowCount(0);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setHorizontalHeaderLabels({"Figure", "cx", "cy", "r",
                                      "Line width", "Line color", "Fill color",
                                      "Area"});

    openInfo = new QPushButton("Open file");
    openInfo->setMinimumSize(200,45);

    saveInfo = new QPushButton("Save file");
    saveInfo->setMinimumSize(200,45);

    Add = new QPushButton("Add");
    Add->setMinimumSize(100,35);

    Delete = new QPushButton("Delete");
    Delete->setMinimumSize(100,35);

    paint = new paintwidget;
    paint->setMinimumSize(900,700);
    paint->setTable(table);

    QGridLayout *glayout = new QGridLayout;
    glayout->addWidget(openInfo, 0, 0);
    glayout->addWidget(saveInfo, 0, 1);
    glayout->addWidget(Add, 1, 0);
    glayout->addWidget(Delete, 1, 1);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(table,0);
    vlayout->addLayout(glayout,1);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(vlayout);
    mainLayout->addWidget(paint);

    connect(openInfo, &QPushButton::clicked, this, &MainWindow::buttonInfo);
    connect(saveInfo, &QPushButton::clicked, this, &MainWindow::saveToFile);
    connect(Add, &QPushButton::clicked, this, &MainWindow::slotAdd);
    connect(Delete, &QPushButton::clicked, this, &MainWindow::slotDelete);
    connect(table, &QTableWidget::cellChanged, this, &MainWindow::onCellChanged);
    connect(table->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::onHeaderClicked);
}

void MainWindow::slotAdd()
{
    int row = table->rowCount();
    table->insertRow(row);
    table->setCellWidget(row,5,createColorButton(0,0,0));
    table->setCellWidget(row,6,createColorButton(255,255,255));

    paint->update();
}

void MainWindow::slotDelete()
{
    int row = table->currentRow();
    if (row >= 0)
        table->removeRow(row);

    paint->update();
}

void MainWindow::onHeaderClicked(int col)
{
    if (col != 0 && col != 7)
        return;

    static Qt::SortOrder order = Qt::AscendingOrder;
    table->sortItems(col, order);

    order = (order == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
}

void MainWindow::buttonInfo()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open file", "C:/Users/RAFAEL/Documents/lab1");
    if (fileName.isEmpty())
    {
        return;
    }

    loadFromFile(fileName);
}

void MainWindow::saveToFile()
{
    QString fileSave = QFileDialog::getSaveFileName(this, "Save file");

    if (fileSave.isEmpty())
        return;

    QFile file(fileSave);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    for (int row = 0; row < table->rowCount(); row++)
    {
        QString figure = table->item(row,0)->text();
        QString cx = table->item(row,1)->text();
        QString cy = table->item(row,2)->text();
        QString r = table->item(row,3)->text();
        QString lineWidth = table->item(row,4)->text();

        QPushButton *lineBtn = qobject_cast<QPushButton*>(table->cellWidget(row,5));

        QPushButton *fillBtn = qobject_cast<QPushButton*>(table->cellWidget(row,6));

        int lr = lineBtn->property("r").toInt();
        int lg = lineBtn->property("g").toInt();
        int lb = lineBtn->property("b").toInt();

        int fr = fillBtn->property("r").toInt();
        int fg = fillBtn->property("g").toInt();
        int fb = fillBtn->property("b").toInt();

        out << figure << " " << cx << " " << cy << " " << r << " " << lineWidth << " " << lr << " " << lg << " " << lb << " "
            << fr << " " << fg << " " << fb << "\n";
    }

}

void MainWindow::loadFromFile(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "title", "File not open");
        return;
    }

    table->blockSignals(true);

    QTextStream in(&file);
    table->setRowCount(0);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split(" ", Qt::SkipEmptyParts);
        if (line.trimmed().isEmpty())
            continue;

        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(parts[0]));
        table->setItem(row, 1, new QTableWidgetItem(parts[1]));
        table->setItem(row, 2, new QTableWidgetItem(parts[2]));
        table->setItem(row, 3, new QTableWidgetItem(parts[3]));
        table->setItem(row, 4, new QTableWidgetItem(parts[4]));

        QPushButton *lineBtn = createColorButton(parts[5].toInt(), parts[6].toInt(), parts[7].toInt());

        table->setCellWidget(row, 5, lineBtn);

        QPushButton *fillBtn = createColorButton(parts[8].toInt(), parts[9].toInt(), parts[10].toInt());

        table->setCellWidget(row, 6, fillBtn);

        int r = parts[3].toInt();
        double area = M_PI * r * r;

        QTableWidgetItem *areaItem = new QTableWidgetItem(QString::number(area, 'f', 2));
        areaItem->setData(Qt::EditRole, area);
        areaItem->setFlags(areaItem->flags() & ~Qt::ItemIsEditable);
        table->setItem(row, 7, areaItem);

    }
    file.close();
    table->blockSignals(false);

    paint->update();
}

void MainWindow::onCellChanged(int row, int col)
{
    if (col != 3)
        return;

    QTableWidgetItem *rItem = table->item(row, 3);
    if(!rItem)
        return;

    bool ok;
    int r = rItem->text().toInt(&ok);

    if(!ok)
        return;

    double area = M_PI * r * r;

    table->blockSignals(true);

    QTableWidgetItem *areaItem = new QTableWidgetItem(QString::number(area, 'f', 2));
    areaItem->setData(Qt::EditRole, area);
    areaItem->setFlags(areaItem->flags() & ~Qt::ItemIsEditable);
    table->setItem(row, 7, areaItem);

    table->blockSignals(false);

    paint->update();
}

QPushButton* MainWindow::createColorButton(int r, int g, int b)
{
    QPushButton *btn = new QPushButton;
    btn->setMinimumHeight(25);

    btn->setStyleSheet(QString("background-color: rgb(%1,%2,%3);").arg(r).arg(g).arg(b));

    btn->setProperty("r", r);
    btn->setProperty("g", g);
    btn->setProperty("b", b);

    connect(btn, &QPushButton::clicked, this, [=](){
        QColor currentColor(
            btn->property("r").toInt(),
            btn->property("g").toInt(),
            btn->property("b").toInt()
            );
        QColor newColor = QColorDialog::getColor(currentColor, this);

        if (!newColor.isValid())
            return;

        btn->setStyleSheet(QString("background-color: rgb(%1,%2,%3);").arg(newColor.red()).arg(newColor.green()).arg(newColor.blue()));
        btn->setProperty("r", newColor.red());
        btn->setProperty("g", newColor.green());
        btn->setProperty("b", newColor.blue());
    });
    return btn;
}
