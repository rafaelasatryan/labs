#include "mainwindow.h"
#include "plotwidget.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto *root = new QWidget(this);
    setCentralWidget(root);

    auto *mainLay = new QHBoxLayout(root);

    // Панель управления
    auto *panel = new QWidget(root);
    panel->setFixedWidth(320);
    auto *pLay = new QVBoxLayout(panel);

    auto *gbShow = new QGroupBox("Отображение", panel);
    auto *sLay = new QVBoxLayout(gbShow);

    mAll = new QCheckBox("Показать все", gbShow);
    mAll->setChecked(true);

    mC1 = new QCheckBox("График 1: №3 (роза 3 лепестка)", gbShow);
    mC2 = new QCheckBox("График 2: №6 (улитка Паскаля)", gbShow);
    mC3 = new QCheckBox("График 3: №9 (декартов лист)", gbShow);
    mC1->setChecked(true);
    mC2->setChecked(true);
    mC3->setChecked(true);

    sLay->addWidget(mAll);
    sLay->addSpacing(8);
    sLay->addWidget(mC1);
    sLay->addWidget(mC2);
    sLay->addWidget(mC3);

    auto *gbPar = new QGroupBox("Параметры", panel);
    auto *parLay = new QVBoxLayout(gbPar);

    mA = new QDoubleSpinBox(gbPar);
    mA->setRange(0.01, 1000.0);
    mA->setDecimals(3);
    mA->setValue(3.0);
    mA->setSingleStep(0.1);

    mB = new QDoubleSpinBox(gbPar);
    mB->setRange(-1000.0, 1000.0);
    mB->setDecimals(3);
    mB->setValue(1.0);
    mB->setSingleStep(0.1);

    parLay->addWidget(new QLabel("a:", gbPar));
    parLay->addWidget(mA);
    parLay->addSpacing(6);
    parLay->addWidget(new QLabel("b:", gbPar));
    parLay->addWidget(mB);

    auto *gbCol = new QGroupBox("Цвет", panel);
    auto *cLay = new QVBoxLayout(gbCol);

    mCol1 = new QPushButton("Цвет графика 1", gbCol);
    mCol2 = new QPushButton("Цвет графика 2", gbCol);
    mCol3 = new QPushButton("Цвет графика 3", gbCol);

    setBtnColor(mCol1, col1);
    setBtnColor(mCol2, col2);
    setBtnColor(mCol3, col3);

    cLay->addWidget(mCol1);
    cLay->addWidget(mCol2);
    cLay->addWidget(mCol3);

    pLay->addWidget(gbShow);
    pLay->addWidget(gbPar);
    pLay->addWidget(gbCol);
    pLay->addStretch(1);

    // Поле графика
    mPlot = new PlotWidget(root);

    mainLay->addWidget(panel);
    mainLay->addWidget(mPlot, 1);

    // Сигналы
    connect(mAll, &QCheckBox::toggled, this, &MainWindow::apply);
    connect(mC1, &QCheckBox::toggled, this, &MainWindow::apply);
    connect(mC2, &QCheckBox::toggled, this, &MainWindow::apply);
    connect(mC3, &QCheckBox::toggled, this, &MainWindow::apply);

    connect(mA, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::apply);
    connect(mB, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::apply);

    connect(mCol1, &QPushButton::clicked, this, &MainWindow::pickColor1);
    connect(mCol2, &QPushButton::clicked, this, &MainWindow::pickColor2);
    connect(mCol3, &QPushButton::clicked, this, &MainWindow::pickColor3);

    apply();
}

void MainWindow::setBtnColor(QPushButton *btn, const QColor &c)
{
    btn->setStyleSheet(QString("background-color:%1;").arg(c.name()));
}

void MainWindow::pickColor1()
{
    auto c = QColorDialog::getColor(col1, this, "Цвет графика 1");
    if (!c.isValid()) return;
    col1 = c; setBtnColor(mCol1, col1);
    apply();
}
void MainWindow::pickColor2()
{
    auto c = QColorDialog::getColor(col2, this, "Цвет графика 2");
    if (!c.isValid()) return;
    col2 = c; setBtnColor(mCol2, col2);
    apply();
}
void MainWindow::pickColor3()
{
    auto c = QColorDialog::getColor(col3, this, "Цвет графика 3");
    if (!c.isValid()) return;
    col3 = c; setBtnColor(mCol3, col3);
    apply();
}

void MainWindow::apply()
{
    const bool all = mAll->isChecked();

    mC1->setEnabled(!all);
    mC2->setEnabled(!all);
    mC3->setEnabled(!all);

    const bool s1 = all ? true : mC1->isChecked();
    const bool s2 = all ? true : mC2->isChecked();
    const bool s3 = all ? true : mC3->isChecked();

    PlotWidget::State st;
    st.a = mA->value();
    st.b = mB->value();
    st.show1 = s1; st.show2 = s2; st.show3 = s3;
    st.color1 = col1; st.color2 = col2; st.color3 = col3;

    mPlot->setState(st);
}
