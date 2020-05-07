#ifndef WINDOWSTATE_H
#define WINDOWSTATE_H

//#include "program.h"
#include <bgfx/bgfx.h>
#include <chrono>
#include <engine/program.h>
//#include <entry/entry.h>

struct MouseButton {
    enum Enum {
        NONE = 0, // glfw None conflict
        Left,
        Middle,
        Right,
        Count
    };
};

struct Key {
    enum Enum {
        NONE = 0,
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
        E,
//        Q,
        Control,

        Count
    };
};


class WindowState {
public:

public:
    WindowState(void* nwh, int width, int height);
    ~WindowState();

    void updateCameraPos();
    void resetWindow();
private:
    void render() const;
public:
    void renderAllWindow(); // only first window is able to do that
    void printDebugMessage();

public:
    void mouseMoveEvent(int x, int y);
    void mouseReleaseEvent(MouseButton::Enum mouseButton);
    void mousePressEvent(MouseButton::Enum mouseButton);
    void keyReleaseEvent(Key::Enum key);
    void keyPressEvent(Key::Enum key);
    void resizeEvent(int width, int height);
    void mouseScrollEvent(int offset);

private:
//    static std::vector<WindowState*> s_windows;

    void* m_nwh = nullptr;
    int m_width;
    int m_height;
    const int m_id;

    bgfx::FrameBufferHandle m_fbh;

    static float s_fps;
    static size_t s_epoch;
    static double s_deltaTime;
    static std::chrono::time_point<std::chrono::high_resolution_clock> s_lastTime;
    static double s_sum;
    static size_t s_counter;

    Shading m_shading = Shading::RENDERED;
//    Shading m_shading = Shading::EMISSIVE;
//    Shading m_shading = Shading::NORMAL;

    int m_iCamera;

    int m_cameraMoveFront = 0;
    int m_cameraMoveRight = 0;
    int m_cameraMoveUp = 0;
    bool m_slowMotion = false;
    bool m_mouseLeftClicked = false;
    float m_lastX;
    float m_lastY;
    bool m_firstMouse = true;

    bool m_ctrlPressed = false;

};

#endif // WINDOWSTATE_H
