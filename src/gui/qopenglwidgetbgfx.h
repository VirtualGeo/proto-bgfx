#ifdef GUI_QT

#ifndef QOPENGLWIDGETBGFX_H
#define QOPENGLWIDGETBGFX_H

#include <QObject>
#include <QOpenGLWidget>
#include <entry/windowstate.h>
#include <QWidget>

class QOpenGLWidgetBgfx : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit QOpenGLWidgetBgfx(QWidget * parent = nullptr);

protected: // QOpenglWidget
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

protected: // Widget
//    void paintEvent(QPaintEvent *e) override;
//    void resizeEvent(QResizeEvent *e) override;


private:
    WindowState * m_windowState = nullptr;
    bgfx::TextureHandle m_backBuffer = BGFX_INVALID_HANDLE;
    bgfx::TextureHandle m_depthBuffer = BGFX_INVALID_HANDLE;

};

#endif // QOPENGLWIDGETBGFX_H

#endif // GUI_QT
