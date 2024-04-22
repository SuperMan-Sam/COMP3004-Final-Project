#include "mainwindow.h"
#include "pcwindow.h"
#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //PCWindow pw;
    //pw.resize(600, 200);
    //pw.show();
    return a.exec();
}
