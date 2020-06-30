#ifdef GUI_QT

//#include "mainwindow.h"
#include <gui/mainwindow.h>

#include <QApplication>
#include <QSurface>
//#include "windowstate.h"
//#include <thread>

//void renderLoop() {
//    MainWindow w(nullptr, 2);
//    w.show();
//    while (1) {
//        WindowState::renderAllWindow();
//    }
//}
//#include <QThread>
#include "entry.h"



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

//    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    QApplication a(argc, argv);
    MainWindow w(nullptr, 1);
    w.show();
//    MainWindow w2(nullptr, 2);
//    w2.show();

//    std::thread(&WindowState::renderAllWindow);
//    std::thread thread(renderLoop);
//    std::thread t(&QApplication::exec, &a);
//    QThread * thread = QThread::create([&a]{ a.exec();});
//    QThread * thread = QThread::create(renderLoop);
//    thread->start();
//    while (1) {
//        WindowState::renderAllWindow();
//    }
    return a.exec();
}

#endif // GUI_QT
