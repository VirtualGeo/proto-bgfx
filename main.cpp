

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#include <iostream>
#include <chrono>
#include <thread>

#define WIN_WIDTH 1600
#define WIN_HEIGHT 900

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

    bgfx::frame();
}

void init()
{

    glfwInit();
    window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Hello, bgfx from glfw!", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    // glfwMakeContextCurrent(window);
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    bgfx::PlatformData pd;
    pd.nwh = glfwGetWin32Window(window);
    bgfx::setPlatformData(pd);

    bgfx::Init bgfxInit;
    bgfxInit.type = bgfx::RendererType::Count;
    bgfxInit.resolution.width = WIN_WIDTH;
    bgfxInit.resolution.height = WIN_HEIGHT;
    bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
    bgfx::init(bgfxInit);

    bgfx::setDebug(BGFX_DEBUG_TEXT);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFF0000FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, WIN_WIDTH, WIN_HEIGHT);
    bgfx::touch(0);
}

void shutdown()
{
    glfwTerminate();
    window = nullptr;
    bgfx::shutdown();
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