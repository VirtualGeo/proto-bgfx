#ifndef WINDOWSTATE_H
#define WINDOWSTATE_H

//#include "program.h"
#include <bgfx/bgfx.h>
#include <chrono>
#include <engine/program.h>
//#include <entry/entry.h>

struct MouseButton {
    enum Enum {
        None = 0,
        Left,
        Middle,
        Right,
        Count
    };
};

struct Key {
    enum Enum {
        None = 0,
        Up,
        Down,
        Right,
        Left,
        PageUp,
        PageDown,
        F1,
        F2,
        F3,
        F4,
        F5,
        N,
        M,
        R,
        Control,

        Count
    };
};

class WindowState {
public:

public:
    WindowState(void* nwh, int width, int height);
    ~WindowState();

    void clear();
    void updateCameraPos();
    void resetWindow();
    void render();
    void printDebugMessage();

public:
    void mouseMoveEvent(int x, int y);
    void mouseReleaseEvent(MouseButton::Enum mouseButton);
    void mousePressEvent(MouseButton::Enum mouseButton);
    void keyReleaseEvent(Key::Enum key);
    void keyPressEvent(Key::Enum key);
    void resizeEvent(int width, int height);

private:
//    static std::vector<WindowState*> s_windows;

    void* m_nwh = nullptr;
    int m_width;
    int m_height;
    const int m_id;

    bgfx::FrameBufferHandle m_fbh;
    float m_fps;
    size_t m_epoch = 10;
    double g_deltaTime;
    double g_sum = 0.0;
    size_t g_counter = 0;
    std::chrono::time_point<std::chrono::high_resolution_clock> g_lastTime;

    Shading m_shading = Shading::RENDERED;
    int m_iCamera;

    int m_cameraMoveFront = 0;
    int m_cameraMoveRight = 0;
    int m_cameraMoveUp = 0;
    bool g_slowMotion = false;
    bool g_mouseLeftClicked = false;
    float g_lastX;
    float g_lastY;
    bool g_firstMouse = true;
};

#endif // WINDOWSTATE_H
