#ifdef GUI_QT

//#include "mainwindow.h"
#include <gui/mainwindow.h>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    MainWindow w2;
//    w2.show();
    return a.exec();
}

#endif // GUI_QT
