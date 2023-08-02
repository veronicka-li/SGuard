#include "cardwindow.h"
#include "ui_cardwindow.h"
#include <QDebug>
#include "mainwindow.h"
#include <QClipboard>
CardWindow::CardWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CardWindow)


{
    ui->setupUi(this);

    ui->lblLogin->setText("");
    ui->lblURL->setText("");
    ui->lblPass->setText("");
    ui->lblName->setText("");

    connect(ui->Copy_4, &QPushButton::clicked, this, &CardWindow::on_Copy_4_clicked);
    connect(ui->Copy, &QPushButton::clicked, this, &CardWindow::on_Copy_clicked);
    connect(ui->Copy_2, &QPushButton::clicked, this, &CardWindow::on_Copy_2_clicked);
    connect(ui->Copy_3, &QPushButton::clicked, this, &CardWindow::on_Copy_3_clicked);
    Consent = new ConsentWindow();
    connect(Consent, &ConsentWindow::firstWindow, this, &CardWindow::DeleteCard);

}

CardWindow::~CardWindow()
{
    delete ui;
}


void CardWindow::DeleteCard()
{
    //удалить карточку, очистить поля
    bool delete1 = Consent-> DeleteThisCard;
    if(delete1)
    {
        qDebug() << "a"<<NumrInFile;

        txt_delete_card(NumrInFile);
        qDebug() << "ba";
        InstanceDelete(SalvatoryFile1, NumrInFile,  *SH1);
        qDebug() << "b";
        ui->CardName->clear();
        ui->URL->clear();
        ui->Login->clear();
        ui->Password->clear();
        this->close();
        qDebug() << "c";
        FlagCreate=0;
        emit firstWindow();
        qDebug() << "d";
    }
    else
        this->show();
}

void CardWindow::OutputText(int NumberInFile)
{
    ui->Password->setEchoMode(QLineEdit::Password);
    FlagCreate=1;
    NumberInFile--;
    NumrInFile=NumberInFile;
    int CodeStreamLength;
    int StringCount = 4;
    unsigned char MPass[17];
    unsigned char * OutputCodeStream;
    int j=0,i = 0, divideWord;

    SalvatoryHeader * SH = new SalvatoryHeader();
    std::fstream * SalvatoryFile = SalvatoryOpen(SalvatoryFileName, SH);

    SH1=SH;
    SalvatoryFile1=SalvatoryFile;
    unsigned char * CodeStream = InstanceRead(SalvatoryFile, NumberInFile, CodeStreamLength);
    InstanceDecription(CodeStream, CodeStreamLength);
    char **StringM1 = Decoder((char*)CodeStream, StringCount);

        int Length=strlen(StringM1[0]);
        for (int j=0;j<Length;j++)
             CardWindowName[j]=StringM1[0][j];
        CardWindowName[Length]='\0';
    ui->CardName->setText((QString)(const char*)StringM1[0]);

    Length=strlen(StringM1[1]);
    for (int j=0;j<Length;j++)
         CardWindowURL[j]=StringM1[1][j];
    CardWindowURL[Length]='\0';
    ui->URL->setText((QString)(const char*)StringM1[1]);

    Length=strlen(StringM1[2]);
    for (int j=0;j<Length;j++)
         CardWindowLogin[j]=StringM1[2][j];
    CardWindowLogin[Length]='\0';
    ui->Login->setText((QString)(const char*)StringM1[2]);

    int MasterPasswordLength = strlen(StringM1[3]);
    for (int i = 0; i < MasterPasswordMaxLength; i++)
    {
        if (i < MasterPasswordLength)
            MPass[i]=StringM1[3][i];
        else MPass[i] = 0;
    }
    decryptionAndGlue(MainKey, MPass);
    Length=strlen((const char*)MPass);
    for (int j=0;j<Length;j++)
         CardWindowPassword[j]=MPass[j];
    CardWindowPassword[Length]='\0';
    ui->Password->setText((QString)(const char*)MPass);
    delete SH;
}

