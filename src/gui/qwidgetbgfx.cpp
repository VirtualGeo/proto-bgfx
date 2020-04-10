#ifdef GUI_QT

#include "qwidgetbgfx.h"

//#include <iostream>
#include <QDebug>
#include <QResizeEvent>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
//#include <QtCore>
#include <bx/file.h>
#include <chrono>
#include <engine/camerafps.h>
#include <engine/scene.h>
#include <QSurface>
#include <QApplication>


namespace {
    std::vector<WindowState> s_windows;
    Scene s_scene;

    bool g_showStats = false;
    float g_mtx[16];

    std::string g_renderer;
    std::string g_vendorID;

}

//CameraFps g_cameraFps({ -7.0f, 1.0f, 0.0f });

using timeUnit = std::chrono::microseconds;


QWidgetBgfx::QWidgetBgfx(QWidget* parent)
    : QWidget(parent)
    , m_cameraFps({ -7.0f, 1.0f, 0.0f })
{
    //    updateCameraFront();

    qDebug() << "QWidgetBgfx(" << parent << ")";
    setAttribute(Qt::WA_NativeWindow);
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);

}

QWidgetBgfx::~QWidgetBgfx()
{
    if (m_iWindow == 0) {
        s_scene.clear();
        //        s_program.clear();
        Program::clear();

        bgfx::shutdown();
    }

#ifdef DEBUG
    std::cout << "[main] shutdown done" << std::endl;
#endif
}

void QWidgetBgfx::render()
{
    if (m_updatePending == false) {
        m_updatePending = true;
        QApplication::postEvent(this, new QEvent{QEvent::UpdateRequest});
    }

}

void QWidgetBgfx::doRender()
{
    if (isVisible() == false)
        return;
    if (m_isInit == false)
        return;

    //    qDebug() << "QWidgetBgfx::paintEvent(" << event << ")";
    //    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFF0000FF, 1.0f, 0);

    if (m_iWindow == 0) {
        printDebugMessage();
    }

    //    const std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
    //    const auto currentTime = std::chrono::system_clock::now();
    const auto currentTime = std::chrono::high_resolution_clock::now();
    auto& window = s_windows[m_iWindow];
    //    const auto currentTime = g_timer.now();
    g_deltaTime = std::chrono::duration_cast<timeUnit>(currentTime - g_lastTime).count() / 1000000.0;
    g_lastTime = currentTime;
    g_sum += g_deltaTime;

    //    qDebug() << g_epoch << g_sum << g_fps;
    if (g_counter >= g_epoch) {
        //        g_epoch = (g_fps = g_epoch / g_sum) / 2; // update g_fps each 0.5 sec
        g_epoch = (window.m_fps = g_epoch / g_sum); // update g_fps each sec
        g_sum = 0.0f;
        g_counter = 0;
    }
    ++g_counter;

    updateCameraPos();

    // --------------------------------- SET CAMERA VIEW
    //    float view[16];
    //    // bx::mtxLookAt(view, eye, at);
    //    bx::mtxLookAt(view, g_cameraFps.m_pos, bx::add(g_cameraFps.m_pos, g_cameraFps.m_front), g_cameraFps.m_up);

    //    float proj[16];
    //    bx::mtxProj(proj, g_cameraFps.m_fov, float(width()) / float(height()), 0.1f, 100.0f,
    //        bgfx::getCaps()->homogeneousDepth);

    //    bgfx::setViewTransform(m_iWidget, view, proj);

    if (m_iWindow != 0) {
        bgfx::setViewFrameBuffer(m_iWindow, window.m_fbh);
    }
    // Set view 0 default viewport.
    bgfx::setViewRect(m_iWindow, 0, 0, uint16_t(width()), uint16_t(height()));
    // This dummy draw call is here to make sure that view 0 is cleared
    // if no other draw calls are submitted to view 0.
    bgfx::setViewClear(m_iWindow, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0X00FF00FF);
    bgfx::touch(m_iWindow);


    // --------------------------------- DRAW SCENE
    const uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS
        | BGFX_STATE_CULL_CCW | BGFX_STATE_BLEND_NORMAL;
    const float ratio = float(width()) / height();
    s_scene.draw(m_iWindow, window.m_shading, g_mtx, state, m_cameraFps, ratio);
    //    g_scene.draw(1, g_program, g_mtx, state, g_cameraPos);

    // Advance to next frame. Process submitted rendering primitives.
//    if (m_iWindow == 0) {
        bgfx::frame();
//    }

    if (m_continuousRender == true)
        render();
}

