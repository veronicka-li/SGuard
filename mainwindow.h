#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cardwindow.h"
#include "database.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void NameCardList();
    ~MainWindow();
signals:
    void firstWindow();


private slots:
    void on_PlusButton_clicked();

    void on_CardList_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    CardWindow *CreateCard;
};

#endif // MAINWINDOW_H
