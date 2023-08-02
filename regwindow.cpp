#include "regwindow.h"
#include "ui_regwindow.h"
RegWindow::RegWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegWindow)
{
    ui->setupUi(this);
    connect(ui->btnEye_1, &QPushButton::clicked, this, &RegWindow::on_btnEye_1_clicked);
    connect(ui->btnEye_2, &QPushButton::clicked, this, &RegWindow::on_btnEye_2_clicked);
}

RegWindow::~RegWindow()
{
    delete ui;
}

void RegWindow::on_StartButton_clicked()
{
     QString MainPassword = ui->MainPassword->text();
     QString ConfirmPassword = ui->ConfirmPassword->text();
     QByteArray MPchar = MainPassword.toLatin1();
     const char *MPass = MPchar.data();
     SalvatoryHeader * SH = new SalvatoryHeader();
     int KeyLength = strlen(MainKey);
     int MasterPasswordLength = strlen(MPass);

     for (int i = 0; i <KeyMaxLength; i++)
     {
         if (i < KeyLength) SH->Key[i] = MainKey[i];
         else SH->Key[i] = 0;
     }

     for (int i = 0; i < MasterPasswordMaxLength; i++)
     {
         if (i < MasterPasswordLength) SH->MasterPassword[i] = MPass[i];
         else SH->MasterPassword[i] = 0;
     }

     std::fstream * SalvatoryFile = SalvatoryOpen(SalvatoryFileName, SH);


     if (MainPassword==ConfirmPassword)
     {

         //переход на след. окно
     }
     else
     {
         //надпись ошибка
     }
}

void RegWindow::on_btnEye_1_clicked(bool checked)
{
    if (checked)
        ui->MainPassword->setEchoMode(QLineEdit::Normal);
    else
        ui->MainPassword->setEchoMode(QLineEdit::Password);
}

void RegWindow::on_btnEye_2_clicked(bool checked)
{
    if (checked)
        ui->ConfirmPassword->setEchoMode(QLineEdit::Normal);
    else
        ui->ConfirmPassword->setEchoMode(QLineEdit::Password);
}
