//#include "mainwindow.h"
#include "src/MainWindow/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.updateLabelImageSize();
    return a.exec();
}
