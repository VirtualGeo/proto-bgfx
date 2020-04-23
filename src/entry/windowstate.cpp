#include "windowstate.h"
//#include <QDebug>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <cassert>
#include <engine/camerafps.h>
#include <entry/entry.h>
#include <list>

//std::vector<WindowState*> WindowState::s_windows;
//std::vector<WindowState*> s_windows;
std::list<WindowState*> s_windows;
//std::list<WindowState&> s_windows;

WindowState::WindowState(void* nwh, int width, int height)
    : m_nwh(nwh)
    , m_width(width)
    , m_height(height)
    , m_id(s_windows.size())
    //    , m_fbh(bgfx::createFrameBuffer(nwh, uint16_t(width), uint16_t(height)))
    , m_iCamera(entry::s_cameras.size())
{
    //    entry::s_cameras.emplace_back(std::make_unique<CameraFps>(bx::Vec3 { -7.0f, 1.0f, 0.0f })); // question : push_back ?
    //    std::cout << &entry::s_cameras << std::endl;
    entry::s_cameras.emplace_back(std::make_unique<CameraFps>(bx::Vec3 { -7.0f, 1.0f, 0.0f })); // question : push_back ?

    m_fbh.idx = bgfx::kInvalidHandle;
    //    Q_ASSERT(! bgfx::isValid(m_fbh));
    assert(!bgfx::isValid(m_fbh));

    //    if (entry::s_windows.empty()) {
    if (m_id == 0) {
        //        s_bgfxInit = true;
        // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
        // Most graphics APIs must be used on the same thread that created the window.
        bgfx::renderFrame();

        bgfx::Init bgfxInit = {};
        //    bgfxInit.platformData.ndt = this->winId;
        //    bgfx::PlatformData pd;
        bgfxInit.platformData.ndt = nullptr;
        bgfxInit.platformData.nwh = nwh;
        //#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
        //        //    bgfxInit.platformData.ndt = glfwGetX11Display();
        //        bgfxInit.platformData.nwh = nwh;
        //#elif BX_PLATFORM_OSX
        //        //    bgfxInit.platformData.nwh = glfwGetCocoaWindow(g_window);
        //        bgfxInit.platformData.nwh = nwh;
        //#elif BX_PLATFORM_WINDOWS
        //        //    bgfxInit.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(g_window);
        //        bgfxInit.platformData.nwh = nwh;
        //#endif
        //    bgfxInit.platformData.nwh = nullptr;
        //    bgfxInit.platformData.nwh = (void*)(uintptr_t)this->effectiveWinId();
        bgfxInit.platformData.context = nullptr;
        bgfxInit.platformData.backBuffer = nullptr;
        bgfxInit.platformData.backBufferDS = nullptr;
        //    bgfxInit.platformData.session = nullptr;
        //    bgfx::setPlatformData(pd);
        bgfxInit.type = bgfx::RendererType::Count; // Automatically choose renderer
        //    bgfxInit.type = bgfx::RendererType::Direct3D9;
        //            bgfxInit.type = bgfx::RendererType::Direct3D11;
        //            bgfxInit.type = bgfx::RendererType::Direct3D12;
        //            bgfxInit.type = bgfx::RendererType::OpenGL;
        //    bgfxInit.type = bgfx::RendererType::OpenGLES;
        //            bgfxInit.type = bgfx::RendererType::Vulkan; // no swap chain
        //    bgfxInit.type = bgfx::RendererType::Metal;

        bgfxInit.resolution.width = width;
        bgfxInit.resolution.height = height;
        //    bgfxInit.resolution.reset = BGFX_RESET_NONE;
        bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
        bgfxInit.vendorId = BGFX_PCI_ID_NONE;
        if (!bgfx::init(bgfxInit)) {
            assert(false);
        }

        //         g_scene.addModel("/home/gauthier/Downloads/Cougar2/cougar.obj");

        const bgfx::Caps* caps = bgfx::getCaps();

        entry::g_renderer = bgfx::getRendererName(caps->rendererType);
        //        Q_ASSERT(0 != (caps->supported & BGFX_CAPS_SWAP_CHAIN)); // vulkan no swap chain

        switch (caps->vendorId) {
        case BGFX_PCI_ID_AMD:
            entry::g_vendorID = "AMD";
            break;
        case BGFX_PCI_ID_INTEL:
            entry::g_vendorID = "INTEL";
            break;
        case BGFX_PCI_ID_NVIDIA:
            entry::g_vendorID = "NVIDIA";
            break;

        default:
            std::cout << "[WindowState] unknown vendor graphic id" << std::endl;
            exit(1);
            break;
        }

        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFF0000FF, 1.0f, 0);
        bgfx::touch(0);

        // ------------------------------- LOAD MODEL
        entry::s_scene.addModel(std::string(PROJECT_DIR) + "assets/sponza/sponza.obj");
        //        entry::s_scene.addModel("/home/gauthier/Downloads/Cougar2/cougar.obj");

        entry::s_scene.addLight(SpotLight({0.0f, -1.0f, 0.5f}));

        //                 entry::s_scene.addModel("C:\\Users\\gauthier.bouyjou\\Downloads\\teapot.obj");
        //            entry::s_scene.addModel("C:\\Users\\gauthier.bouyjou\\Downloads\\export\\cougar.obj");
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
        Program::init(caps);
        //        s_program.setShading(Program::Shading(g_iViewportShading));
        //        g_viewportShading = Program::shadingFileNames[g_iViewportShading];
        //        m_isInit = true;
        //        Q_ASSERT(entry::s_windows.size() == 0);
        //        Q_ASSERT(m_iWindow == 0);
    }
    //    m_fbh = bgfx::createFrameBuffer((void*)(uintptr_t)winId(), uint16_t(width()), uint16_t(height()));
    //    m_iWindow = entry::s_windows.size();
    //    m_windows[m_iWindow].m_fbh = bgfx::createFrameBuffer((void*)(uintptr_t)winId(), uint16_t(width()), uint16_t(height()));
    //    winId();
    //    WindowState wstate(nwh, 20, 40);

    //        entry::s_cameras.emplace_back(std::make_unique<CameraFps>(bx::Vec3 { -7.0f, 1.0f, 0.0f })); // question : push_back ?
    //    std::cout << &entry::s_cameras << std::endl;

    //    auto& window = entry::s_windows[m_id];
    if (m_id != 0) {
        m_fbh = bgfx::createFrameBuffer((void*)(uintptr_t)m_nwh, uint16_t(m_width), uint16_t(m_height));
    }

    //    m_windows.emplace_back(nwh, width(), height());
    //    bgfx::setViewFrameBuffer(m_iWidget, m_fbh);

    bx::mtxIdentity(entry::g_mtx);

    //    setFocus();

    //    m_lastTime = std::chrono::high_resolution_clock::now();
    // m_lastTime = std::chrono::steady_clock::now();
    //    m_iWidget = g_nWidget;
    //    bgfx::setViewFrameBuffer(m_iWidget, m_fbh);
    //    m_iWidget = 1;
    //    ++g_nWidget;

    //    init();
    s_windows.emplace_back(this);
    m_lastTime = std::chrono::high_resolution_clock::now();
}

