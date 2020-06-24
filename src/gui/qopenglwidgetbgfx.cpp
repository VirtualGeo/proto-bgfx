#ifdef GUI_QT

#include "qopenglwidgetbgfx.h"
#include <QDebug>
#include <QResizeEvent>
#include <QOpenGLContext>
#include <QCoreApplication>
//#include <QWGLNativeContext>
#include <QtPlatformHeaders/QWGLNativeContext>

QOpenGLWidgetBgfx::QOpenGLWidgetBgfx(QWidget *parent)
    : QOpenGLWidget(parent)
//    , WindowState((void*)(uintptr_t)winId(), nullptr, width(), height())
{

//    setAttribute(Qt::WA_NativeWindow);
//    setAttribute(Qt::WA_PaintOnScreen);
//    setAttribute(Qt::WA_NoSystemBackground);
//    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

//    setAttribute(Qt::WA_AlwaysStackOnTop);
//    setAttribute(Qt::WA_NoSystemBackground);
//    setAttribute(Qt::WA_TranslucentBackground);


//    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
//    setFocus();

}

void QOpenGLWidgetBgfx::initializeGL()
{
    QOpenGLContext * qGLContext = QOpenGLContext::currentContext();
    bool success = qGLContext->nativeHandle().canConvert<QWGLNativeContext>();
    Q_ASSERT(success);
    QWGLNativeContext native = qGLContext->nativeHandle().value<QWGLNativeContext>();
//    void * native = qGLContext->nativeHandle().value<void *>();
    void * context = (void*)native.context();

    m_windowState = new WindowState((void*)(uintptr_t)winId(), nullptr, width(), height(), context);

}

void QOpenGLWidgetBgfx::resizeGL(int w, int h)
{
    qDebug() << "QOpenGLWidgetBgfx::resizeEvent(" << w << h << ")";
    QOpenGLWidget::resizeGL(w, h);

    //   if (m_isInit == false) {
    //       init();
    //       return;
    //   }

//    const auto& size = event->size();
    const auto& size = QSize(w, h);
    Q_ASSERT(size.width() == width());

    if ((size.width() < 0) || (size.height() < 0)) {
        return;
    }

//    m_window->resizeEvent(size.width(), size.height());
    assert(m_windowState != nullptr);
    m_windowState->resizeEvent(size.width(), size.height());
    //    render();

}

void QOpenGLWidgetBgfx::paintGL()
{
    assert(m_windowState != nullptr);
    m_windowState->renderAllWindow();
    update();
}

//void QOpenGLWidgetBgfx::paintEvent(QPaintEvent *e)
//{
// 	makeCurrent();
//	doneCurrent();
////    WindowState::renderAllWindow();
////    update();
//}

//void QOpenGLWidgetBgfx::resizeEvent(QResizeEvent *event)
//{
//}

#endif // GUI_QT
