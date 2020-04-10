#ifdef GUI_QT

//#include "mainwindow.h"
#include <gui/mainwindow.h>

#include <QApplication>
#include <QSurface>

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setVersion(4, 1);
//    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setDepthBufferSize(24);
    format.setSamples(16);
    QSurfaceFormat::setDefaultFormat(format);


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    MainWindow w2;
//    w2.show();
    return a.exec();
}

#endif // GUI_QT