WindowState::~WindowState()
{

    if (m_id == 0) {
        entry::s_scene.clear();
        //        s_program.clear();
        Program::clear();
        //        for (auto & window : s_windows) {
        //            window.clear();
        //        }

        bgfx::shutdown();
    } else {
        assert(bgfx::isValid(m_fbh));
        bgfx::destroy(m_fbh);
        m_fbh.idx = bgfx::kInvalidHandle;
    }

    s_windows.remove(this);
}

//WindowState::~WindowState()
//{
////    bgfx::destroy(m_fbh);

//}

//void WindowState::clear()
//{
//    bgfx::destroy(m_fbh);
//}

void WindowState::updateCameraPos()
{
    //    auto& window = entry::s_windows[m_iWindow];

    float cameraSpeed = m_slowMotion ? 1.0f : 5.0f;
    cameraSpeed *= m_deltaTime;

    auto& camera = *entry::s_cameras[m_iCamera];
    if (m_cameraMoveFront) {
        camera.move(Camera::Direction::FRONT, cameraSpeed * m_cameraMoveFront);
    }
    if (m_cameraMoveUp) {
        camera.move(Camera::Direction::UP, cameraSpeed * m_cameraMoveUp);
    }
    if (m_cameraMoveRight) {
        camera.move(Camera::Direction::RIGHT, cameraSpeed * m_cameraMoveRight);
    }
}

