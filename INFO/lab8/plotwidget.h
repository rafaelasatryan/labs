#pragma once

#include <QWidget>
#include <QColor>

class PlotWidget : public QWidget
{
    Q_OBJECT
public:
    struct State {
        double a = 3.0;
        double b = 1.0;

        bool show1 = true; // №3
        bool show2 = true; // №6
        bool show3 = true; // №9

        QColor color1 = Qt::red;
        QColor color2 = Qt::blue;
        QColor color3 = Qt::darkGreen;
    };

    explicit PlotWidget(QWidget *parent = nullptr);
    void setState(const State& s);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    State st;
};
