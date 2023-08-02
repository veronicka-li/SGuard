#pragma once
#ifndef REGWINDOW_H
#define REGWINDOW_H

#include <QWidget>
#include "database.h"

namespace Ui {
class RegWindow;
}

class RegWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RegWindow(QWidget *parent = nullptr);
    ~RegWindow();

private slots:
    void on_StartButton_clicked();
    void on_btnEye_1_clicked(bool checked);
    void on_btnEye_2_clicked(bool checked);

private:
    Ui::RegWindow *ui;
};

#endif // REGWINDOW_H
