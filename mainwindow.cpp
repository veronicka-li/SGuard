#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //инициалиализация окна cardwindow
    CreateCard = new CardWindow();
    connect(CreateCard, &CardWindow::firstWindow, this, &MainWindow::NameCardList);
    //CardList
    struct stat statBuff;

    if (!(stat(FileNameText, &statBuff) < 0))
    {
        int n;
        std::fstream TextFile = ride_string_number(n);
        char *StringName = new char[MaxNameCardLength];
        TextFile.getline(StringName, MaxNameCardLength);
        for (int i = 0; i < n; i++)
        {
          TextFile.getline(StringName, MaxNameCardLength);
          ui->CardList->addItem(QString::number(i+1) +". "+ StringName);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::NameCardList()
{
    //CardList
    struct stat statBuff;
    if (!(stat(FileNameText, &statBuff) < 0))
    {
        int n;
        std::fstream TextFile = ride_string_number(n);
        char *StringName = new char[MaxNameCardLength];
        ui->CardList->clear();
        TextFile.getline(StringName, MaxNameCardLength);
        for (int i = 0; i < n; i++)
        {
            TextFile.getline(StringName, MaxNameCardLength);
            ui->CardList->addItem(QString::number(i+1) +". "+ StringName);
        }
    }
   this->show();
}

void MainWindow::on_PlusButton_clicked()
{
    CreateCard->show();
    this->close();
}


void MainWindow::on_CardList_doubleClicked(const QModelIndex &index)
{
    QString StringName = ui->CardList->currentItem()->text();
    QByteArray Namechar = StringName.toLatin1();
    char *SName =  Namechar.data();
    char number=SName[0];
    int numder2=number-'0';

    for (int i = 1; SName[i]!='.';i++)
    {
         number = SName[i];
         numder2 =10*numder2 +(number-'0');
    }
    CreateCard->OutputText(numder2);
    CreateCard->show();
    this->close();
}
