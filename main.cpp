

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"
#include <bx/uint32_t.h>
#include "logo.h"

#include <iostream>
#include <chrono>
#include <thread>

#define WIN_WIDTH 640
#define WIN_HEIGHT 480

void init();
void shutdown();
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void update();

GLFWwindow *window = nullptr;

int main(int argc, char const *argv[])
{
    std::cout << "hello bgfx" << std::endl;

    init();

    unsigned int counter = 0;
    // while (true)
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        update();
        // bgfx::frame();

        counter++;
        std::this_thread::sleep_for(std::chrono::milliseconds(16));

        // glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shutdown();
    // bgfx::shutdown(); // question : after or before glfwTerminate ?
    // glfwTerminate();

    return 0;
}

void update()
{
    bgfx::setViewRect(0, 0, 0, uint16_t(WIN_WIDTH), uint16_t(WIN_HEIGHT));
    bgfx::touch(0);

    bgfx::dbgTextClear();
    bgfx::dbgTextImage(
        bx::max<uint16_t>(uint16_t(WIN_WIDTH / 2 / 8), 20) - 20, bx::max<uint16_t>(uint16_t(WIN_HEIGHT / 2 / 16), 6) - 6, 40, 12, s_logo, 160);

    bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");

    bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
    bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");

    const bgfx::Stats *stats = bgfx::getStats();
    bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.", stats->width, stats->height, stats->textWidth, stats->textHeight);

    bgfx::frame();
}

void init()
{

    glfwInit();
    window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Hello, bgfx from glfw!", NULL, NULL);
    if (window == NULL)
    {
        // std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
        // exit(-1);
    }
    glfwMakeContextCurrent(window); // question : what does this fonction ?
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    bgfx::PlatformData pd;
    pd.nwh = glfwGetWin32Window(window);
    bgfx::setPlatformData(pd);

    bgfx::Init bgfxInit;
    // bgfxInit.type = bgfx::RendererType::OpenGL;
    bgfxInit.type = bgfx::RendererType::Count;
    bgfxInit.resolution.width = WIN_WIDTH;
    bgfxInit.resolution.height = WIN_HEIGHT;
    bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
    bgfx::init(bgfxInit);

    bgfx::setDebug(BGFX_DEBUG_TEXT);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFF0000FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, WIN_WIDTH, WIN_HEIGHT);
    bgfx::touch(0);

    glfwMakeContextCurrent(nullptr); // question : why we can do it ?
}

void shutdown()
{
    glfwMakeContextCurrent(window);
    bgfx::shutdown();

    glfwMakeContextCurrent(nullptr);
    glfwDestroyWindow(window);
    // window = nullptr; // question : necessary ?
    glfwTerminate();
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // glViewport(0, 0, width, height);
}