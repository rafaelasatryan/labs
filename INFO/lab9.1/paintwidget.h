#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QTableWidget>
#include <QWidget>

class paintwidget : public QWidget
{
    Q_OBJECT
public:
    explicit paintwidget(QWidget *parent = nullptr);
    void setTable(QTableWidget *table);
protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
private:
    QTableWidget *table = nullptr;
    int selectedRow = -1;
};

#endif // PAINTWIDGET_H
