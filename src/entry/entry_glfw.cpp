#ifdef GUI_GLFW

#include "bgfx/platform.h"
#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/file.h>
#include <bx/macros.h>
#include <bx/uint32_t.h>

#include <GLFW/glfw3.h>
#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX // question : quésako ?
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
//#include "GLFW/glfw4native.h"
#include <GLFW/glfw3native.h>

//#include <thread>
// #include <filesystem>
// #include <glm/glm.hpp>
// #include <assert.h>
//#include "Mesh.h"
//#include "scene.h"
#include <engine/scene.h>
#include <cassert>
#include <fstream>
#include <iostream>
//#include "Vertex.h"
//#include "program.h"
#include <engine/program.h>

//#define WIN_WIDTH 800
//#define WIN_HEIGHT 600
//#define WIN_WIDTH 1920
//#define WIN_HEIGHT 1080

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
uint32_t getResetFlags();
void resetWindow();

//bgfx::ShaderHandle loadShader(const char* FILENAME);
void updateCameraFront();

// ----------------- GLOBAL VARS
int g_width = 800;
int g_height = 600;
GLFWwindow* g_window = nullptr;
// bgfx::ProgramHandle g_program;
Program g_program;
size_t g_counter = 0; // avoid g_fps = epoch / sum; with sum = 0
int g_epoch = 10;
// Mesh* g_mesh = nullptr;
Scene g_scene;
bool g_clicked = false;

float g_deltaTime = 0.0f;
//float g_lastFrame = 0.0f;

bool g_firstMouse = true;
// ------------------------ CAM INIT ---------------------------------
// SPONZA:
float g_yaw = 0.0f;
float g_pitch = 0.0f;
bx::Vec3 g_cameraPos = { -7.0f, 1.0f, 0.0f };
// COUGAR:
// float g_yaw = 210.0f;
// float g_pitch = -27.0f;
// bx::Vec3 g_cameraPos = { 10.0f, 5.0f, 7.0f };
// -------------------------------------------------------------------

float g_lastX = g_width / 2.0f;
float g_lastY = g_height / 2.0f;
float g_fov = 60.0f;
//bx::Vec3 g_cameraPos = { 20.0f, 20.0f, 0.0f };
bx::Vec3 g_cameraFront;
bx::Vec3 g_cameraUp = { 0.0f, 1.0f, 0.0f };
const float g_maxFov = 120.0f;
float g_fps = 0.0f;
size_t g_nbVertices;
size_t g_nbTriangles;
size_t g_nbObjects;
float g_texturesSize;
size_t g_nbTextures;
int g_parsingTime;
int g_loadingMaterialsTime;
int g_loadingObjectsTime;
int g_totalLoadingTime;
size_t g_nbVertexBuffer;
size_t g_nbIndexBuffer;
std::string g_renderer;
std::string g_vendorID;
//int g_debugHMargin;
bool g_showStats = false;
bool g_vsyncEnable = false;
int g_mssaLevel = 0;
//bool g_shiftPressed = false;
bool g_slowMotion = false;
int g_textureSamplerFlags = 0;
std::string g_viewportShading = "rendered";
int g_iViewportShading = Program::Shading::RENDERED;
//int g_iViewportShading = Program::Shading::NORMAL;
float g_mtx[16];

