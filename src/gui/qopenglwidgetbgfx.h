#ifdef GUI_QT

#ifndef QOPENGLWIDGETBGFX_H
#define QOPENGLWIDGETBGFX_H

#include <QObject>
#include <QOpenGLWidget>
#include <entry/windowstate.h>

class QOpenGLWidgetBgfx : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit QOpenGLWidgetBgfx(QWidget * parent = nullptr);

protected: // QOpenglWidget
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

protected: // Widget
//    void paintEvent(QPaintEvent *e) override;
//    void resizeEvent(QResizeEvent *e) override;


private:
    WindowState * m_windowState = nullptr;
};

#endif // QOPENGLWIDGETBGFX_H

#endif // GUI_QT