void QWidgetBgfx::init()
{
    if (s_windows.empty()) {
        //        s_bgfxInit = true;
        // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
        // Most graphics APIs must be used on the same thread that created the window.
        bgfx::renderFrame();

        bgfx::Init bgfxInit = {};
        //    bgfxInit.platformData.ndt = this->winId;
        //    bgfx::PlatformData pd;
        bgfxInit.platformData.ndt = nullptr;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
        //    bgfxInit.platformData.ndt = glfwGetX11Display();
        bgfxInit.platformData.nwh = (void*)(uintptr_t)winId();
#elif BX_PLATFORM_OSX
        //    bgfxInit.platformData.nwh = glfwGetCocoaWindow(g_window);
        bgfxInit.platformData.nwh = (void*)(uintptr_t)winId();
#elif BX_PLATFORM_WINDOWS
        //    bgfxInit.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(g_window);
        bgfxInit.platformData.nwh = (void*)(uintptr_t)winId();
#endif
        //    bgfxInit.platformData.nwh = nullptr;
        //    bgfxInit.platformData.nwh = (void*)(uintptr_t)this->effectiveWinId();
        bgfxInit.platformData.context = nullptr;
        bgfxInit.platformData.backBuffer = nullptr;
        bgfxInit.platformData.backBufferDS = nullptr;
        //    bgfxInit.platformData.session = nullptr;
        //    bgfx::setPlatformData(pd);
        bgfxInit.type = bgfx::RendererType::Count; // Automatically choose renderer
        //    bgfxInit.type = bgfx::RendererType::Direct3D9;
        //    bgfxInit.type = bgfx::RendererType::Direct3D11;
        //    bgfxInit.type = bgfx::RendererType::Direct3D12;
        //    bgfxInit.type = bgfx::RendererType::OpenGL;
        //    bgfxInit.type = bgfx::RendererType::OpenGLES;
        //    bgfxInit.type = bgfx::RendererType::Vulkan;
        //    bgfxInit.type = bgfx::RendererType::Metal;

        bgfxInit.resolution.width = this->width();
        bgfxInit.resolution.height = this->height();
        //    bgfxInit.resolution.reset = BGFX_RESET_NONE;
        bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
        bgfxInit.vendorId = BGFX_PCI_ID_NONE;
        if (!bgfx::init(bgfxInit)) {
            Q_ASSERT(false);
        }
        //         g_scene.addModel("/home/gauthier/Downloads/Cougar2/cougar.obj");

        const bgfx::Caps* caps = bgfx::getCaps();
        g_renderer = bgfx::getRendererName(caps->rendererType);

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

        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFF0000FF, 1.0f, 0);
        bgfx::touch(0);

        // ------------------------------- LOAD MODEL
        s_scene.addModel(std::string(PROJECT_DIR) + "assets/sponza/sponza.obj");
        //         g_scene.addModel("/home/gauthier/Downloads/Cougar2/cougar.obj");
        //    g_scene.addModel("C:\\Users\\gauthier.bouyjou\\Downloads\\export\\cougar.obj");
        //        g_nbVertices = g_scene.nbVertices();
        //        g_nbTriangles = g_scene.nbTriangles();
        //        g_nbObjects = g_scene.nbObjects();
        //        g_texturesSize = g_scene.texturesSize() / 1000000.0f;
        //        g_nbTextures = g_scene.nbTextures();
        //        g_parsingTime = g_scene.parsingTime();
        //        g_loadingMaterialsTime = g_scene.loadingMaterialsTime();
        //        g_loadingObjectsTime = g_scene.loadingObjectsTime();
        //        g_totalLoadingTime = g_parsingTime + g_loadingMaterialsTime + g_loadingObjectsTime;
        //        g_nbVertexBuffer = g_scene.nbVertexBuffer();
        //        g_nbIndexBuffer = g_scene.nbIndexBuffer();

        // -------------------------------- INIT PROGRAM (SHADERS)
        //        s_program.init();
        Program::init();
        //        s_program.setShading(Program::Shading(g_iViewportShading));
        //        g_viewportShading = Program::shadingFileNames[g_iViewportShading];
    }
    //    m_fbh = bgfx::createFrameBuffer((void*)(uintptr_t)winId(), uint16_t(width()), uint16_t(height()));
    m_iWindow = s_windows.size();
    //    m_windows[m_iWindow].m_fbh = bgfx::createFrameBuffer((void*)(uintptr_t)winId(), uint16_t(width()), uint16_t(height()));
    //    winId();
    void* nwh = (void*)(uintptr_t)winId();
    //    WindowState wstate(nwh, 20, 40);
    s_windows.push_back(WindowState(nwh, width(), height()));
    //    m_windows.emplace_back(nwh, width(), height());
    //    bgfx::setViewFrameBuffer(m_iWidget, m_fbh);

    bx::mtxIdentity(g_mtx);

    //    setFocus();
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    g_lastTime = std::chrono::high_resolution_clock::now();
    // g_lastTime = std::chrono::steady_clock::now();
    //    m_iWidget = g_nWidget;
    //    bgfx::setViewFrameBuffer(m_iWidget, m_fbh);
    //    m_iWidget = 1;
    //    ++g_nWidget;

    m_isInit = true;
}