//#ifdef UNDEF_H
int main(int argc, char const* argv[])
{
    glfwSetErrorCallback(error_callback);

    updateCameraFront(); // with initial g_yaw and g_pitch
#ifdef DEBUG
    std::cout << "[main] hello bgfx !!!" << std::endl;
#endif

    //    std::cout << PROJECT_DIR << std::endl;
    //    return 0;

    init();
#ifdef DEBUG
    std::cout << "[main] init done." << std::endl;
#endif
    bgfx::setDebug(BGFX_DEBUG_TEXT);

    //    float sum = 0.0f;
    // while (true)
    //    float fps = 0.0f;
    //    g_fps = 0.0f;
    float sum = 0.0f;
    auto lastTime = glfwGetTime();
    // int epoch = 10;
    while (!glfwWindowShouldClose(g_window)) {

        const auto currentTime = glfwGetTime();
        g_deltaTime = currentTime - lastTime; // need deltaTime at each frame by processInput()
        lastTime = currentTime;
        sum += g_deltaTime;

        if (g_counter >= g_epoch) {
            //		    g_lastFrame = currentFrame;

            g_epoch = (g_fps = g_epoch / sum) / 2; // fps echo epoch = 0.5 sec
            // g_epoch = (g_fps = g_epoch / sum) * 5; // fps echo epoch = 5 sec (better average)

            // fps = 1.0f / g_deltaTime;
            sum = 0.0f;
            g_counter = 0;
            // g_epoch = g_fps;
            //			bgfx::dbgTextClear();
            //            printDebugMessage();
        }
        //        bgfx::dbgTextPrintf(0, 0, 0x08, "fps = %.1f", g_fps);

        processInput(g_window);

        update();

        // bgfx::frame();

        ++g_counter;
        // std::this_thread::sleep_for(std::chrono::milliseconds(16));

        // glfwSwapBuffers(g_window); question : why not use ?
        glfwPollEvents();
        // std::cout << "update " << g_counter << std::endl;
        // glfwSetMousePos(100, 100);
    }

    shutdown();
    // bgfx::shutdown(); // question : after or before glfwTerminate ?
    // glfwTerminate();

    return 0;
}
//#endif

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

    // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
    // Most graphics APIs must be used on the same thread that created the window.
    bgfx::renderFrame();

    // bgfx::PlatformData pd;
    // pd.nwh = glfwGetWin32Window(window);
    // bgfx::setPlatformData(pd);

    bgfx::Init bgfxInit = {};
//	bgfxInit.platformData.nwh = glfwGetWin32Window(g_window);
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    bgfxInit.platformData.ndt = glfwGetX11Display();
    bgfxInit.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(g_window);
#elif BX_PLATFORM_OSX
    bgfxInit.platformData.nwh = glfwGetCocoaWindow(g_window);
#elif BX_PLATFORM_WINDOWS
    bgfxInit.platformData.nwh = glfwGetWin32Window(g_window);
#endif
    bgfxInit.platformData.context = NULL;
    bgfxInit.platformData.backBuffer = NULL;
    bgfxInit.platformData.backBufferDS = NULL;

    // std::cout << RENDERER_TYPE << std::endl;
    bgfxInit.type = bgfx::RendererType::Count; // Automatically choose renderer
//    bgfxInit.type = bgfx::RendererType::Direct3D9;
//    bgfxInit.type = bgfx::RendererType::Direct3D11;
//    bgfxInit.type = bgfx::RendererType::Direct3D12;
//    bgfxInit.type = bgfx::RendererType::OpenGL;
//    bgfxInit.type = bgfx::RendererType::OpenGLES;
//    bgfxInit.type = bgfx::RendererType::Vulkan;
//    bgfxInit.type = bgfx::RendererType::Metal;
//    std::cout << "[main] RendererType auto" << std::endl;

    glfwGetWindowSize(g_window, &g_width, &g_height);
    bgfxInit.resolution.width = g_width;
    bgfxInit.resolution.height = g_height;
    //    bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
    //    bgfxInit.resolution.reset = BGFX_RESET_NONE;
    //    bgfxInit.resolution.reset = g_vsyncEnable ? BGFX_RESET_VSYNC : BGFX_RESET_NONE;
    bgfxInit.resolution.reset = getResetFlags();
    bgfxInit.vendorId = BGFX_PCI_ID_NONE;
    // bgfxInit.vendorId = BGFX_PCI_ID_INTEL;
    //    bgfxInit.vendorId = BGFX_PCI_ID_NVIDIA;
    // bgfx::init(bgfxInit);
    if (!bgfx::init(bgfxInit)) {
        std::cout << "[main] Failed to initialize bgfx" << std::endl;
        // throw std::runtime_error("Failed to initialize bgfx");
        exit(1);
        // return 1;
    }

    // bgfx::setDebug(BGFX_DEBUG_TEXT);
    // bgfx::setDebug(BGFX_DEBUG_WIREFRAME);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFF0000FF, 1.0f, 0);
    // question : why only here once, not at each frame ?

    //    bgfx::setViewRect(0, 0, 0, g_width, g_height);
    bgfx::touch(0);

    // glfwMakeContextCurrent(nullptr); // question : why we can do it ?

    // ------------------------------- LOADING MODEL