void CardWindow::on_CircleButton_clicked()
{
    SalvatoryHeader * SH = new SalvatoryHeader();
    std::fstream * SalvatoryFile = SalvatoryOpen(SalvatoryFileName, SH);
    bool FlagModify=0, FlagModifyNameCard=0;
    if (FlagCreate) //равен 1 при изменении или просмотре карточки
    {
        //изменение имени

         QString NameString = ui->CardName->text();
         QByteArray NSchar = NameString.toLatin1();
         char *Name = NSchar.data();

         int length_in_class=strlen(CardWindowName);
         int length_in_window=strlen(Name);

         if (length_in_class==length_in_window)
         {
             for(int i=0;i<length_in_class;i++)
             {
                 if (CardWindowName[i]!=Name[i])
                 {
                     FlagModify = 1;
                     FlagModifyNameCard=1;
                 }
             }
         }
         else
         {
             FlagModify = 1;
             FlagModifyNameCard=1;
         }

         //URL

         QString URLString = ui->URL->text();
         QByteArray URLchar = URLString.toLatin1();
         char *URL = URLchar.data();
         length_in_class=strlen(CardWindowURL);
         length_in_window=strlen(URL);

         if (length_in_class==length_in_window)
         {
             for(int i=0;i<length_in_class;i++)
             if (CardWindowURL[i]!=URL[i])
                 FlagModify = 1;
         }
         else
             FlagModify = 1;

         //логин

         QString LoginString = ui->Login->text();
         QByteArray Lchar = LoginString.toLatin1();
         char *Login = Lchar.data();
         length_in_class=strlen(CardWindowLogin);
         length_in_window=strlen(Login);

         if (length_in_class==length_in_window)
         {
             for(int i=0;i<length_in_class;i++)
             if (CardWindowLogin[i]!=Login[i])
                 FlagModify = 1;
         }
         else
             FlagModify = 1;


         //пароль

         QString PassString = ui->Password->text();
         QByteArray Pchar = PassString.toLatin1();
         char *Password = Pchar.data();
         length_in_class=strlen(CardWindowPassword);
         length_in_window=strlen(Password);

         if (length_in_class==length_in_window)
         {
             for(int i=0;i<length_in_class;i++)
             if (CardWindowPassword[i]!=Password[i])
                 FlagModify = 1;
         }
         else
             FlagModify = 1;
    }
    if(!FlagCreate||FlagModify)   //при создании карточки или изменении
    {
        qDebug() << "1";

        char **MasCard = new char *[4];
        int StringCount=4, CodeStreamLength;

        QString NameCard = ui->CardName->text();
        QByteArray NCard =NameCard.toLatin1();
        MasCard[0] = NCard.data();

        QString URLCard = ui->URL->text();
        QByteArray UCard =URLCard.toLatin1();
        MasCard[1] = UCard.data();

        QString LoginCard = ui->Login->text();
        QByteArray LCard =LoginCard.toLatin1();
        MasCard[2]  = LCard.data();

        QString PasswordCard = ui->Password->text();
        QByteArray PCard =PasswordCard.toLatin1();
        MasCard[3]  = PCard.data();
  qDebug() << "2";
        int MasterPasswordLength = strlen(MasCard[3]);
        unsigned char MPass[17];

        bool FlagEng = 0;
        int j=0;
        int dlina = NameCard.size();
        while(j<dlina)
        {
            if ((NameCard[j]<'!')||(NameCard[j]>'z'))
                FlagEng=1;
            j++;
        }
        j=0;
          qDebug() << "3";
        dlina = URLCard.size();
        while(j<dlina)
        {
            if ((URLCard[j]<'!')||(URLCard[j]>'z'))
                FlagEng=1;
            j++;
        }
        j=0;
        dlina = LoginCard.size();
          qDebug() << "4";
        while(j<dlina)
        {
            if ((LoginCard[j]<'!')||(LoginCard[j]>'z'))
                FlagEng=1;
            j++;
        }
        j=0;
        dlina = PasswordCard.size();

        while(j<dlina)
        {
            if ((PasswordCard[j]<'!')||(PasswordCard[j]>'z'))
                FlagEng=1;
            j++;
        }

        if ((MasterPasswordLength>3) && (MasterPasswordLength<17)&& NameCard.size()<31 && URLCard.size()< 151 && LoginCard.size()<151 && !FlagEng)
        {
            for (int i = 0; i < MasterPasswordMaxLength; i++)
            {
                if (i < MasterPasswordLength)
                    MPass[i]=MasCard[3][i];
                else MPass[i] = 0;
            }
              qDebug() << "7";
           encryptionAndCut(MainKey, MPass);
           MasterPasswordLength = strlen((char*)MPass);
            for (int i=0;i<MasterPasswordMaxLength;i++)
            {
                if (i < MasterPasswordLength)
                    MasCard[3][i]=(char)MPass[i];
                else
                    MasCard[3][i]=0;
            }
qDebug() << "8";
            unsigned char *CodeStream = (unsigned char *)Coder(MasCard, StringCount, CodeStreamLength);
            InstanceEncription(CodeStream, CodeStreamLength);
            if(FlagModify)
            {
                TextModify(NumrInFile, FlagModifyNameCard, MasCard[0]);
                InstanceModify(SalvatoryFile1, NumrInFile,  *SH1, CodeStream, CodeStreamLength);
            }
            else
            {
                InstanceAdd(SalvatoryFile,  *SH, CodeStream, CodeStreamLength);
                write_string(MasCard[0]);
            }
            qDebug() << "9";
            this->close();
            ui->CardName->clear();
            ui->URL->clear();
            ui->Login->clear();
            ui->Password->clear();
            FlagCreate=0;
            qDebug() << "10";
            emit firstWindow();

            delete[] MasCard;

            qDebug() << "11";
        }
        qDebug() << "fine";
    }
    else
    {
        this->close();
        ui->CardName->clear();
        ui->URL->clear();
        ui->Login->clear();
        ui->Password->clear();
        FlagCreate=0;
        emit firstWindow();
    }
    qDebug() << "8";
}

