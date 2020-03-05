

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

int main(int argc, char const *argv[])
{
    std::cout << "hello" << std::endl;
    glfwInit();
    GLFWwindow *window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Hello, bgfx from glfw!", NULL, NULL);

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

    unsigned int counter = 0;
    while (true)
    {
        bgfx::frame();
        counter++;
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    

    return 0;
}