//   g_scene.addModel(std::string(PROJECT_DIR) + "assets/sponza/sponza.obj");
         g_scene.addModel("/home/gauthier/Downloads/Cougar2/cougar.obj");
    //    g_scene.addModel("C:\\Users\\gauthier.bouyjou\\Downloads\\export\\cougar.obj");

    //        g_scene.addModel("/home/gauthier/Downloads/Cougar/Cougar.obj");
    //    g_scene.addModel(std::string(PROJECT_DIR) + "assets/McGuire/Dabrovic_Sponza/sponza-blend.obj");
    //    g_scene.addModel(std::string(PROJECT_DIR) + "assets/McGuire/Crytek_Sponza/sponza-blend.obj");
    //    g_scene.addModel(std::string(PROJECT_DIR) + "assets/McGuire/San_Miguel/san-miguel-blend.obj");


    g_nbVertices = g_scene.nbVertices();
    g_nbTriangles = g_scene.nbTriangles();
    g_nbObjects = g_scene.nbObjects();
    g_texturesSize = g_scene.texturesSize() / 1000000.0f;
    g_nbTextures = g_scene.nbTextures();
    g_parsingTime = g_scene.parsingTime();
    g_loadingMaterialsTime = g_scene.loadingMaterialsTime();
    g_loadingObjectsTime = g_scene.loadingObjectsTime();
    g_totalLoadingTime = g_parsingTime + g_loadingMaterialsTime + g_loadingObjectsTime;
    g_nbVertexBuffer = g_scene.nbVertexBuffer();
    g_nbIndexBuffer = g_scene.nbIndexBuffer();
    //    printDebugMessage();

    const bgfx::Caps* caps = bgfx::getCaps();
    //    assert(caps->supported & BGFX_CAPS_FORMAT_TEXTURE_MIP_AUTOGEN);
    //    switch (bgfx::getRendererType()) {
    g_renderer = bgfx::getRendererName(caps->rendererType);
    //    switch (caps->rendererType) {
    //    case bgfx::RendererType::Noop:
    //    case bgfx::RendererType::Direct3D9:
    //        g_renderer = "DirectX9";
    //        break;
    //    case bgfx::RendererType::Direct3D11:
    //        g_renderer = "DirectX11";
    //        break;
    //    case bgfx::RendererType::Direct3D12:
    //        g_renderer = "DirectX12";
    //        break;
    //    // case bgfx::RendererType::Gnm:
    //    // shaderPath = "shaders/bin/pssl/";
    //    // break;
    //    case bgfx::RendererType::Metal:
    //        g_renderer = "Metal";
    //        break;
    //    case bgfx::RendererType::OpenGLES:
    //        g_renderer = "OpenGLES";
    //        break;
    //    case bgfx::RendererType::OpenGL:
    //        g_renderer = "OpenGL";
    //        break;
    //    case bgfx::RendererType::Vulkan:
    //        g_renderer = "Vulkan";
    //        break;
    //    }
    //    g_debugHMargin = (g_renderer == "OpenGL") ? (0) : (2);

    // bgfx::getCaps().vendorId;

    switch (caps->vendorId) {
    case BGFX_PCI_ID_AMD:
        g_vendorID = "AMD";
        break;
    case BGFX_PCI_ID_INTEL:
        g_vendorID = "INTEL";
        break;
    case BGFX_PCI_ID_NVIDIA:
        g_vendorID = "NVIDIA";
        break;

    default:
        std::cout << "unknown vendor graphic id" << std::endl;
        exit(1);
        break;
    }

    // ----------------- INIT SHADERS AND PROGRAM
    //    bgfx::ShaderHandle vsh = loadShader("cubes.vert");
    //    bgfx::ShaderHandle fsh = loadShader("cubes.frag");
    //    g_program = bgfx::createProgram(vsh, fsh, true);
    //    g_program.init("cubes");
    //    g_program.init("rendered");
    g_program.init();
    g_program.setShading(Program::Shading(g_iViewportShading));
    g_viewportShading = Program::shadingFileNames[g_iViewportShading];
    //    g_program.init("material");
    bx::mtxIdentity(g_mtx);

    // g_texColor = bgfx::createUniform("s_texColor",
    // bgfx::UniformType::Sampler);

    // bgfx::ProgramHandle program;
    // program = loadProgram("cubes.vert", "cubes.frag");
}

