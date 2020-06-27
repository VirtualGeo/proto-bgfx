#ifdef GUI_QT

#include "qopenglwidgetbgfx.h"
#include <QDebug>
#include <QResizeEvent>
#include <QOpenGLContext>
#include <QCoreApplication>
//#include <QWGLNativeContext>

#if BX_PLATFORM_WINDOWS
#include <QtPlatformHeaders/QWGLNativeContext>
using NativeContext = QWGLNativeContext;
#elif BX_PLATFORM_LINUX
#include <QtPlatformHeaders/QGLXNativeContext>
using NativeContext = QGLXNativeContext;
#endif

//#include <QtPlatformHeaders/QGLXNativeContext>
//#include <QtPlatformHeaders/QNativ
#include <thread>
//#include <QGLContext>
#include <QOpenGLFunctions>


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
    std::cout << "[THREAD] initializeGL: " << std::this_thread::get_id() << std::endl;
    QOpenGLContext * qGLContext = QOpenGLContext::currentContext();
    bool success = qGLContext->nativeHandle().canConvert<NativeContext>();
    Q_ASSERT(success);
    NativeContext native = qGLContext->nativeHandle().value<NativeContext>();
//    void * native = qGLContext->nativeHandle().value<void *>();
    void * context = (void*)native.context();
//    void * context = (void*)native;

//    void * context = qGLContext->nativeHandle().data();
//    void * context = (void*)5555;
    context = (void*)5555;
    std::cout << "[CONTEXT] initializeGL: " << context << std::endl;
    GLint framebufferId = -1;
    QOpenGLFunctions * gl = qGLContext->functions();
    gl->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebufferId);

    void * backBuffer = (void*)framebufferId;

//    m_windowState = new WindowState((void*)(uintptr_t)winId(), nullptr, width(), height(), context);
    m_windowState = new WindowState(nullptr, nullptr, width(), height(), context);

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
    QOpenGLContext * currentContext = QOpenGLContext::currentContext();
    GLint framebufferId = -1;
    QOpenGLFunctions * gl = currentContext->functions();
    gl->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebufferId);

    GLint renderbufferId = -1;
//    QOpenGLContext * context = this->context();
//    std::cout << "[paintGL] context: " << context << std::endl;
//    QOpenGLFramebufferObject * fbo =
//    assert(m_windowState != nullptr);
    m_windowState->renderAllWindow();
//    makeCurrent();
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
