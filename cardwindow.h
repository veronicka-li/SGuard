#ifndef CARDWINDOW_H
#define CARDWINDOW_H

#include <QWidget>
#include "database.h"
#include <consentwindow.h>
#include <QLineEdit>
#include <QDebug>

namespace Ui {
class CardWindow;
}

class CardWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CardWindow(QWidget *parent = nullptr);
    void OutputText(int NumberInFile);
    void DeleteCard();
    ~CardWindow();

signals:
    void firstWindow();
private slots:
    void on_CircleButton_clicked();

    void on_CancelButton_clicked();

    void on_TrashButton_clicked();

    void on_EyeButton_clicked();

    void on_Back_clicked();

    void on_Copy_4_clicked();

    void on_Copy_3_clicked();

    void on_Copy_2_clicked();

    void on_Copy_clicked();

    void on_Generator_clicked();

 void Check_Password(QString password);
 void Check_Login(QString login);
 void Check_URL(QString url);
 void Check_Name(QString name);

private:
    Ui::CardWindow *ui;

    bool FlagCreate = 0;
    bool FlagChecked = 0;
    int NumrInFile;

    char *CardWindowName = new char[MaxStringCard];
    char *CardWindowURL = new char[MaxStringCard];
    char *CardWindowLogin = new char[MaxStringCard];
    char *CardWindowPassword = new char[MaxStringCard];

    SalvatoryHeader * SH1;
    std::fstream * SalvatoryFile1;

    ConsentWindow *Consent;
};

#endif // CARDWINDOW_H
