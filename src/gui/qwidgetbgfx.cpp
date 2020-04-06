#include "qwidgetbgfx.h"

//#include <iostream>
#include <QDebug>
#include <QResizeEvent>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
//#include <QtCore>
#include <bx/file.h>
#include <chrono>
#include <engine/scene.h>

namespace {

bool g_showStats = true;
Scene g_scene;
Program g_program;
float g_mtx[16];
bx::Vec3 g_cameraPos = { -7.0f, 1.0f, 0.0f };
float g_yaw = 0.0f;
float g_pitch = 0.0f;
float g_fov = 60.0f;
bx::Vec3 g_cameraFront;
bx::Vec3 g_cameraUp = { 0.0f, 1.0f, 0.0f };
bool g_slowMotion = false;
//std::chrono::duration<double> g_deltaTime;
using timeUnit = std::chrono::microseconds;
//using timeUnit = std::chrono::seconds;
timeUnit g_deltaTime;
int g_cameraMoveFront = 0;
int g_cameraMoveRight = 0;
int g_cameraMoveUp = 0;
std::chrono::time_point<std::chrono::system_clock> g_lastTime;
//std::chrono::high_resolution_clock g_timer;

//int g_width;
//int g_height;

void printDebugMessage();
void updateCameraPos();
void updateCameraFront();

}

QWidgetBgfx::QWidgetBgfx(QWidget* parent)
    : QWidget(parent)
{
    updateCameraFront();

    qDebug() << "QWidgetBgfx(" << parent << ")";
    setAttribute(Qt::WA_NativeWindow);

    // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
    // Most graphics APIs must be used on the same thread that created the window.
    bgfx::renderFrame();

    bgfx::Init bgfxInit = {};
    //    bgfxInit.platformData.ndt = this->winId;
    //    bgfx::PlatformData pd;
    bgfxInit.platformData.ndt = nullptr;
    bgfxInit.platformData.nwh = (void*)(uintptr_t)winId();
    //    bgfxInit.platformData.nwh = (void*)(uintptr_t)this->effectiveWinId();
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);

    bgfxInit.platformData.context = nullptr;
    bgfxInit.platformData.backBuffer = nullptr;
    bgfxInit.platformData.backBufferDS = nullptr;
    //    bgfxInit.platformData.session = nullptr;
    //    bgfx::setPlatformData(pd);
    bgfxInit.type = bgfx::RendererType::Count;
    bgfxInit.resolution.width = this->width();
    bgfxInit.resolution.height = this->height();
    //    bgfxInit.resolution.reset = BGFX_RESET_NONE;
    bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
    bgfxInit.vendorId = BGFX_PCI_ID_NONE;
    if (!bgfx::init(bgfxInit)) {
        Q_ASSERT(false);
    }

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFF0000FF, 1.0f, 0);
    bgfx::touch(0);

    // ------------------------------- LOAD MODEL
    g_scene.addModel(std::string(PROJECT_DIR) + "assets/sponza/sponza.obj");
    //         g_scene.addModel("/home/gauthier/Downloads/Cougar2/cougar.obj");
    //    g_scene.addModel("C:\\Users\\gauthier.bouyjou\\Downloads\\export\\cougar.obj");

    // -------------------------------- INIT PROGRAM (SHADERS)
    g_program.init();

    bx::mtxIdentity(g_mtx);

    setFocus();
}

void QWidgetBgfx::paintEvent(QPaintEvent* event)
{
    //    qDebug() << "QWidgetBgfx::paintEvent(" << event << ")";
    //    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFF0000FF, 1.0f, 0);

    // Set view 0 default viewport.
    bgfx::setViewRect(0, 0, 0, uint16_t(width()), uint16_t(height()));

    // This dummy draw call is here to make sure that view 0 is cleared
    // if no other draw calls are submitted to view 0.
    bgfx::touch(0);

    printDebugMessage();

    //    const std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
    const auto currentTime = std::chrono::system_clock::now();
//    const auto currentTime = g_timer.now();
    g_deltaTime = std::chrono::duration_cast<timeUnit>(currentTime - g_lastTime);
    g_lastTime = currentTime;

    updateCameraPos();

    // --------------------------------- SET CAMERA VIEW
    float view[16];
    // bx::mtxLookAt(view, eye, at);
    bx::mtxLookAt(view, g_cameraPos, bx::add(g_cameraPos, g_cameraFront), g_cameraUp);

    float proj[16];
    bx::mtxProj(proj, g_fov, float(width()) / float(height()), 0.1f, 100.0f,
        bgfx::getCaps()->homogeneousDepth);

    bgfx::setViewTransform(0, view, proj);

    // --------------------------------- DRAW SCENE
    const uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS
        | BGFX_STATE_CULL_CCW | BGFX_STATE_BLEND_NORMAL;
    g_scene.draw(0, g_program, g_mtx, state, g_cameraPos);

    // Advance to next frame. Process submitted rendering primitives.
    bgfx::frame();
    update();
}