void printDebugMessage()
{
    //        int margin = 2;
    //   const int margin = (g_renderer == "OpenGL") ? (0) : (2);
    //    tvm.printf(0, pos++, 0x8f, "       Vendor: %s ", m_vendor);
    //    tvm.printf(0, pos++, 0x8f, "     Renderer: %s ", m_renderer);
    //    tvm.printf(0, pos++, 0x8f, "      Version: %s ", m_version);
    //    tvm.printf(0, pos++, 0x8f, " GLSL version: %s ", m_glslVersion);

    //    int line = g_debugHMargin - 1;
    if (g_showStats) {
        bgfx::setDebug(BGFX_DEBUG_STATS);
    } else {
        bgfx::dbgTextClear();
        bgfx::setDebug(BGFX_DEBUG_TEXT);
        const bgfx::Stats* stats = bgfx::getStats();
        int line = -1;
        bgfx::dbgTextPrintf(0, ++line, 0x2F, "F1:Stats | F2:Vsync | F3:Msaa | F4:Sampler | F5:Shading | F6: | F7:");
        //        bgfx::dbgTextPrintf(0, ++line, 0x0F, " %s / " BX_COMPILER_NAME " / " BX_CPU_NAME " / " BX_ARCH_NAME " / " BX_PLATFORM_NAME " ", bgfx::getRendererName(bgfx::getRendererType());
        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Fps:%.1f | Verts:%d | Tris:%d | Verts/Tris:%.2f | Objects:%d | Textures:%d (%.1f MiB)",
            g_fps, g_nbVertices, g_nbTriangles, (float)g_nbVertices / g_nbTriangles, g_nbObjects, g_nbTextures, g_texturesSize);
        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Vertex buffer:%d | Index buffer:%d | Index buffer/Vertex buffer:%.2f",
            g_nbVertexBuffer, g_nbIndexBuffer, (float)g_nbIndexBuffer / g_nbVertexBuffer);
        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Viewport shading: %s", g_viewportShading.c_str());
        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Arch: " BX_COMPILER_NAME " / " BX_CPU_NAME " / " BX_ARCH_NAME " / " BX_PLATFORM_NAME " ");
        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Renderer: %s", g_renderer.c_str());
        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Graphic Vendor: %s Corporation", g_vendorID.c_str());
        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Backbuffer %dx%d in pixels", stats->width, stats->height);
        bgfx::dbgTextPrintf(0, ++line, 0x0F, "TinyObj parsing time: %d ms", g_parsingTime);
        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Loading materials time: %d ms", g_loadingMaterialsTime);
        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Loading objects time: %d ms", g_loadingObjectsTime);
        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Total loading time: %d ms", g_totalLoadingTime);
        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Total draw call: %d", g_nbIndexBuffer + 1);
    }
}

