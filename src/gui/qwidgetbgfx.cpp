#ifdef GUI_QT

#include "qwidgetbgfx.h"

#include <QApplication>
#include <QDebug>
#include <QResizeEvent>
#include <QSurface>

static const std::map<int, MouseButton::Enum> s_translateMouseKey = {
    { Qt::LeftButton, MouseButton::Left },
    { Qt::RightButton, MouseButton::Right },
    { Qt::MiddleButton, MouseButton::Middle },
};
static const std::map<int, Key::Enum> s_translateKey = {
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
    { Qt::Key_N, Key::N },
    { Qt::Key_M, Key::M },
    { Qt::Key_R, Key::R },
    { Qt::Key_E, Key::E },
    { Qt::Key_Control, Key::Control },
};

QWidgetBgfx::QWidgetBgfx(QWidget* parent)
    : QWidget(parent)
//    , m_iWindow(WindowState::s_windows.size())
    , WindowState((void*)(uintptr_t)winId(), width(), height())
{
//    void* nwh = (void*)(uintptr_t)winId();
//    WindowState::s_windows.push_back(WindowState(nwh, width(), height()));

//    m_window = &WindowState::s_windows.back();

    qDebug() << "QWidgetBgfx(" << parent << ")";
    setAttribute(Qt::WA_NativeWindow);
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    setFocus();
}

//QWidgetBgfx::~QWidgetBgfx()
//{
//}

//void QWidgetBgfx::render() // source: https://zeduckmaster.frama.io/2016/how-to-create-a-custom-rendering-in-a-qt5-widget/
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

void QWidgetBgfx::paintEvent(QPaintEvent*)
{
//    m_window->render();
    WindowState::render();
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

QPaintEngine* QWidgetBgfx::paintEngine() const
{
    //    qDebug() << "QWidgetBgfx::paintEngine()";
    return nullptr;
}

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

//    m_window->resizeEvent(size.width(), size.height());
    WindowState::resizeEvent(size.width(), size.height());
    //    render();
}


void QWidgetBgfx::keyPressEvent(QKeyEvent* event)
{
//    m_window->keyPressEvent(s_translateKey.at(event->key()));
    WindowState::keyPressEvent(s_translateKey.at(event->key()));
    //    default:
    //        QWidget::keyPressEvent(event);
    //        break;
    //    }
}

void QWidgetBgfx::keyReleaseEvent(QKeyEvent* event)
{
//    m_window->keyReleaseEvent(s_translateKey.at(event->key()));
    WindowState::keyReleaseEvent(s_translateKey.at(event->key()));
    //    default:
    //        QWidget::keyReleaseEvent(event);
    //        break;
    //    }
}

void QWidgetBgfx::mousePressEvent(QMouseEvent* event)
{
//    m_window->mousePressEvent(s_translateMouseKey.at(event->button()));
    WindowState::mousePressEvent(s_translateMouseKey.at(event->button()));
    //    qDebug() << this << "QWidgetBgfx::mousePressEvent(" << event << ")";
}

void QWidgetBgfx::mouseReleaseEvent(QMouseEvent* event)
{
//    m_window->mouseReleaseEvent(s_translateMouseKey.at(event->button()));
    WindowState::mouseReleaseEvent(s_translateMouseKey.at(event->button()));
    //    qDebug() << "QWidgetBgfx::mouseReleaseEvent(" << event << ")";
}

void QWidgetBgfx::mouseMoveEvent(QMouseEvent* event)
{
//    m_window->mouseMoveEvent(event->x(), event->y());
    WindowState::mouseMoveEvent(event->x(), event->y());
    //        qDebug() << "QWidgetBgfx::mouseMoveEvent(" << event << ")";
}

#endif // GUI_QT
