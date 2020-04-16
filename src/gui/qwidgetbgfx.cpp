#ifdef GUI_QT

#include "qwidgetbgfx.h"

#include <QDebug>
#include <QResizeEvent>
#include <QApplication>
#include <QSurface>

static std::map<int, MouseButton::Enum> s_translateMouseKey;
static std::map<int, Key::Enum> s_translateKey;


QWidgetBgfx::QWidgetBgfx(QWidget* parent)
    : QWidget(parent)
    , m_iWindow(WindowState::s_windows.size())
{
    if (m_iWindow == 0) {
        s_translateMouseKey[Qt::LeftButton] = MouseButton::Left;
        s_translateMouseKey[Qt::RightButton] = MouseButton::Right;
        s_translateMouseKey[Qt::MiddleButton] = MouseButton::Middle;

        s_translateKey[Qt::Key_Up] = Key::Up;
        s_translateKey[Qt::Key_Down] = Key::Down;
        s_translateKey[Qt::Key_Right] = Key::Right;
        s_translateKey[Qt::Key_Left] = Key::Left;
        s_translateKey[Qt::Key_PageUp] = Key::PageUp;
        s_translateKey[Qt::Key_PageDown] = Key::PageDown;
        s_translateKey[Qt::Key_F1] = Key::F1;
        s_translateKey[Qt::Key_F2] = Key::F2;
        s_translateKey[Qt::Key_F3] = Key::F3;
        s_translateKey[Qt::Key_F4] = Key::F4;
        s_translateKey[Qt::Key_F5] = Key::F5;
        s_translateKey[Qt::Key_N] = Key::N;
        s_translateKey[Qt::Key_M] = Key::M;
        s_translateKey[Qt::Key_R] = Key::R;
        s_translateKey[Qt::Key_Control] = Key::Control;
    }


    void* nwh = (void*)(uintptr_t)winId();
    WindowState::s_windows.push_back(WindowState(nwh, width(), height()));

    m_window = &WindowState::s_windows.back();


    qDebug() << "QWidgetBgfx(" << parent << ")";
    setAttribute(Qt::WA_NativeWindow);
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    setFocus();
}

QWidgetBgfx::~QWidgetBgfx()
{
}

//void QWidgetBgfx::render()
//{
//    if (m_updatePending == false) {
//        m_updatePending = true;
//        QApplication::postEvent(this, new QEvent{QEvent::UpdateRequest});
//    }

//}

//void QWidgetBgfx::doRender()
//{
//    if (isVisible() == false)
//        return;
//    if (m_isInit == false)
//        return;

//    if (m_continuousRender == true)
//        render();
//}

//void QWidgetBgfx::init()
//{
////    m_isInit = true;
//}

void QWidgetBgfx::paintEvent(QPaintEvent* event)
{
    m_window->render();
    update();
}

//void QWidgetBgfx::showEvent(QShowEvent *event)
//{
//    QWidget::showEvent(event);
//    if (m_isInit == false) {
//        init();
//    }
//}

//bool QWidgetBgfx::event(QEvent *event)
//{
//    switch (event->type()) {
//    case QEvent::UpdateRequest:
//        m_updatePending = false;
//        doRender();
//        return true;

//    default:
//        return QWidget::event(event);
//    }

//}

void QWidgetBgfx::resizeEvent(QResizeEvent* event)
{
    qDebug() << "QWidgetBgfx::resizeEvent(" << event << ")";
    QWidget::resizeEvent(event);

    //   if (m_isInit == false) {
    //       init();
    //       return;
    //   }

    const auto& size = event->size();
    Q_ASSERT(size.width() == width());

    if ((size.width() < 0) || (size.height() < 0)) {
        return;
    }

    m_window->resizeEvent(size.width(), size.height());
    //    render();
}

QPaintEngine* QWidgetBgfx::paintEngine() const
{
    //    qDebug() << "QWidgetBgfx::paintEngine()";
    return nullptr;
}

void QWidgetBgfx::keyPressEvent(QKeyEvent* event)
{
    m_window->keyPressEvent(s_translateKey[event->key()]);
//    default:
//        QWidget::keyPressEvent(event);
//        break;
//    }
}

void QWidgetBgfx::keyReleaseEvent(QKeyEvent* event)
{
    m_window->keyReleaseEvent(s_translateKey[event->key()]);
//    default:
//        QWidget::keyReleaseEvent(event);
//        break;
//    }
}

void QWidgetBgfx::mousePressEvent(QMouseEvent* event)
{
    m_window->mousePressEvent(s_translateMouseKey[event->button()]);
    //    qDebug() << this << "QWidgetBgfx::mousePressEvent(" << event << ")";
}

void QWidgetBgfx::mouseReleaseEvent(QMouseEvent* event)
{
    m_window->mouseReleaseEvent(s_translateMouseKey[event->button()]);
    //    qDebug() << "QWidgetBgfx::mouseReleaseEvent(" << event << ")";
}

void QWidgetBgfx::mouseMoveEvent(QMouseEvent* event)
{
    m_window->mouseMoveEvent(event->x(), event->y());
    //        qDebug() << "QWidgetBgfx::mouseMoveEvent(" << event << ")";
}


#endif // GUI_QT
