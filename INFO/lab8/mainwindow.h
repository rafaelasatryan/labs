#pragma once

#include <QMainWindow>
#include <QColor>

class PlotWidget;
class QCheckBox;
class QDoubleSpinBox;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void pickColor1();
    void pickColor2();
    void pickColor3();
    void apply();

private:
    void setBtnColor(QPushButton* btn, const QColor& c);

    PlotWidget* mPlot = nullptr;

    QCheckBox* mAll = nullptr;
    QCheckBox* mC1  = nullptr;
    QCheckBox* mC2  = nullptr;
    QCheckBox* mC3  = nullptr;

    QDoubleSpinBox* mA = nullptr;
    QDoubleSpinBox* mB = nullptr;

    QPushButton* mCol1 = nullptr;
    QPushButton* mCol2 = nullptr;
    QPushButton* mCol3 = nullptr;

    QColor col1 = Qt::red;
    QColor col2 = Qt::blue;
    QColor col3 = Qt::darkGreen;
};
