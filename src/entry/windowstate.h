#ifndef WINDOWSTATE_H
#define WINDOWSTATE_H

//#include "program.h"
#include <bgfx/bgfx.h>
#include <chrono>
#include <engine/program.h>
//#include <entry/entry.h>
//#include <shaderc/shaderc.h>

#define VIEW_ID_START_WINDOW 30
#define VIEW_ID_START_SHADOW 10
#define VIEW_ID_START_DEBUG_SHADOW 40

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
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        N,
        M,
        R,
        E,
        //        Q,
        Control,
        Shift,

        Count
    };
};

enum Projection {
    NONE,
    PERSPECTIVE,
    ORTHOGRAPHIC,
};

struct View {
public:
    int iCamera;
    float ratio;
    int id;
//    Shading shading = Shading::RENDERED;
    Shading shading = Shading::NORMAL;
    Projection projection = PERSPECTIVE;
};

class WindowState {
public:
public:
    WindowState(void* nwh, void* ndt, int width, int height,
        void* context = nullptr, void* backBuffer = nullptr,
        void* backBufferDS = nullptr);
    ~WindowState();

private:
    void render() const;

public:
    uintptr_t renderAllWindow(); // only first window is able to do that
    void printDebugMessage();
    void updateCameraPos();

//    void resizeOffscreenFB();
    void resetWindow();

public:
    void resizeEvent(int width, int height);
    void mouseMoveEvent(int x, int y);
    void mouseReleaseEvent(MouseButton::Enum mouseButton);
    void mousePressEvent(MouseButton::Enum mouseButton);
    void keyReleaseEvent(Key::Enum key);
    void keyPressEvent(Key::Enum key);
    void mouseScrollEvent(int offset);


    bgfx::FrameBufferHandle m_offScreenFBH = BGFX_INVALID_HANDLE; // default fbh created by bgfx init
private:
    //    static std::vector<WindowState*> s_windows;
//    bool m_init = false;

    const void* m_nwh = nullptr;
    const void* m_ndt = nullptr;
    int m_width;
    int m_height;
    const void* m_context = nullptr;
    const void* m_backBuffer = nullptr;
    const void* m_backBufferDS = nullptr;

    View m_view;

    //    bgfx::FrameBufferHandle m_offscreenFB = BGFX_INVALID_HANDLE;
    //    bgfx::TextureHandle m_backBuffer = BGFX_INVALID_HANDLE;
    //    bgfx::TextureHandle m_depthBuffer = BGFX_INVALID_HANDLE;

//    static size_t s_epoch;
//    static double s_deltaTime;
    //    static double s_currentTime;
//    static size_t s_iFrame;
    //    static std::chrono::time_point<std::chrono::high_resolution_clock> s_currentTime;
//    static double s_sum;
//    static size_t s_counter;


    //    Shading m_shading = Shading::RENDERED;
    //    Shading m_shading = Shading::EMISSIVE;
    //    Shading m_shading = Shading::NORMAL;
//    Shading m_shading = Shading::MATERIAL;

    //    int m_iCamera;

    int m_cameraMoveFront = 0;
    int m_cameraMoveRight = 0;
    int m_cameraMoveUp = 0;
    bool m_slowMotion = false;
    bool m_mouseLeftClicked = false;
    float m_lastX;
    float m_lastY;
    bool m_firstMouse = true;

    bool m_ctrlPressed = false;
    bool m_shiftPressed = false;

};

#endif // WINDOWSTATE_H
