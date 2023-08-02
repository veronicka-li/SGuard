#include "consentwindow.h"
#include "ui_consentwindow.h"
#include "cardwindow.h"

ConsentWindow::ConsentWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConsentWindow)
{
    ui->setupUi(this);
}


ConsentWindow::~ConsentWindow()
{
    delete ui;
}
void ConsentWindow::on_OK_clicked()
{
    DeleteThisCard=1;
    this->close();
    emit firstWindow();
}

void ConsentWindow::on_Cancel_clicked()
{
    DeleteThisCard=0;
    this->close();
    emit firstWindow();
}