void update()
{
    // Set view 0 default viewport.
    bgfx::setViewRect(0, 0, 0, uint16_t(g_width), uint16_t(g_height));

    // This dummy draw call is here to make sure that view 0 is cleared
    // if no other draw calls are submitted to view 0.
    bgfx::touch(0);

    printDebugMessage();
    //    bgfx::dbgTextPrintf(0, 0, 0x0F, "Fps:%.1f", g_fps);

    // ------------------------- RENDER SCENE
    //    const bx::Vec3 at = { 0.0f, 2.0f, 0.0f };
    //    const bx::Vec3 at = { 0.0f, 2.0f, 0.0f };
    //    const bx::Vec3 eye = { 0.0f, 2.0f, -2.0f };
    // {
    float view[16];
    // bx::mtxLookAt(view, eye, at);
    bx::mtxLookAt(view, g_cameraPos, bx::add(g_cameraPos, g_cameraFront), g_cameraUp);

    float proj[16];
    bx::mtxProj(proj, g_fov, float(g_width) / float(g_height), 0.1f, 100.0f,
        bgfx::getCaps()->homogeneousDepth);

    bgfx::setViewTransform(0, view, proj);

//    float mtx[16];
    //     bx::mtxRotateXY(mtx, g_counter * 0.001f, g_counter * 0.001f);
    // if (g_focused) {
    //    bx::mtxIdentity(mtx);
    // bx::mtxScale(mtx, 0.01f);
    // } else {

//    bx::mtxRotateY(mtx, 0.001f);
//    bx::mtxMul(g_mtx, g_mtx, mtx);

    // }
    // bx::mtxScale(mtx, 0.1f);
//     bgfx::setTransform(mtx);

    // const uint64_t state = 0 | BGFX_STATE_WRITE_R | BGFX_STATE_WRITE_G |
    // 					   BGFX_STATE_WRITE_B | BGFX_STATE_WRITE_A |
    // 					   BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS |
    // 					   BGFX_STATE_CULL_CCW | BGFX_STATE_MSAA | s_ptState[0];

    //        const uint64_t state = 0 | BGFX_STATE_PT_TRISTRIP |
    //        BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
    //        BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CCW |
    //        BGFX_STATE_BLEND_NORMAL;
    //    const uint64_t state = 0 | BGFX_STATE_DEFAULT;
    const uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS
        | BGFX_STATE_CULL_CCW | BGFX_STATE_BLEND_NORMAL;
    //    const uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z
    //            | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CCW | BGFX_STATE_BLEND_NORMAL
    //            | BGFX_STATE_MSAA;
    // UINT64_C(0), BGFX_STATE_PT_TRISTRIP,

    // const uint64_t stateTransparent =
    // 	0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
    // 	BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CCW | BGFX_STATE_MSAA |
    // 	BGFX_STATE_BLEND_ALPHA;

    // const uint64_t stateOpaque = BGFX_STATE_DEFAULT;

    g_scene.draw(0, g_program, g_mtx, state, g_cameraPos);

    // Advance to next frame. Process submitted rendering primitives.
    bgfx::frame();
}

void shutdown()
{

    g_scene.clear(); // clear bgfx handles (texture, vertexBuffer, indexBuffer) before shutdown bgfx
    g_program.clear(); // clear uniform handles

    bgfx::shutdown();

    // glfwMakeContextCurrent(nullptr);
    // glfwMakeContextCurrent(g_window);
    glfwDestroyWindow(g_window);
    // window = nullptr; // question : necessary ?
    glfwTerminate();
#ifdef DEBUG
    std::cout << "[main] shutdown done" << std::endl;
#endif
}

uint32_t getResetFlags()
{
    uint32_t flags = 0 | BGFX_RESET_NONE;
    if (g_vsyncEnable) {
        flags |= BGFX_RESET_VSYNC;
    }

    switch (g_mssaLevel) {
    case 1:
        flags |= BGFX_RESET_MSAA_X2;
        break;
    case 2:
        flags |= BGFX_RESET_MSAA_X4;
        break;
    case 3:
        flags |= BGFX_RESET_MSAA_X8;
        break;
    case 4:
        flags |= BGFX_RESET_MSAA_X16; // question : why msaa_16 only differ
        break;
    }
    return flags;
}