void WindowState::resetWindow()
{
    assert(m_id == 0);
    bgfx::reset(m_width, m_height, entry::getResetFlags());
}

void WindowState::render()
{
    //    if (! entry::s_bgfxInit)
    //        return;

    // return;
    //   if (m_isInit == false) {
    ////       init();
    //        return;
    //   }
    //    bgfx::setViewRect(1, 0, 0, uint16_t(width()), uint16_t(height()));
    //    // This dummy draw call is here to make sure that view 0 is cleared
    //    // if no other draw calls are submitted to view 0.
    //    bgfx::setViewClear(1, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0X00FF00FF);
    //    bgfx::touch(1);
    //    bgfx::frame();

    //        return;
    //    render();
    //    qDebug() << "QWidgetBgfx::paintEvent(" << event << ")";
    //    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFF0000FF, 1.0f, 0);

    if (m_id == 0) {
        printDebugMessage();
        //        entry::printDebugMessage();
    }

    //    const std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
    //    const auto currentTime = std::chrono::system_clock::now();
    const auto currentTime = std::chrono::high_resolution_clock::now();

    //    const auto currentTime = g_timer.now();
    m_deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - m_lastTime).count() / 1000000.0;
    m_lastTime = currentTime;
    m_sum += m_deltaTime;

    //    qDebug() << g_epoch << m_sum << g_fps;
    if (m_counter >= m_epoch) {
        m_epoch = (m_fps = m_epoch / m_sum) / 2; // update g_fps each 0.5 sec
        //        m_epoch = (m_fps = m_epoch / m_sum); // update g_fps each sec
        m_sum = 0.0f;
        m_counter = 0;
    }
    ++m_counter;

    //    updateCameraPos();
    updateCameraPos();

    // --------------------------------- SET CAMERA VIEW
    //    float view[16];
    //    // bx::mtxLookAt(view, eye, at);
    //    bx::mtxLookAt(view, g_cameraFps.m_pos, bx::add(g_cameraFps.m_pos, g_cameraFps.m_front), g_cameraFps.m_up);

    //    float proj[16];
    //    bx::mtxProj(proj, g_cameraFps.m_fov, float(width()) / float(height()), 0.1f, 100.0f,
    //        bgfx::getCaps()->homogeneousDepth);

    //    bgfx::setViewTransform(m_iWidget, view, proj);
    //    bgfx::setViewT
    //    bgfx::setViewRect(0, 0, 0, uint16_t(width()), uint16_t(height()));
    //    bgfx::touch(0);

    if (m_id != 0) {
        assert(bgfx::isValid(m_fbh));
        bgfx::setViewFrameBuffer(m_id, m_fbh);
    }
    // Set view 0 default viewport.
    bgfx::setViewRect(m_id, 0, 0, uint16_t(m_width), uint16_t(m_height));
    // This dummy draw call is here to make sure that view 0 is cleared
    // if no other draw calls are submitted to view 0.
    bgfx::setViewClear(m_id, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0X555555FF);
    //    bgfx::touch(m_id);

    // --------------------------------- DRAW SCENE
    //    if (m_id == 0) {
    const uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS
        | BGFX_STATE_CULL_CCW | BGFX_STATE_BLEND_NORMAL;
    const float ratio = float(m_width) / m_height;

    assert(0 <= m_iCamera && m_iCamera < entry::s_cameras.size());
    const auto& camera = *entry::s_cameras[m_iCamera];
    entry::s_scene.draw(m_id, m_shading, entry::g_mtx, state, camera, ratio);
    //    g_scene.draw(1, g_program, g_mtx, state, g_cameraPos);

    // Advance to next frame. Process submitted rendering primitives.
    if (m_id == 0) { // avoid flipping, put F1 to show (only with direct3D)
        bgfx::frame();
    }
    //    return;
}