void QWidgetBgfx::paintEvent(QPaintEvent* event)
{
    if (m_isInit == false) {
        init();
    }
    render();
//    return;
//    update();
}

void QWidgetBgfx::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    if (m_isInit == false) {
        init();
    }
}

bool QWidgetBgfx::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        m_updatePending = false;
        doRender();
        return true;

    default:
        return QWidget::event(event);
    }

}

void QWidgetBgfx::resizeEvent(QResizeEvent* event)
{
    qDebug() << "QWidgetBgfx::resizeEvent(" << event << ")";
    QWidget::resizeEvent(event);

    if (m_isInit == false) {
        init();
    }

    auto& window = s_windows[m_iWindow];
    const auto& size = event->size();
    Q_ASSERT(size.width() == width());
    if ((size.width() < 0) || (size.height() < 0)) {
        return;
    }
    //    g_width = size.width();
    window.m_width = size.width();
    //    g_height = size.height();
    window.m_height = size.height();
    resetWindow();

    //    m_fbh = bgfx::createFrameBuffer((void*)(uintptr_t)winId(), uint16_t(width()), uint16_t(height()));
    bgfx::destroy(window.m_fbh);
    window.m_fbh = bgfx::createFrameBuffer((void*)(uintptr_t)winId(), uint16_t(width()), uint16_t(height()));

    render();
}

QPaintEngine* QWidgetBgfx::paintEngine() const
{
    //    qDebug() << "QWidgetBgfx::paintEngine()";
    return nullptr;
}

void QWidgetBgfx::keyPressEvent(QKeyEvent* event)
{
    auto& window = s_windows[m_iWindow];
    auto& shading = window.m_shading;

    qDebug() << "QWidgetBgfx::keyPressEvent(" << event << ")";
    switch (event->key()) {
    case Qt::Key_F1:
        g_showStats = !g_showStats;
        break;
    case Qt::Key_F2:
        g_vsyncEnable = !g_vsyncEnable;
        g_epoch = 10;
        resetWindow();
        break;
    case Qt::Key_F3:
        g_mssaLevel = ++g_mssaLevel % 5;
        resetWindow();
        break;
    case Qt::Key_F4:
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
        }
        break;
    case Qt::Key_F5:
        ++shading;
        //        shading = ++shading % Program::Shading::Count;
        break;
    case Qt::Key_Up:
        m_cameraMoveFront = 1;
        break;
    case Qt::Key_Down:
        m_cameraMoveFront = -1;
        break;
    case Qt::Key_Right:
        m_cameraMoveRight = 1;
        break;
    case Qt::Key_Left:
        m_cameraMoveRight = -1;
        break;
    case Qt::Key_PageUp:
        m_cameraMoveUp = 1;
        break;
    case Qt::Key_PageDown:
        m_cameraMoveUp = -1;
        break;
    case Qt::Key_N:
        shading = Shading::NORMAL;
        break;
    case Qt::Key_M:
        shading = Shading::MATERIAL;
        break;
    case Qt::Key_R:
        shading = Shading::RENDERED;
        break;
    case Qt::Key_Control:
        g_slowMotion = !g_slowMotion;
        break;
    default:
        QWidget::keyPressEvent(event);
        break;
    }
}

void QWidgetBgfx::keyReleaseEvent(QKeyEvent* event)
{
    switch (event->key()) {
    case Qt::Key_Up:
    case Qt::Key_Down:
        m_cameraMoveFront = 0;
        break;
    case Qt::Key_Right:
    case Qt::Key_Left:
        m_cameraMoveRight = 0;
        break;
    case Qt::Key_PageUp:
    case Qt::Key_PageDown:
        m_cameraMoveUp = 0;
        break;
    default:
        QWidget::keyReleaseEvent(event);
        break;
    }
}

