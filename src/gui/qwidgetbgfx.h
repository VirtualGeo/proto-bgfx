#ifdef GUI_QT

#ifndef QWIDGETBGFX_H
#define QWIDGETBGFX_H

#include <QObject>
#include <QWidget>
#include <bgfx/bgfx.h>
//#include "window.h"
#include <engine/camerafps.h>
#include <engine/scene.h>
#include <engine/windowstate.h>

class QWidgetBgfx : public QWidget {
    Q_OBJECT
public:
    explicit QWidgetBgfx(QWidget* parent = nullptr);
    virtual ~QWidgetBgfx();

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    QPaintEngine* paintEngine() const override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

signals:

private:
    void updateCameraPos();
    void printDebugMessage();
    void resetWindow();
    uint32_t getResetFlags();

private:
    //    static std::vector<WindowState> s_windows;
    //    static Scene s_scene;
    //    static Program s_program;

    int m_iWindow;
    CameraFps m_cameraFps; //({-7.0f, 1.0f, 0.0f});
    int m_cameraMoveFront = 0;
    int m_cameraMoveRight = 0;
    int m_cameraMoveUp = 0;
    bool g_slowMotion = false;
    bool g_mouseLeftClicked = false;
    float g_lastX;
    float g_lastY;
    bool g_firstMouse = true;
//    std::string g_viewportShading = "rendered";
//    int g_iViewportShading = Program::Shading::RENDERED;
    int g_mssaLevel = 0;
    int g_textureSamplerFlags = 0;
    bool g_vsyncEnable = false;

    //    int m_iWidget;
    //    bgfx::FrameBufferHandle m_fbh;

    double g_deltaTime;
    //double g_fps = 0.0;
    double g_sum = 0.0;
    size_t g_counter = 0;
    size_t g_epoch = 10; // compute first fps mean with 10 frames
    std::chrono::time_point<std::chrono::high_resolution_clock> g_lastTime;
};

#endif // QWIDGETBGFX_H

#endif
