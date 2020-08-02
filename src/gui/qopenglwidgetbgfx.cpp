#ifdef GUI_QT

#include "qopenglwidgetbgfx.h"
#include <QDebug>
#include <QResizeEvent>
#include <QOpenGLContext>
#include <QCoreApplication>
//#include <QWGLNativeContext>
#include <bgfx/platform.h>

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
#include <QOpenGLExtraFunctions>

#define GL_CHECK() assert(gl->glGetError() == 0);

static std::map<int, MouseButton::Enum> s_translateMouseKey = {
    { Qt::LeftButton, MouseButton::Left },
    { Qt::RightButton, MouseButton::Right },
    { Qt::MiddleButton, MouseButton::Middle },
};
//static const std::map<int, Key::Enum> s_translateKey = {
static std::map<int, Key::Enum> s_translateKey = {
    { Qt::Key_Up, Key::Up },
    { Qt::Key_Down, Key::Down },
    { Qt::Key_Right, Key::Right },
    { Qt::Key_Left, Key::Left },
    { Qt::Key_PageUp, Key::PageUp },
    { Qt::Key_PageDown, Key::PageDown },
    { Qt::Key_F1, Key::F1 },
    { Qt::Key_F2, Key::F2 },
    { Qt::Key_F3, Key::F3 },
    { Qt::Key_F4, Key::F4 },
    { Qt::Key_F5, Key::F5 },
    { Qt::Key_F6, Key::F6 },
    { Qt::Key_F7, Key::F7 },
    { Qt::Key_F8, Key::F8 },
    { Qt::Key_F9, Key::F9 },
    { Qt::Key_F10, Key::F10 },
    { Qt::Key_F11, Key::F11 },
    { Qt::Key_F12, Key::F12 },
    { Qt::Key_N, Key::N },
    { Qt::Key_M, Key::M },
    { Qt::Key_R, Key::R },
    { Qt::Key_E, Key::E },
    { Qt::Key_Control, Key::Control },
};



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


    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    setFocus();


}

void QOpenGLWidgetBgfx::initializeGL()
{
    std::cout << "[THREAD] initializeGL: " << std::this_thread::get_id() << std::endl;
    QOpenGLContext * qGLContext = QOpenGLContext::currentContext();
    bool success = qGLContext->nativeHandle().canConvert<NativeContext>();
    Q_ASSERT(success);
    NativeContext native = qGLContext->nativeHandle().value<NativeContext>();
//    void * native = qGLContext->nativeHandle().value<void *>();
    void * nativeGlContext = (void*)native.context();
//    void * context = (void*)native;


//    void * context = qGLContext->nativeHandle().data();
//    void * context = (void*)5555;
//    context = (void*)5555;
    std::cout << "[CONTEXT] initializeGL: " << nativeGlContext << std::endl;
//    GLint framebufferId = -1;
//    QOpenGLFunctions * gl = qGLContext->functions();
//    gl->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebufferId);

//    void * backBuffer = (void*)framebufferId;

//    m_windowState = new WindowState((void*)(uintptr_t)winId(), nullptr, width(), height());
//    m_windowState = new WindowState(nullptr, nullptr, width(), height(), nativeGlContext);
    m_windowState = new WindowState(nullptr, nullptr, width(), height());


//    m_backBuffer = bgfx::createTexture2D(width(), height(), false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_RT, NULL);
//    m_depthBuffer = bgfx::createTexture2D(width(), height(), false, 1, bgfx::TextureFormat::D24S8, BGFX_TEXTURE_RT, NULL);
//    bgfx::TextureHandle fbtextures[2] = {m_backBuffer, m_depthBuffer};
//    m_windowState->m_offScreenFBH = bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, false);
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

    if (bgfx::isValid(m_backBuffer)) {
        bgfx::destroy(m_backBuffer);
        m_backBuffer = BGFX_INVALID_HANDLE;
    }
    if (bgfx::isValid(m_depthBuffer)) {
        bgfx::destroy(m_depthBuffer);
        m_depthBuffer = BGFX_INVALID_HANDLE;
    }
    m_backBuffer = bgfx::createTexture2D(w, h, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_RT, NULL);
    m_depthBuffer = bgfx::createTexture2D(w, h, false, 1, bgfx::TextureFormat::D24S8, BGFX_TEXTURE_RT, NULL);

    bgfx::TextureHandle fbtextures[2] = {m_backBuffer, m_depthBuffer};
    m_windowState->m_offScreenFBH = bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, false);
}