void WindowState::printDebugMessage()
{
    if (entry::g_showStats) {
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
        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Renderer: %s", entry::g_renderer.c_str());
        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Graphic Vendor: %s Corporation", entry::g_vendorID.c_str());
        //            bgfx::dbgTextPrintf(0, ++line, 0x0F, "Fps:%.1f", g_fps);
        //        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Fps: %.2f, Backbuffer: %dx%d, Viewport shading: %s", g_fps, stats->width, stats->height, g_viewportShading.c_str());
        //        for (int i = 0; i < s_windows.size(); ++i) {
        //            //        for (const auto& window : m_windows) {
        //            const auto& window = *s_windows[i];
        //            bgfx::dbgTextPrintf(0, ++line, 0x0F, "Window: %d, Fps: %.2f, Backbuffer: %dx%d, Viewport shading: %s", i, window.m_fps, window.m_width, window.m_height, Program::filename(window.m_shading));
        //        }
        int i = 0;
        for (const auto& window : s_windows) {
            bgfx::dbgTextPrintf(0, ++line, 0x0F, "Window: %d, Fps: %.2f, Backbuffer: %dx%d, Viewport shading: %s", i, window->m_fps, window->m_width, window->m_height, Program::filename(window->m_shading));
            ++i;
        }
        entry::s_scene.printStats(line);
    }
}

// ------------------------------------------------ EVENT
void WindowState::mouseMoveEvent(int x, int y)
{
    //    auto& window = entry::s_windows[m_iWindow];
    //        qDebug() << "QWidgetBgfx::mouseMoveEvent(" << event << ")";
    if (m_mouseLeftClicked) {
        //        int xpos = event->x();
        //        int ypos = event->y();
        //        int xpos = x;
        //        int ypos = y;
        if (m_firstMouse) {
            m_lastX = x;
            m_lastY = y;
            m_firstMouse = false;
        }

        // float xoffset = xpos - m_lastX;
        float xoffset = m_lastX - x;
        float yoffset = m_lastY - y; // reversed since y-coordinates go from bottom to top
        m_lastX = x;
        m_lastY = y;

        float sensitivity = 0.5f; // change this value to your liking
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        auto& camera = *entry::s_cameras[m_iCamera];
        camera.mouseMove(xoffset, yoffset);
        //        camera.rotate(xoffset, yoffset);
    }
}

void WindowState::mouseReleaseEvent(MouseButton::Enum mouseButton)
{
    switch (mouseButton) {
    case MouseButton::Left:
        m_mouseLeftClicked = false;
        break;

        //    case MouseButton::NONE:
        //    default:
        //        //        assert(mouseButton != MouseButton::NONE);
        //        std::cout << "[WindowState] unknown key " << mouseButton << std::endl;
        //        break;

        //    default:
        //        assert(false);
        //        break;
    }
}

void WindowState::mousePressEvent(MouseButton::Enum mouseButton)
{
    switch (mouseButton) {
    case MouseButton::Left:
        m_mouseLeftClicked = true;
        m_firstMouse = true;
        break;

        //    case MouseButton::NONE:
        //    default:
        //        //        assert(mouseButton != MouseButton::NONE);
        //        std::cout << "[WindowState] unknown key " << mouseButton << std::endl;
        //        break;

        //    default:
        //        assert(false);
        //        break;
    }
}

