#ifdef GUI_QT

//#include "mainwindow.h"
#include <gui/mainwindow.h>

#include <QApplication>
#include <QSurface>

int main(int argc, char *argv[])
{
//    QSurfaceFormat format;
//    format.setVersion(4, 1);
////    format.setProfile(QSurfaceFormat::CoreProfile);
//    format.setProfile(QSurfaceFormat::CompatibilityProfile);
//    format.setDepthBufferSize(24);
//    format.setSamples(8);
//    QSurfaceFormat::setDefaultFormat(format);

//    QSurfaceFormat lDefault = QSurfaceFormat::defaultFormat();
//    lDefault.setSwapInterval(0);
//    QSurfaceFormat::setDefaultFormat(lDefault);


    QApplication a(argc, argv);
    MainWindow w(nullptr, 1);
    w.show();
//    MainWindow w2(nullptr, 2);
//    w2.show();
    return a.exec();
}

#endif // GUI_QT
