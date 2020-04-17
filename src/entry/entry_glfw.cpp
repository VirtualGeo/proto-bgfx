#ifdef GUI_GLFW

#include <bgfx/platform.h>

#include <GLFW/glfw3.h>
#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX // question : quésako ?
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
//#include "GLFW/glfw4native.h"
#include <GLFW/glfw3native.h>

#include <fstream>
#include <iostream>
#include "windowstate.h"


void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action,
    int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void error_callback(int error, const char* description);
// void focus_callback(GLFWwindow *window, int focused);

void init();
void shutdown();
void update();

// ----------------- GLOBAL VARS
const int g_width = 800;
const int g_height = 600;
GLFWwindow* g_window = nullptr;

WindowState* g_windowState = nullptr;

//static const std::map<int, MouseButton::Enum> s_translateMouseKey = {
static std::map<int, MouseButton::Enum> s_translateMouseKey = {
    { GLFW_MOUSE_BUTTON_LEFT, MouseButton::Left },
    { GLFW_MOUSE_BUTTON_RIGHT, MouseButton::Right },
    { GLFW_MOUSE_BUTTON_MIDDLE, MouseButton::Middle },
};
//static const std::map<int, Key::Enum> s_translateKey = {
static std::map<int, Key::Enum> s_translateKey = {
    { GLFW_KEY_UP, Key::Up },
    { GLFW_KEY_DOWN, Key::Down },
    { GLFW_KEY_RIGHT, Key::Right },
    { GLFW_KEY_LEFT, Key::Left },
    { GLFW_KEY_PAGE_UP, Key::PageUp },
    { GLFW_KEY_PAGE_DOWN, Key::PageDown },
    { GLFW_KEY_F1, Key::F1 },
    { GLFW_KEY_F2, Key::F2 },
    { GLFW_KEY_F3, Key::F3 },
    { GLFW_KEY_F4, Key::F4 },
    { GLFW_KEY_F5, Key::F5 },
    { GLFW_KEY_L, Key::N }, // qwerty to dvorak
    { GLFW_KEY_M, Key::M }, // qwerty to dvorak
    { GLFW_KEY_O, Key::R }, // qwerty to dvorak
    { GLFW_KEY_RIGHT_CONTROL, Key::Control },
};

int main(int argc, char const* argv[])
{
    glfwSetErrorCallback(error_callback);

#ifdef DEBUG
    std::cout << "[main] hello bgfx !!!" << std::endl;
#endif

    init();
#ifdef DEBUG
    std::cout << "[main] init done." << std::endl;
#endif
    while (!glfwWindowShouldClose(g_window)) {

        processInput(g_window);

        update();

        // glfwSwapBuffers(g_window); question : why not use ?
        glfwPollEvents();
        // glfwSetMousePos(100, 100);
    }

    shutdown();
    // bgfx::shutdown(); // question : after or before glfwTerminate ?
    // glfwTerminate();

    return 0;
}

void init()
{
    // --------------------- INIT GLFW
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    g_window = glfwCreateWindow(g_width, g_height, "Hello, bgfx from glfw!",
        NULL, NULL);
    if (g_window == NULL) {
        // std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::cout << "[main] Failed to create GLFW window" << std::endl;
        throw std::runtime_error("Failed to create GLFW window");
        // exit(-1);
    }
    // glfwMakeContextCurrent(window); // question : what does this fonction ?
    glfwSetFramebufferSizeCallback(g_window, framebuffer_size_callback);
    glfwSetCursorPosCallback(g_window, mouse_callback);
    glfwSetScrollCallback(g_window, scroll_callback);
    glfwSetMouseButtonCallback(g_window, mouse_button_callback);
    glfwSetKeyCallback(g_window, key_callback);
    // glfwSetWindowFocusCallback(g_window, focus_callback);

    // glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // bgfx::RenderFrame();

    void* ndt = nullptr;
    void* nwh = nullptr;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    ndt = glfwGetX11Display();
    nwh = (void*)(uintptr_t)glfwGetX11Window(g_window);
#elif BX_PLATFORM_OSX
    nwh = glfwGetCocoaWindow(g_window);
#elif BX_PLATFORM_WINDOWS
    nwh = glfwGetWin32Window(g_window);
#endif
    int width;
    int height;
    glfwGetWindowSize(g_window, &width, &height);
    g_windowState = new WindowState(nwh, width, height);
}

void update()
{
    g_windowState->render();
}

void shutdown()
{
//    g_windowState->clear();
    delete g_windowState;

    //    bgfx::shutdown();

    // glfwMakeContextCurrent(nullptr);
    // glfwMakeContextCurrent(g_window);
    glfwDestroyWindow(g_window);
    // window = nullptr; // question : necessary ?
    glfwTerminate();
#ifdef DEBUG
    std::cout << "[main] shutdown done" << std::endl;
#endif
}

// ------------------------------------ GLFW FUNCTIONS
void error_callback(int error, const char* description)
{
    std::cout << "GLFW Error " << error << ": " << description << std::endl;
    exit(1);
}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    g_windowState->resizeEvent(width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (action) {
    case GLFW_PRESS:

//        g_windowState->keyPressEvent(s_translateKey.at(key));
        g_windowState->keyPressEvent(s_translateKey[key]);
        break;

    case GLFW_RELEASE:
//        g_windowState->keyReleaseEvent(s_translateKey.at(key));
        g_windowState->keyReleaseEvent(s_translateKey[key]);
        break;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    g_windowState->mouseMoveEvent(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    g_windowState->mouseScrollEvent(yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action,
    int mods)
{
    //    if (action == GLFW_PRESS) {
    switch (action) {
    case GLFW_PRESS:
//        g_windowState->mousePressEvent(s_translateMouseKey.at(button));
        g_windowState->mousePressEvent(s_translateMouseKey[button]);
        break;
    case GLFW_RELEASE:
//        g_windowState->mouseReleaseEvent(s_translateMouseKey.at(button));
        g_windowState->mouseReleaseEvent(s_translateMouseKey[button]);
        break;
    }
}

#endif // GUI_GLFW