void QWidgetBgfx::resizeEvent(QResizeEvent* event)
{
    qDebug() << "QWidgetBgfx::resizeEvent(" << event << ")";
    QWidget::resizeEvent(event);

    const auto& size = event->size();
    Q_ASSERT(size.width() == width());
    //    const auto & size = event->size();
    //    bgfx::reset(size.width(), size.height(), BGFX_RESET_VSYNC);
    bgfx::reset(size.width(), size.height(), BGFX_RESET_NONE);
    //    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
}

QPaintEngine* QWidgetBgfx::paintEngine() const
{
    //    qDebug() << "QWidgetBgfx::paintEngine()";
    return nullptr;
}

void QWidgetBgfx::keyPressEvent(QKeyEvent* event)
{
    //    qDebug() << "QWidgetBgfx::keyPressEvent(" << event << ")";
    if (event->key() == Qt::Key_F1) {
        g_showStats = !g_showStats;
    } else if (event->key() == Qt::Key_Up) {
        g_cameraMoveFront = 1;
    } else if (event->key() == Qt::Key_Down) {
        g_cameraMoveFront = -1;
    } else if (event->key() == Qt::Key_Right) {
        g_cameraMoveRight = 1;
    } else if (event->key() == Qt::Key_Left) {
        g_cameraMoveRight = -1;
    } else if (event->key() == Qt::Key_PageUp) {
        g_cameraMoveUp = 1;
    } else if (event->key() == Qt::Key_PageDown) {
        g_cameraMoveUp = -1;
    }
    //    else if (event->key() == Qt::Key_Up) {
    //        g_cameraMoveFront = 1;
    //    }
    else {
        QWidget::keyPressEvent(event);
    }
}

void QWidgetBgfx::keyReleaseEvent(QKeyEvent* event)
{
    //    qDebug() << "QWidgetBgfx::keyReleaseEvent(" << event << ")";
    //    if (event->key() == Qt::Key_F1) {
    //        g_showStats = !g_showStats;
    //    }
    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down) {
        g_cameraMoveFront = 0;
    } else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_Left) {
        g_cameraMoveRight = 0;
    } else if (event->key() == Qt::Key_PageUp || event->key() == Qt::Key_PageDown) {
        g_cameraMoveUp = 1;
    }
    //    else if (event->key() == Qt::Key_Up) {
    //        g_cameraMoveFront = 1;
    //    }
    else {
        QWidget::keyReleaseEvent(event);
    }
}

// ---------------------------------------------- PRIVATE
namespace {

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
        //                bgfx::dbgTextPrintf(0, ++line, 0x0F, " %s / " BX_COMPILER_NAME " / " BX_CPU_NAME " / " BX_ARCH_NAME " / " BX_PLATFORM_NAME " ", bgfx::getRendererName(bgfx::getRendererType());
        //        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Fps:%.1f | Verts:%d | Tris:%d | Verts/Tris:%.2f | Objects:%d | Textures:%d (%.1f MiB)",
        //            g_fps, g_nbVertices, g_nbTriangles, (float)g_nbVertices / g_nbTriangles, g_nbObjects, g_nbTextures, g_texturesSize);
        //        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Vertex buffer:%d | Index buffer:%d | Index buffer/Vertex buffer:%.2f",
        //            g_nbVertexBuffer, g_nbIndexBuffer, (float)g_nbIndexBuffer / g_nbVertexBuffer);
        //        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Viewport shading: %s", g_viewportShading.c_str());
        //        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Arch: " BX_COMPILER_NAME " / " BX_CPU_NAME " / " BX_ARCH_NAME " / " BX_PLATFORM_NAME " ");
        //        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Renderer: %s", g_renderer.c_str());
        //        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Graphic Vendor: %s Corporation", g_vendorID.c_str());
        //        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Backbuffer %dx%d in pixels", stats->width, stats->height);
        //        bgfx::dbgTextPrintf(0, ++line, 0x0F, "TinyObj parsing time: %d ms", g_parsingTime);
        //        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Loading materials time: %d ms", g_loadingMaterialsTime);
        //        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Loading objects time: %d ms", g_loadingObjectsTime);
        //        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Total loading time: %d ms", g_totalLoadingTime);
        //        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Total draw call: %d", g_nbIndexBuffer + 1);
    }
}