void QOpenGLWidgetBgfx::paintGL()
{
    QOpenGLContext * currentContext = QOpenGLContext::currentContext();

    GLint qtFbo = -1;
    QOpenGLExtraFunctions * gl = currentContext->extraFunctions();
    gl->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &qtFbo); GL_CHECK();

//    std::cout << "[OpenGLWidgetBgfx] GL_FRAMEBUFFER_BINDING Qt: " << qtFbo << std::endl;
//    GLint renderbufferId = -1;
//    QOpenGLContext * context = this->context();
//    std::cout << "[paintGL] context: " << context << std::endl;
//    QOpenGLFramebufferObject * fbo =
//    assert(m_windowState != nullptr);
    m_windowState->renderAllWindow();

//    uintptr_t backBufferId = bgfx::getInternal(m_backBuffer);
    uintptr_t backBufferId = 0;

//    uintptr_t srcFB = bgfx::getInternal(m_windowState->m_offScreenFBH);
//    assert(gl->glIsFramebuffer(srcFB));

//    gl->glBindFramebuffer(GL_FRAMEBUFFER, srcFB);

//    GLuint backBufferId;
//    GLint srcFB;
//    gl->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &srcFB);

    GLuint srcFB;
    gl->glGenFramebuffers(1, &srcFB); GL_CHECK();
    gl->glBindFramebuffer(GL_FRAMEBUFFER, srcFB); GL_CHECK();
    gl->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, backBufferId, 0); GL_CHECK();
    GLenum status = gl->glCheckFramebufferStatus(GL_FRAMEBUFFER); GL_CHECK();
    assert(status == GL_FRAMEBUFFER_COMPLETE);
    gl->glBindFramebuffer(GL_READ_FRAMEBUFFER, srcFB); GL_CHECK();

//    gl->glBindFramebuffer(GL_READ_FRAMEBUFFER, backBufferId);
    gl->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, qtFbo); GL_CHECK();
    gl->glBlitFramebuffer(0, 0, width(), height(), 0, 0, width(), height(), GL_COLOR_BUFFER_BIT, GL_NEAREST); GL_CHECK();
    gl->glBindFramebuffer(GL_FRAMEBUFFER, qtFbo); GL_CHECK();
//    gl->glGetError();

    gl->glDeleteFramebuffers(1, &srcFB); GL_CHECK();


//    std::cout << "[QOpenGLWidgetBgfx] attch0:" << attch0 << std::endl;
//    makeCurrent();
    update();
}

void QOpenGLWidgetBgfx::keyPressEvent(QKeyEvent *event)
{
    m_windowState->keyPressEvent(s_translateKey[event->key()]);
}

void QOpenGLWidgetBgfx::keyReleaseEvent(QKeyEvent *event)
{
    m_windowState->keyReleaseEvent(s_translateKey[event->key()]);
}

void QOpenGLWidgetBgfx::mousePressEvent(QMouseEvent *event)
{
    m_windowState->mousePressEvent(s_translateMouseKey[event->button()]);
}

void QOpenGLWidgetBgfx::mouseReleaseEvent(QMouseEvent *event)
{
    m_windowState->mouseReleaseEvent(s_translateMouseKey[event->button()]);
}

void QOpenGLWidgetBgfx::mouseMoveEvent(QMouseEvent *event)
{
    m_windowState->mouseMoveEvent(event->x(), event->y());
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