void resetWindow()
{
    bgfx::reset(g_width, g_height, getResetFlags());
    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
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
        // if (g_focused) {
        // }
        glfwSetWindowShouldClose(window, true);
    }

    //    float cameraSpeed = g_shiftPressed ? 1.0f : 5.0f;
    float cameraSpeed = g_slowMotion ? 1.0f : 5.0f;
    cameraSpeed *= g_deltaTime;
    //    cameraSpeed= 5.0 * g_deltaTime;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        g_cameraPos = bx::add(g_cameraPos, bx::mul(g_cameraFront, cameraSpeed));
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        g_cameraPos = bx::sub(g_cameraPos, bx::mul(g_cameraFront, cameraSpeed));

    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        g_cameraPos = bx::add(g_cameraPos, bx::mul(g_cameraUp, cameraSpeed));
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        g_cameraPos = bx::sub(g_cameraPos, bx::mul(g_cameraUp, cameraSpeed));

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        g_cameraPos = bx::sub(
            g_cameraPos, bx::mul(bx::normalize(bx::cross(g_cameraFront, g_cameraUp)), cameraSpeed));
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        g_cameraPos = bx::add(
            g_cameraPos, bx::mul(bx::normalize(bx::cross(g_cameraFront, g_cameraUp)), cameraSpeed));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    g_width = width;
    g_height = height;

    resetWindow();

    //    bgfx::reset(g_width, g_height, g_vsyncEnable ? BGFX_RESET_VSYNC : BGFX_RESET_NONE);
    //    std::cout << "size_callback" << std::endl;
    // glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
        g_showStats = !g_showStats;
    }
    if (key == GLFW_KEY_F2 && action == GLFW_PRESS) {
        g_vsyncEnable = !g_vsyncEnable;
        g_epoch = 10;
        resetWindow();
    }
    if (key == GLFW_KEY_F3 && action == GLFW_PRESS) {
        g_mssaLevel = ++g_mssaLevel % 5;
        resetWindow();
    }
    if (key == GLFW_KEY_F4 && action == GLFW_PRESS) {
        g_textureSamplerFlags = ++g_textureSamplerFlags % 5;
        switch (g_textureSamplerFlags) {
        case 0:
            Texture::s_textureSamplerFlags = 0 | BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE;
            break;
        case 1:
            Texture::s_textureSamplerFlags = 0 | BGFX_TEXTURE_NONE | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIN_POINT;
            break;
        case 2:
            Texture::s_textureSamplerFlags = 0 | BGFX_TEXTURE_NONE | BGFX_SAMPLER_MAG_ANISOTROPIC | BGFX_SAMPLER_MIN_POINT;
            break;
        case 3:
            Texture::s_textureSamplerFlags = 0 | BGFX_TEXTURE_NONE | BGFX_SAMPLER_MAG_SHIFT | BGFX_SAMPLER_MIN_SHIFT;
            break;
        case 4:
            Texture::s_textureSamplerFlags = 0 | BGFX_TEXTURE_NONE | BGFX_SAMPLER_MAG_ANISOTROPIC | BGFX_SAMPLER_MIN_ANISOTROPIC;
            break;
            //        case 4:
            //            Texture::s_textureSamplerFlags = 0 | BGFX_TEXTURE_NONE | BGFX_SAMPLER_MAG_MASK | BGFX_SAMPLER_MIN_MASK; // question : why failed
            //            break;
        }
    }
    if (key == GLFW_KEY_F5 && action == GLFW_PRESS) {
        g_iViewportShading = ++g_iViewportShading % Program::Shading::Count;
        g_program.setShading(Program::Shading(g_iViewportShading));
        g_viewportShading = Program::shadingFileNames[g_iViewportShading];
    }
    if (key == GLFW_KEY_L && action == GLFW_RELEASE) {
        g_iViewportShading = Program::Shading::NORMAL;
        g_program.setShading(Program::Shading(g_iViewportShading));
        g_viewportShading = Program::shadingFileNames[g_iViewportShading];
        //            g_viewportShading = "Normal";
        //            g_program.setShading(Program::Shading::NORMAL);
    }
    if (key == GLFW_KEY_M && action == GLFW_RELEASE) {
        g_iViewportShading = Program::Shading::MATERIAL;
        g_program.setShading(Program::Shading(g_iViewportShading));
        g_viewportShading = Program::shadingFileNames[g_iViewportShading];
        //            g_viewportShading = "Material";
        //            g_program.setShading(Program::Shading::MATERIAL);
    }
    if (key == GLFW_KEY_O && action == GLFW_RELEASE) {
        g_iViewportShading = Program::Shading::RENDERED;
        g_program.setShading(Program::Shading(g_iViewportShading));
        g_viewportShading = Program::shadingFileNames[g_iViewportShading];
        //            g_viewportShading = "Rendered";
        //            g_program.setShading(Program::Shading::RENDERED);
    }
    if (key == GLFW_KEY_RIGHT_CONTROL && action == GLFW_RELEASE) {
        g_slowMotion = !g_slowMotion;
    }
