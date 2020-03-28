#include "bgfx/platform.h"
#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/file.h>
#include <bx/macros.h>
#include <bx/uint32_t.h>

#include <GLFW/glfw3.h>
#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
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
#include "scene.h"
#include <cassert>
#include <fstream>
#include <iostream>
//#include "Vertex.h"
#include "program.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
//#define WIN_WIDTH 1920
//#define WIN_HEIGHT 1080

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action,
    int mods);
// void focus_callback(GLFWwindow *window, int focused);
void init();
void shutdown();
void update();

//bgfx::ShaderHandle loadShader(const char* FILENAME);
void updateCameraFront();

// ----------------- GLOBAL VARS
GLFWwindow* g_window = nullptr;
// bgfx::ProgramHandle g_program;
Program g_program;
unsigned int g_counter = 0;
// Mesh* g_mesh = nullptr;
Scene g_scene;
bool g_clicked = false;

float g_deltaTime = 0.0f;
//float g_lastFrame = 0.0f;

bool g_firstMouse = true;
float g_yaw = 0.0f;
float g_pitch = 0.0f;
float g_lastX = WIN_WIDTH / 2.0f;
float g_lastY = WIN_HEIGHT / 2.0f;
float g_fov = 60.0f;
bx::Vec3 g_cameraPos = { 0.0f, 2.0f, 0.0f };
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
std::string g_renderer;
std::string g_vendorID;
int g_debugHMargin;
size_t g_nbVertexBuffer;
size_t g_nbIndexBuffer;