void CardWindow::on_CancelButton_clicked()
{
    //очистить поля

    this->close();
    ui->CardName->clear();
    ui->URL->clear();
    ui->Login->clear();
    ui->Password->clear();
    FlagCreate=0;
    emit firstWindow();
}

void CardWindow::on_TrashButton_clicked()
{
    if (FlagCreate) //равен 1 при изменении или просмотре карточки
    {
        //удалить карточку, очистить поля
        Consent->show();
        this->close();
    }
    else    //при создании карточки
    {
        //очистить поля
        ui->CardName->clear();
        ui->URL->clear();
        ui->Login->clear();
        ui->Password->clear();
        this->close();
        FlagCreate=0;
        emit firstWindow();
    }
}

void CardWindow::on_EyeButton_clicked()
{
    FlagChecked=!FlagChecked;
    if (FlagChecked)
        ui->Password->setEchoMode(QLineEdit::Normal);
    else
        ui->Password->setEchoMode(QLineEdit::Password);
}

void CardWindow::on_Back_clicked()
{
    this->close();
    ui->CardName->clear();
    ui->URL->clear();
    ui->Login->clear();
    ui->Password->clear();
    FlagCreate=0;
    this->close();
    emit firstWindow();
}

void CardWindow::on_Copy_4_clicked()
{
    QApplication::clipboard()->setText(ui->CardName->text());
}

void CardWindow::on_Copy_3_clicked()
{
     QApplication::clipboard()->setText(ui->URL->text());
}

void CardWindow::on_Copy_2_clicked()
{
    QApplication::clipboard()->setText(ui->Login->text());
}

void CardWindow::on_Copy_clicked()
{
    QApplication::clipboard()->setText(ui->Password->text());
}

void CardWindow::on_Generator_clicked()
{
 int passLenght = rand() % 4 + 12;
 int numOfSmallChars;
 int numOfBigChars;
 int numOfNumbers;
 char * password;
 srand(time(NULL));
 password = new char[passLenght + 1];

 numOfSmallChars = rand() % (passLenght - 2) + 1;
 int charRandEnd = passLenght - numOfSmallChars;
 numOfBigChars = rand() % (charRandEnd - 1) + 1;
 numOfNumbers = passLenght - numOfSmallChars - numOfBigChars;
         for (int i = 0; i<numOfNumbers; ++i) {
             password[i] = char(rand() % 10 + 48);
         }
         for (int i = numOfNumbers; i<numOfNumbers + numOfBigChars; ++i) {
             password[i] = char(rand() % 26 + 65);
         }

         for (int i = numOfNumbers + numOfBigChars; i<passLenght; ++i) {
             password[i] = char(rand() % 26 + 97);
         }

                 std::random_shuffle(password, password + passLenght);  //изменяем порядок элементов в пароле
                 password[passLenght] = '\0';
                 ui->Password->setText((QString)(const char*)password);
                 delete [] password;


}

void CardWindow::Check_Password(QString password)
{
     ui->lblPass->setText("");
      if ((password.size() < 4) || (password.size() > 16))
        ui->lblPass->setText("Incorrect password length.");
    else
    {
        if ((password.size() >= 4) && ( password.size()<= 8))
        ui->lblPass->setText("A simple password.");
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
                            ui->lblPass->setText("A simple password.");

                        if (((z == 1) || (z == 2)) && (z != dlina))
                            ui->lblPass->setText("A simple password.");
                        else

                            if ((z > 2) && (z != dlina))
                               ui->lblPass->setText("A complicated password.");
                            else
                            {
                                if (((V != 0) && (v != 0) && (n==0)) || ((v != 0) && (n != 0) && (V==0)) || ((V != 0) && (n != 0) && (v==0)) )
                                    ui->lblPass->setText("The average password.");
                                if ((V != 0) && (v != 0) && (n != 0))
                                   ui->lblPass->setText("A complicated password.");
                            }
                    }
                    else
                    if ((dlina >= 13) && (dlina <= 16))
                    {
                        if ((n == dlina) || (V == dlina) || (v == dlina) )
                           ui->lblPass->setText("The average password.");
                        if(z!=0)
                           ui->lblPass->setText("A complicated password.");
                        else
                        {
                            if ((V != 0) && (v != 0))
                                ui->lblPass->setText("A complicated password.");
                            if (((V != 0) && (v == 0) && (n != 0) ) || ((V == 0) && (v != 0) && (n != 0) ) )
                                ui->lblPass->setText("The average password.");

                        }
                    }

                }
    }
}
void CardWindow::Check_Name(QString name)
{
    if(name.size()>30)
        ui->lblName->setText("Incorrect name size");
    else     ui->lblName->setText("");
}
void CardWindow::Check_URL(QString url)
{
    if(url.size()>150)
        ui->lblURL->setText("Incorrect URL size");
    else    ui->lblURL->setText("");

}
void CardWindow::Check_Login(QString login)
{
    if(login.size()>150)
        ui->lblLogin->setText("Incorrect login size");
    else ui->lblLogin->setText("");

}
