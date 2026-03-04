#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QPushButton;
class QTableWidget;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void loadFromFile(const QString& path);
private slots:
    void buttonInfo();
    void saveToFile();
    void slotAdd();
    void slotDelete();
    void onHeaderClicked(int col);
    void onCellChanged(int row, int col);
private:
    QPushButton *openInfo;
    QPushButton *saveInfo;
    QPushButton *Add;
    QPushButton *Delete;
    QTableWidget *table;
    QPushButton *createColorButton(int r, int g, int b);
};

#endif // MAINWINDOW_H