int main(int argc, char const* argv[])
{
    updateCameraFront(); // with current g_yaw and g_pitch
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
    const int epoch = 50;
    while (!glfwWindowShouldClose(g_window)) {
        bgfx::dbgTextClear();

        const auto currentTime = glfwGetTime();
        g_deltaTime = currentTime - lastTime; // need deltaTime at each frame by processInput()
        lastTime = currentTime;
        sum += g_deltaTime;

        if (g_counter % epoch == 0) {
            //		    g_lastFrame = currentFrame;

            g_fps = epoch / sum; // sum is float
            // fps = 1.0f / g_deltaTime;
            sum = 0.0f;
        }
        //        bgfx::dbgTextPrintf(0, 0, 0x08, "fps = %.1f", g_fps);

        processInput(g_window);

        update();

        // bgfx::frame();

        g_counter++;
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

void init()
{

    // --------------------- INIT GLFW
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    g_window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Hello, bgfx from glfw!",
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
    // glfwSetWindowFocusCallback(g_window, focus_callback);

    // glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // bgfx::RenderFrame();
    bgfx::renderFrame();

    // bgfx::PlatformData pd;
    // pd.nwh = glfwGetWin32Window(window);
    // bgfx::setPlatformData(pd);

    bgfx::Init bgfxInit = {};
//	bgfxInit.platformData.nwh = glfwGetWin32Window(g_window);
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    bgfxInit.platformData.ndt = glfwGetX11Display();
    bgfxInit.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(g_window);
#elif BX_PLATFORM_WINDOWS
    bgfxInit.platformData.nwh = glfwGetWin32Window(g_window);
#endif
    bgfxInit.platformData.context = NULL;
    bgfxInit.platformData.backBuffer = NULL;
    bgfxInit.platformData.backBufferDS = NULL;

    // std::cout << RENDERER_TYPE << std::endl;
#ifdef RENDERER_OpenGL
    bgfxInit.type = bgfx::RendererType::OpenGL;
//    std::cout << "RendererType OpenGL" << std::endl;
#endif
#ifdef RENDERER_OpenGLES
    bgfxInit.type = bgfx::RendererType::OpenGLES;
#endif
#ifdef RENDERER_Vulkan
    bgfxInit.type = bgfx::RendererType::Vulkan;
//    std::cout << "RendererType Vulkan" << std::endl;
#endif
#ifdef RENDERER_Direct3D12
    bgfxInit.type = bgfx::RendererType::Direct3D12;
//    std::cout << "RendererType Direct3D12" << std::endl;
#endif
#ifdef RENDERER_Metal
    bgfxInit.type = bgfx::RendererType::Metal;
#endif
#ifdef RENDERER_Auto
    bgfxInit.type = bgfx::RendererType::Count; // Automatically choose renderer
//    std::cout << "[main] RendererType auto" << std::endl;
#endif
    bgfxInit.resolution.width = WIN_WIDTH;
    bgfxInit.resolution.height = WIN_HEIGHT;
    bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
    //            bgfxInit.resolution.reset = BGFX_RESET_NONE;
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

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFF0000FF, 1.0f,
        0);
    bgfx::setViewRect(0, 0, 0, WIN_WIDTH, WIN_HEIGHT);
    bgfx::touch(0);

    // glfwMakeContextCurrent(nullptr); // question : why we can do it ?

    // ------------------------------- LOADING MODEL
        g_scene.addModel(std::string(PROJECT_DIR) + "assets/sponza/sponza.obj");
    //    g_scene.addModel(std::string(PROJECT_DIR) + "assets/McGuire/Dabrovic_Sponza/sponza-blend.obj");
    //    g_scene.addModel(std::string(PROJECT_DIR) + "assets/McGuire/Crytek_Sponza/sponza-blend.obj");
    //    g_scene.addModel(std::string(PROJECT_DIR) + "assets/McGuire/San_Miguel/san-miguel-blend.obj");

    //    g_scene.addModel("/home/gauthier/Downloads/Cougar/Cougar.obj");
//    g_scene.addModel("/home/gauthier/Downloads/Cougar2/cougar.obj");
    //    g_scene.addModel("C:\\Users\\gauthier.bouyjou\\Downloads\\export\\Cougar.obj");

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

    const bgfx::Caps* caps = bgfx::getCaps();
    assert(caps->supported & BGFX_CAPS_FORMAT_TEXTURE_MIP_AUTOGEN);
    //    switch (bgfx::getRendererType()) {
    switch (caps->rendererType) {
    case bgfx::RendererType::Noop:
    case bgfx::RendererType::Direct3D9:
        g_renderer = "DirectX9";
        break;
    case bgfx::RendererType::Direct3D11:
        g_renderer = "DirectX11";
        break;
    case bgfx::RendererType::Direct3D12:
        g_renderer = "DirectX12";
        break;
    // case bgfx::RendererType::Gnm:
    // shaderPath = "shaders/bin/pssl/";
    // break;
    case bgfx::RendererType::Metal:
        g_renderer = "Metal";
        break;
    case bgfx::RendererType::OpenGLES:
        g_renderer = "OpenGLES";
        break;
    case bgfx::RendererType::OpenGL:
        g_renderer = "OpenGL";
        break;
    case bgfx::RendererType::Vulkan:
        g_renderer = "Vulkan";
        break;
    }
    g_debugHMargin = (g_renderer == "OpenGL") ? (0) : (2);

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
    g_program.init("cubes");

    // g_texColor = bgfx::createUniform("s_texColor",
    // bgfx::UniformType::Sampler);

    // bgfx::ProgramHandle program;
    // program = loadProgram("cubes.vert", "cubes.frag");
}

void update()
{
    // Set view 0 default viewport.
    bgfx::setViewRect(0, 0, 0, uint16_t(WIN_WIDTH), uint16_t(WIN_HEIGHT));

    // This dummy draw call is here to make sure that view 0 is cleared
    // if no other draw calls are submitted to view 0.
    bgfx::touch(0);

    //        int margin = 2;
    //   const int margin = (g_renderer == "OpenGL") ? (0) : (2);
    //    int line = g_debugHMargin - 1;
    int line = -1;
    bgfx::dbgTextPrintf(0, ++line, 0x0F, "Fps:%.1f | Verts:%d | Tris:%d | Verts/Tris:%.2f | Objects:%d | Textures:%d (%.1f MiB)",
        g_fps, g_nbVertices, g_nbTriangles, (float)g_nbVertices / g_nbTriangles, g_nbObjects, g_nbTextures, g_texturesSize);
    bgfx::dbgTextPrintf(0, ++line, 0x0F, "Vertex buffer:%d | Index buffer:%d | Index buffer/Vertex buffer:%.2f",
        g_nbVertexBuffer, g_nbIndexBuffer, (float)g_nbIndexBuffer / g_nbVertexBuffer);
    bgfx::dbgTextPrintf(0, ++line, 0x0F, "Current renderer = %s", g_renderer.c_str());
    bgfx::dbgTextPrintf(0, ++line, 0x0F, "Graphic Vendor ID = %s", g_vendorID.c_str());
    bgfx::dbgTextPrintf(0, ++line, 0x0F, "Parsing time = %d ms", g_parsingTime);
    bgfx::dbgTextPrintf(0, ++line, 0x0F, "Loading materials time = %d ms", g_loadingMaterialsTime);
    bgfx::dbgTextPrintf(0, ++line, 0x0F, "Loading objects time = %d ms", g_loadingObjectsTime);
    bgfx::dbgTextPrintf(0, ++line, 0x0F, "Total loading time = %d ms", g_totalLoadingTime);

    // ------------------------- RENDER SCENE
    const bx::Vec3 at = { 0.0f, 2.0f, 0.0f };
    const bx::Vec3 eye = { 0.0f, 2.0f, -2.0f };
    // {
    float view[16];
    // bx::mtxLookAt(view, eye, at);
    bx::mtxLookAt(view, g_cameraPos, bx::add(g_cameraPos, g_cameraFront), g_cameraUp);

    float proj[16];
    bx::mtxProj(proj, g_fov, float(WIN_WIDTH) / float(WIN_HEIGHT), 0.1f, 100.0f,
        bgfx::getCaps()->homogeneousDepth);

    bgfx::setViewTransform(0, view, proj);

    float mtx[16];
    // bx::mtxRotateXY(mtx, g_counter * 0.01f, g_counter * 0.01f);
    // if (g_focused) {
    bx::mtxIdentity(mtx);
    // bx::mtxScale(mtx, 0.01f);
    // } else {

    // bx::mtxRotateY(mtx, g_counter * 0.01f);
    // }
    // bx::mtxScale(mtx, 0.1f);
    // bgfx::setTransform(mtx);

    // const uint64_t state = 0 | BGFX_STATE_WRITE_R | BGFX_STATE_WRITE_G |
    // 					   BGFX_STATE_WRITE_B | BGFX_STATE_WRITE_A |
    // 					   BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS |
    // 					   BGFX_STATE_CULL_CCW | BGFX_STATE_MSAA | s_ptState[0];

    //        const uint64_t state = 0 | BGFX_STATE_PT_TRISTRIP |
    //        BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
    //        BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CCW |
    //        BGFX_STATE_BLEND_NORMAL;
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

    g_scene.draw(0, g_program, mtx, state);

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

// ------------------------------------ GLFW FUNCTIONS
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        // if (g_focused) {
        // }
        glfwSetWindowShouldClose(window, true);
    }

    float cameraSpeed = 5.0 * g_deltaTime;
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
    // glViewport(0, 0, width, height);
}

void updateCameraFront()
{

    bx::Vec3 front;
    front.x = bx::cos(bx::toRad(g_yaw)) * bx::cos(bx::toRad(g_pitch));
    front.y = bx::sin(bx::toRad(g_pitch));
    front.z = bx::sin(bx::toRad(g_yaw)) * bx::cos(bx::toRad(g_pitch));
    g_cameraFront = bx::normalize(front);
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

// void focus_callback(GLFWwindow *window, int focused) {
// 	g_focused = focused == GLFW_TRUE;
// }
