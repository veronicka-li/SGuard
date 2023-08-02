#include "authwindow.h"
#include "ui_authwindow.h"
#include <QDebug>

AuthWindow::AuthWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AuthWindow)
{
    ui->setupUi(this);
    sW=new MainWindow();
    ui->lblAuth->setText("");
    connect(sW,&MainWindow::firstWindow,this,&AuthWindow::show);
    connect(ui->btnEye, &QPushButton::clicked, this, [=](bool checked){
        if (checked)
            ui->MainPassword->setEchoMode(QLineEdit::Normal);
        else
            ui->MainPassword->setEchoMode(QLineEdit::Password);
    });


 }

AuthWindow::~AuthWindow()
{
    delete ui;
}

void AuthWindow::on_StartButton_clicked()
{
    char MasterPassword[16];
    QString MainPassword = ui->MainPassword->text();
    QByteArray MPchar = MainPassword.toLatin1();
    char *MPass1 = MPchar.data();
    //проверка пароля на содержание русских букв
    bool FlagEng = 0;
    int j=0;
    int dlina = MainPassword.size();
    while(j<dlina)
    {
        if ((MainPassword[j]<'!')||(MainPassword[j]>'z'))
            FlagEng=1;
        j++;
    }

    unsigned char MPass[17];
    SalvatoryHeader * SH = new SalvatoryHeader();
    struct stat statBuff;
    if(!FlagEng)
    {
    if (stat(SalvatoryFileName, &statBuff) < 0)
    {
        int KeyLength = strlen(MainKey);
        int MasterPasswordLength = strlen(MPass1);
        if ((MasterPasswordLength>=4) && (MasterPasswordLength<=16))
        {
            for (int i = 0; i <KeyMaxLength; i++)
            {
                if (i < KeyLength) SH->Key[i] = MainKey[i];
                else SH->Key[i] = 0;
            }

            for (int i = 0; i < MasterPasswordMaxLength; i++)
            {
                if (i < MasterPasswordLength)
                    MPass[i]=MPass1[i];
                else MPass[i] = 0;
            }
            encryptionAndCut(MainKey, MPass);
            MasterPasswordLength = strlen((char*)MPass);
            for (int i = 0; i < MasterPasswordMaxLength; i++)
            {
                if (i < MasterPasswordLength) SH->MasterPassword[i] = MPass[i];
                else SH->MasterPassword[i] = 0;
            }
            std::fstream * SalvatoryFile = SalvatoryOpen(SalvatoryFileName, SH);

            sW->show();
            this->close();
        }

    }

    else    //пользователь не в первый раз зашел в приложение
    {
       std::fstream * SalvatoryFile = SalvatoryOpen(SalvatoryFileName, SH);
       GetMainPass (SalvatoryFile, MasterPassword);
       decryptionAndGlue(MainKey, (unsigned char*)MasterPassword);

       if (!strcmp((char *)MPass1,(char *)MasterPassword))
        {
              sW->show();
              this->close();
              delete SH;
        }
        else
        {
           ui->lblAuth->setText("The password is incorrect. Try again.");
        }
    }
    }
    else
        ui->lblAuth->setText("The password is incorrect. Try again.");
}

void AuthWindow::on_PasswordEdited(QString password)
{
    if ((password.size() < 4) || (password.size() > 16))
        ui->lblAuth->setText("Incorrect password length.");
    else
    {
        if ((password.size() >= 4) && ( password.size()<= 8))
        ui->lblAuth->setText("A simple password.");
        else
                {

            int i=0,n=0,v=0,V=0,z=0,dlina;
                dlina = password.size();
                    while (i <= dlina)
                    {
                        if (('0' <= password[i]) && (password[i] <= '9'))
                            n++; //счетчик цифр в пароле

                        if (('A' <= password[i]) && (password[i] <= 'Z'))
                            V++; // счетчик заглавных букв в английском языке

                        if (('a' <= password[i]) && (password[i] <= 'z'))
                            v++;// счетчик строчных в англе

                        if (((' ' <= password[i]) && (password[i] <= '/')) || ((':' <= password[i]) && (password[i] <= '@')) || (('[' <= password[i]) && (password[i] <= '`')) || (password[i] == '~') || (password[i] == '№') || (password[i] == '{') || (password[i] == '}'))
                            z++;//счетчик знаков

                        i++;// счетчик цикла
                    }

                    if ((dlina >= 9) && (dlina <= 12))
                    {


                        if ((n == dlina) || (V == dlina)  || (v == dlina)  || (z == dlina))
                            ui->lblAuth->setText("A simple password.");

                        if (((z == 1) || (z == 2)) && (z != dlina))
                            ui->lblAuth->setText("A simple password.");
                        else

                            if ((z > 2) && (z != dlina))
                               ui->lblAuth->setText("A complicated password.");
                            else
                            {
                                if (((V != 0) && (v != 0) && (n==0)) || ((v != 0) && (n != 0) && (V==0)) || ((V != 0) && (n != 0) && (v==0)) )
                                    ui->lblAuth->setText("The average password.");
                                if ((V != 0) && (v != 0) && (n != 0))
                                   ui->lblAuth->setText("A complicated password.");
                            }
                    }
                    else
                    if ((dlina >= 13) && (dlina <= 16))
                    {
                        if ((n == dlina) || (V == dlina) || (v == dlina) )
                           ui->lblAuth->setText("The average password.");
                        if(z!=0)
                           ui->lblAuth->setText("A complicated password.");
                        else
                        {
                            if ((V != 0) && (v != 0))
                                ui->lblAuth->setText("A complicated password.");
                            if (((V != 0) && (v == 0) && (n != 0) ) || ((V == 0) && (v != 0) && (n != 0) ) )
                                ui->lblAuth->setText("The average password.");

                        }
                    }

                }
    }

}