//    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_KEY_RIGHT_SHIFT) {
//        g_shiftPressed = action == GLFW_PRESS;
        //        if (action == GLFW_RELEASE) {
        //            g_slowMotion = !g_slowMotion;
        //        }

        //        if (action == GLFW_PRESS) {
        //            g_shiftPressed = true;
        //        }
        //        else if (action == GLFW_RELEASE) {
        //            g_shiftPressed = false;
        //        }
        //        g_shiftPressed = ! g_shiftPressed;
//    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!g_clicked) {
        return;
    }
    if (g_firstMouse) {
        g_lastX = xpos;
        g_lastY = ypos;
        g_firstMouse = false;
    }

    // float xoffset = xpos - g_lastX;
    float xoffset = g_lastX - xpos;
    float yoffset = g_lastY - ypos; // reversed since y-coordinates go from bottom to top
    g_lastX = xpos;
    g_lastY = ypos;

    float sensitivity = 0.5f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    g_yaw += xoffset;
    g_pitch += yoffset;

    // make sure that when g_pitch is out of bounds, screen doesn't get flipped
    if (g_pitch > 89.0f)
        g_pitch = 89.0f;
    if (g_pitch < -89.0f)
        g_pitch = -89.0f;
    //    bx::Vec3 front;
    //    front.x = bx::cos(bx::toRad(g_yaw)) * bx::cos(bx::toRad(g_pitch));
    //    front.y = bx::sin(bx::toRad(g_pitch));
    //    front.z = bx::sin(bx::toRad(g_yaw)) * bx::cos(bx::toRad(g_pitch));
    //    g_cameraFront = bx::normalize(front);
    updateCameraFront();
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    xoffset *= 5;
    yoffset *= 5;
    if (g_fov >= 1.0f && g_fov <= g_maxFov)
        g_fov -= yoffset;
    if (g_fov <= 1.0f)
        g_fov = 1.0f;
    if (g_fov >= g_maxFov)
        g_fov = g_maxFov;
}

void mouse_button_callback(GLFWwindow* window, int button, int action,
    int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        g_clicked = true;
        g_firstMouse = true;
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        g_clicked = false;
    }
    // popup_menu();
}

// --------------------------------- CAMERA FUNCTIONS
void updateCameraFront()
{

    bx::Vec3 front;
    front.x = bx::cos(bx::toRad(g_yaw)) * bx::cos(bx::toRad(g_pitch));
    front.y = bx::sin(bx::toRad(g_pitch));
    front.z = bx::sin(bx::toRad(g_yaw)) * bx::cos(bx::toRad(g_pitch));
    g_cameraFront = bx::normalize(front);
}
// void focus_callback(GLFWwindow *window, int focused) {
// 	g_focused = focused == GLFW_TRUE;
// }

#endif // GUI_GLFW