void QWidgetBgfx::mousePressEvent(QMouseEvent* event)
{
    qDebug() << this << "QWidgetBgfx::mousePressEvent(" << event << ")";
    if (event->button() == Qt::LeftButton) {
        g_mouseLeftClicked = true;
        g_firstMouse = true;
    }
}

void QWidgetBgfx::mouseReleaseEvent(QMouseEvent* event)
{
    //    qDebug() << "QWidgetBgfx::mouseReleaseEvent(" << event << ")";
    if (event->button() == Qt::LeftButton) {
        g_mouseLeftClicked = false;
    }
}

void QWidgetBgfx::mouseMoveEvent(QMouseEvent* event)
{
    //        qDebug() << "QWidgetBgfx::mouseMoveEvent(" << event << ")";
    if (g_mouseLeftClicked) {
        int xpos = event->x();
        int ypos = event->y();
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

        m_cameraFps.rotate(xoffset, yoffset);
    }
}


void QWidgetBgfx::updateCameraPos()
{
    float cameraSpeed = g_slowMotion ? 1.0f : 5.0f;
    cameraSpeed *= g_deltaTime;

    if (m_cameraMoveFront) {
        m_cameraFps.move(Camera::Direction::FRONT, cameraSpeed * m_cameraMoveFront);
    }
    if (m_cameraMoveUp) {
        m_cameraFps.move(Camera::Direction::UP, cameraSpeed * m_cameraMoveUp);
    }
    if (m_cameraMoveRight) {
        m_cameraFps.move(Camera::Direction::RIGHT, cameraSpeed * m_cameraMoveRight);
    }
}

void QWidgetBgfx::printDebugMessage()
{
    if (g_showStats) {
        bgfx::setDebug(BGFX_DEBUG_STATS);
    } else {
        bgfx::setDebug(BGFX_DEBUG_TEXT);
        //        if (m_iWidget == 0) {
        bgfx::dbgTextClear();
        //        if (m_iWig)
        const bgfx::Stats* stats = bgfx::getStats();
        int line = -1;
        bgfx::dbgTextPrintf(0, ++line, 0x2F, "F1:Stats | F2:Vsync | F3:Msaa | F4:Sampler | F5:Shading | F6: | F7:");
        //        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Fps:%.2f", g_fps);
        //            bgfx::dbgTextPrintf(0, ++line, 0x0F, "Viewport shading: %s", g_viewportShading.c_str());
        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Arch: " BX_COMPILER_NAME " / " BX_CPU_NAME " / " BX_ARCH_NAME " / " BX_PLATFORM_NAME " ");
        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Renderer: %s", g_renderer.c_str());
        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Graphic Vendor: %s Corporation", g_vendorID.c_str());
        //            bgfx::dbgTextPrintf(0, ++line, 0x0F, "Fps:%.1f", g_fps);
        //        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Fps: %.2f, Backbuffer: %dx%d, Viewport shading: %s", g_fps, stats->width, stats->height, g_viewportShading.c_str());
        for (int i = 0; i < s_windows.size(); ++i) {
            //        for (const auto& window : m_windows) {
            const auto& window = s_windows[i];
            bgfx::dbgTextPrintf(0, ++line, 0x0F, "Window: %d, Fps: %.2f, Backbuffer: %dx%d, Viewport shading: %s", i, window.m_fps, window.m_width, window.m_height, Program::filename(window.m_shading));
        }
        s_scene.printStats(line);
    }
}

void QWidgetBgfx::resetWindow()
{
    //    bgfx::reset(g_width, g_height, getResetFlags());
    if (m_iWindow == 0) {
        const auto& window = s_windows[m_iWindow];
        bgfx::reset(window.m_width, window.m_height, getResetFlags());
    }

    //    bgfx::setViewRect(m_iWindow, 0, 0, bgfx::BackbufferRatio::Equal);
}

uint32_t QWidgetBgfx::getResetFlags()
{

    uint32_t flags = 0 | BGFX_RESET_NONE;
    if (g_vsyncEnable) {
        flags |= BGFX_RESET_VSYNC;
    }

    switch (g_mssaLevel) {
    case 0:
        break;
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
//    QSurfaceFormat format;
//    format.setVersion(4, 1);
////    format.setProfile(QSurfaceFormat::CoreProfile);
//    format.setProfile(QSurfaceFormat::CompatibilityProfile);
//    format.setDepthBufferSize(24);
//    format.setSamples(bx::pow(2, g_mssaLevel));
//    QSurfaceFormat::setDefaultFormat(format);

    return flags;
}

#endif
