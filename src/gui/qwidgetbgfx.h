#ifdef GUI_QT

#ifndef QWIDGETBGFX_H
#define QWIDGETBGFX_H

#include <QObject>
#include <QWidget>
#include <entry/windowstate.h>

class QWidgetBgfx : public QWidget, WindowState {
    Q_OBJECT
public:
    explicit QWidgetBgfx(QWidget* parent = nullptr);
//    virtual ~QWidgetBgfx();

//    void render();
private:
//    void doRender();
//   void init();

protected:
    void paintEvent(QPaintEvent* event) override;
//    void showEvent(QShowEvent *event) override;
//    bool event(QEvent *event) override;
    QPaintEngine* paintEngine() const override;

    void resizeEvent(QResizeEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
signals:

private:
//    void doRender();
//    void init();
private:
//    bool m_updatePending = false;
//   bool m_isInit = false;
//    bool m_continuousRender = true;

//    const int m_iWindow;
//    WindowState * m_window = nullptr;
//    std::unique_ptr<WindowState> m_window = nullptr;
};

#endif // QWIDGETBGFX_H

#endif // GUI_QT