void WindowState::keyReleaseEvent(Key::Enum key)
{
    //    assert(key != Key::None);
    switch (key) {
    case Key::Up:
    case Key::Down:
        m_cameraMoveFront = 0;
        break;
    case Key::Right:
    case Key::Left:
        m_cameraMoveRight = 0;
        break;
    case Key::PageUp:
    case Key::PageDown:
        m_cameraMoveUp = 0;
        break;

    case Key::Control:
        m_ctrlPressed = false;
        break;

    case Key::NONE:
    default:
        //        assert(key != Key::NONE);
        //        std::cout << "[WindowState] unknown key " << key << std::endl;
        break;
        //    default:
        //        QWidget::keyReleaseEvent(event);
        //        break;
        //    default:
        //        assert(false);
        //        break;
    }
}

void WindowState::keyPressEvent(Key::Enum key)
{

    //    auto& shading = window.m_shading;

    //    qDebug() << "QWidgetBgfx::keyPressEvent(" << event << ")";
    switch (key) {
    case Key::F1:
        entry::g_showStats = !entry::g_showStats;
        break;
    case Key::F2:
        entry::g_vsyncEnable = !entry::g_vsyncEnable;
        //        for (int i = 0; i < s_windows.size(); ++i) {
        //            s_windows[i]->m_epoch = 10;
        //        }
        for (auto& window : s_windows) {
            window->m_epoch = 10;
        }
        //        window.m_epoch = 10;
        resetWindow();
        break;
    case Key::F3:
        entry::g_mssaLevel = ++entry::g_mssaLevel % 5;
        resetWindow();
        break;
    case Key::F4:
        entry::g_textureSamplerFlags = ++entry::g_textureSamplerFlags % 5;
        switch (entry::g_textureSamplerFlags) {
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
    case Key::F5:
        ++m_shading;
        //        shading = ++shading % Program::Shading::Count;
        break;
    case Key::Up:
        m_cameraMoveFront = 1;
        break;
    case Key::Down:
        m_cameraMoveFront = -1;
        break;
    case Key::Right:
        m_cameraMoveRight = 1;
        break;
    case Key::Left:
        m_cameraMoveRight = -1;
        break;
    case Key::PageUp:
        m_cameraMoveUp = 1;
        break;
    case Key::PageDown:
        m_cameraMoveUp = -1;
        break;
    case Key::N:
        m_shading = Shading::NORMAL;
        break;
    case Key::M:
        m_shading = Shading::MATERIAL;
        break;
    case Key::R:
        m_shading = Shading::RENDERED;
        break;
    case Key::E:
        m_shading = Shading::EMISSIVE;
        break;

        //    case Key::Q:
        //        if (m_ctrlPressed) {
        //        }
        break;
    case Key::Control:
        m_ctrlPressed = true;
        m_slowMotion = !m_slowMotion;
        break;

        //    case Key::NONE:
        //    default:
        //        std::cout << "[WindowState] unknown key " << key << std::endl;
        //        //        assert(key != Key::NONE);
        //        //        break;
        //        //    default:
        //        //        QWidget::keyPressEvent(event);
        //        break;

        //    default:
        //        assert(false);
        //        break;
    }
}

void WindowState::resizeEvent(int width, int height)
{
    //    g_width = size.width();
    m_width = width;
    //    g_height = size.height();
    m_height = height;

    //    m_fbh = bgfx::createFrameBuffer((void*)(uintptr_t)winId(), uint16_t(width()), uint16_t(height()));

    if (m_id != 0) {
        assert(bgfx::isValid(m_fbh));
        bgfx::destroy(m_fbh);
        m_fbh.idx = bgfx::kInvalidHandle;
        m_fbh = bgfx::createFrameBuffer(m_nwh, uint16_t(m_width), uint16_t(m_height));
    } else {
        resetWindow();
    }
}

void WindowState::mouseScrollEvent(int offset)
{
    auto& camera = *entry::s_cameras[m_iCamera];
    camera.zoom(offset);
}