//void processInput(GLFWwindow* window)
void updateCameraPos()
{

    //    float cameraSpeed = g_shiftPressed ? 1.0f : 5.0f;
    float cameraSpeed = g_slowMotion ? 1.0f : 5.0f;
    //    cameraSpeed = 0.01f;
    cameraSpeed *= g_deltaTime.count();
    qDebug() << g_deltaTime.count();
    //    cameraSpeed= 5.0 * g_deltaTime;

    //    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    if (g_cameraMoveFront) {
        if (g_cameraMoveFront > 0) {
            g_cameraPos = bx::add(g_cameraPos, bx::mul(g_cameraFront, cameraSpeed));
        } else {
            g_cameraPos = bx::sub(g_cameraPos, bx::mul(g_cameraFront, cameraSpeed));
        }
    }
    //        g_cameraPos = bx::add(g_cameraPos, bx::mul(g_cameraFront, cameraSpeed));
    //    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    //        g_cameraPos = bx::sub(g_cameraPos, bx::mul(g_cameraFront, cameraSpeed));
    if (g_cameraMoveUp) {
        if (g_cameraMoveUp > 0) {
            //                g_cameraPos = bx::add(g_cameraPos, bx::mul(g_cameraFront, cameraSpeed));
            g_cameraPos = bx::add(g_cameraPos, bx::mul(g_cameraUp, cameraSpeed));
        } else {
            g_cameraPos = bx::sub(g_cameraPos, bx::mul(g_cameraUp, cameraSpeed));
        }
    }
    //        g_cameraPos = bx::add(g_cameraPos, bx::mul(g_cameraFront, cameraSpeed));

    //    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
    //        g_cameraPos = bx::add(g_cameraPos, bx::mul(g_cameraUp, cameraSpeed));
    //    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
    //        g_cameraPos = bx::sub(g_cameraPos, bx::mul(g_cameraUp, cameraSpeed));
    if (g_cameraMoveRight) {
        if (g_cameraMoveRight > 0) {
            //                g_cameraPos = bx::add(g_cameraPos, bx::mul(g_cameraFront, cameraSpeed));
            //            g_cameraPos = bx::add(g_cameraPos, bx::mul(g_cameraUp, cameraSpeed));
            g_cameraPos = bx::sub(
                g_cameraPos, bx::mul(bx::normalize(bx::cross(g_cameraFront, g_cameraUp)), cameraSpeed));
        } else {
            //            g_cameraPos = bx::sub(g_cameraPos, bx::mul(g_cameraUp, cameraSpeed));
            g_cameraPos = bx::add(
                g_cameraPos, bx::mul(bx::normalize(bx::cross(g_cameraFront, g_cameraUp)), cameraSpeed));
        }
    }

    //    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    //        g_cameraPos = bx::sub(
    //            g_cameraPos, bx::mul(bx::normalize(bx::cross(g_cameraFront, g_cameraUp)), cameraSpeed));
    //    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    //        g_cameraPos = bx::add(
    //            g_cameraPos, bx::mul(bx::normalize(bx::cross(g_cameraFront, g_cameraUp)), cameraSpeed));
}

void updateCameraFront()
{

    bx::Vec3 front;
    front.x = bx::cos(bx::toRad(g_yaw)) * bx::cos(bx::toRad(g_pitch));
    front.y = bx::sin(bx::toRad(g_pitch));
    front.z = bx::sin(bx::toRad(g_yaw)) * bx::cos(bx::toRad(g_pitch));
    g_cameraFront = bx::normalize(front);
}

}
