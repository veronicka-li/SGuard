#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include <mainwindow.h>
#include "database.h"

namespace Ui {
class AuthWindow;
}

class AuthWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AuthWindow(QWidget *parent = nullptr);
    ~AuthWindow();

private slots:
    void on_StartButton_clicked();
    void on_PasswordEdited(QString password);



private:
    Ui::AuthWindow *ui;
    MainWindow *sW;
};

#endif // AUTHWINDOW_H
