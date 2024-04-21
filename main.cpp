#include "mainwindow.h"
#include "pcwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    PCWindow pw;
    pw.show();
    return a.exec();
}
