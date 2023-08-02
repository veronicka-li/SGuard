#ifndef CONSENTWINDOW_H
#define CONSENTWINDOW_H

#include <QWidget>
#include <QLineEdit>

namespace Ui {
class ConsentWindow;
}

class ConsentWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ConsentWindow(QWidget *parent = 0);
    bool CheckDelete ();
    bool DeleteThisCard;
    ~ConsentWindow();
signals:
    void firstWindow();
private slots:
    void on_OK_clicked();

    void on_Cancel_clicked();

private:
    Ui::ConsentWindow *ui;
};

#endif // CONSENTWINDOW_H
