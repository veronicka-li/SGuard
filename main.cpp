#include "authwindow.h"
#include <QApplication>
//#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AuthWindow w;
    w.show();
    return a.exec();
}
