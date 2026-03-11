#include "paintwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <cmath>
#include <QKeyEvent>
#include <QPushButton>

paintwidget::paintwidget(QWidget *parent) : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
}

void paintwidget::setTable(QTableWidget *t)
{
    table = t;
}

void paintwidget::paintEvent(QPaintEvent *)
{
    if(!table) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    for(int row = 0; row < table->rowCount(); row++)
    {
        int cx = table->item(row,1)->text().toInt();
        int cy = table->item(row,2)->text().toInt();
        int r = table->item(row,3)->text().toInt();
        int lineWidth = table->item(row,4)->text().toInt();

        QPushButton *lineBtn = qobject_cast<QPushButton*>(table->cellWidget(row,5));
        QPushButton *fillBtn = qobject_cast<QPushButton*>(table->cellWidget(row,6));

        QColor lineColor(lineBtn->property("r").toInt(),
                         lineBtn->property("g").toInt(),
                         lineBtn->property("b").toInt());

        QColor fillColor(fillBtn->property("r").toInt(),
                         fillBtn->property("g").toInt(),
                         fillBtn->property("b").toInt());

        if(row == selectedRow)
            lineColor = Qt::red;

        QPen pen(lineColor);
        pen.setWidth(lineWidth);
        painter.setPen(pen);
        painter.setBrush(fillColor);
        painter.drawEllipse(cx - r, cy - r, r*2, r*2);
    }
}

void paintwidget::mousePressEvent(QMouseEvent *event)
{
    if(!table) return;

    int x = event->pos().x();
    int y = event->pos().y();

    for(int row = 0; row < table->rowCount(); row++)
    {
        int cx = table->item(row,1)->text().toInt();
        int cy = table->item(row,2)->text().toInt();
        int r = table->item(row,3)->text().toInt();

        double dist = sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));

        if(dist <= r)
        {
            selectedRow = row;
            table->selectRow(row);
            update();
            break;
        }
    }
}

void paintwidget::keyPressEvent(QKeyEvent *event)
{
    if(selectedRow < 0) return;

    int r = table->item(selectedRow,3)->text().toInt();

    if(event->key() == Qt::Key_Plus)
        r += 5;

    if(event->key() == Qt::Key_Minus)
        r -= 5;

    if(r < 1) r = 1;

    table->item(selectedRow,3)->setText(QString::number(r));

    update();
}
