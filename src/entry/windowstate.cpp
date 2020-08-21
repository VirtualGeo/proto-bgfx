#include "windowstate.h"

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <cassert>
//#include <engine/camerafps.h>
#include <engine/geometry.h>
#include <entry/entry.h>
#include <list>
#include <engine/dirLight.h>

#include <cstring>

//#include <utility.h>
//#include <thread>

std::list<WindowState*> s_windows;

WindowState::WindowState(void* nwh, void* ndt, int width, int height, void* context,
    void* backBuffer, void* backBufferDS)
    : m_nwh(nwh)
    , m_ndt(ndt)
    , m_width(width)
    , m_height(height)
    , m_context(context)
    //    , m_id(s_windows.size())
    , m_backBuffer(backBuffer)
    , m_backBufferDS(backBufferDS)
//    , m_view({entry::s_scene.m_cameras.size(), float(m_width) / m_height, s_windows.size(), Shading::MATERIAL}) // question: how to do that
//    , m_iCamera(entry::s_scene.m_cameras.size())
//    const float ratio = float(m_width) / m_height;
{
    m_view.id = VIEW_ID_START_WINDOW + s_windows.size();

    //    m_view.shading = Shading::MATERIAL;

//    m_offScreenFBH.idx = bgfx::kInvalidHandle;
    m_offScreenFBH = BGFX_INVALID_HANDLE; // default fbh created by bgfx init
    assert(!bgfx::isValid(m_offScreenFBH));

    // first created view call bgfx::init()
    if (m_view.id == VIEW_ID_START_WINDOW) { // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
        // Most graphics APIs must be used on the same thread that created the window.
        bgfx::renderFrame();
        //        std::cout << "[THREAD] bgfx init: " << std::this_thread::get_id() << std::endl;

        bgfx::Init bgfxInit = {};
        bgfxInit.platformData.ndt = const_cast<void*>(m_ndt);
        bgfxInit.platformData.nwh = const_cast<void*>(m_nwh);
        //        bgfxInit.platformData.nwh = nullptr;
        bgfxInit.platformData.context = const_cast<void*>(m_context);
        bgfxInit.platformData.backBuffer = const_cast<void*>(m_backBuffer);
        bgfxInit.platformData.backBufferDS = const_cast<void*>(m_backBufferDS);
        //    bgfxInit.platformData.session = nullptr;
        //    bgfx::setPlatformData(pd);
        bgfxInit.type = bgfx::RendererType::Count; // Automatically choose renderer
        //                bgfxInit.type = bgfx::RendererType::Direct3D9;
//                        bgfxInit.type = bgfx::RendererType::Direct3D11;
//                        bgfxInit.type = bgfx::RendererType::Direct3D12;
        bgfxInit.type = bgfx::RendererType::OpenGL;
        //        bgfxInit.type = bgfx::RendererType::OpenGLES;
//                        bgfxInit.type = bgfx::RendererType::Vulkan; // no swap chain
        //        bgfxInit.type = bgfx::RendererType::Metal;

        bgfxInit.resolution.width = width;
        bgfxInit.resolution.height = height;
        //        bgfxInit.resolution.reset = BGFX_RESET_NONE;
        //                bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
        bgfxInit.vendorId = BGFX_PCI_ID_NONE;
        //        bgfxInit.deviceId;
        if (!bgfx::init(bgfxInit)) {
            assert(false);
        }

        //        const bgfx::InternalData* internalData = bgfx::getInternalData();
        //        std::cout << "[WindowState] bgfx init: " << internalData->context << std::endl;
        //        if (m_context != nullptr) {
        //        assert(m_context == nullptr || bgfx::getInternalData()->context == m_context);
        //        assert(m_context == nullptr || bgfx::getInternalData()->context == nullptr);
        //        }

        const bgfx::Caps* caps = bgfx::getCaps();
        assert(caps->supported & BGFX_CAPS_TEXTURE_COMPARE_LEQUAL); // sampler supported
        //        assert(caps->homogeneousDepth);

        entry::s_renderer = bgfx::getRendererName(caps->rendererType);
        //        Q_ASSERT(0 != (caps->supported & BGFX_CAPS_SWAP_CHAIN)); // vulkan no swap chain

        switch (caps->vendorId) {
        case BGFX_PCI_ID_AMD:
            entry::s_vendorID = "AMD";
            break;
        case BGFX_PCI_ID_INTEL:
            entry::s_vendorID = "INTEL";
            break;
        case BGFX_PCI_ID_NVIDIA:
            entry::s_vendorID = "NVIDIA";
            break;

        default:
            std::cout << "[WindowState] unknown vendor graphic id" << std::endl;
            exit(1);
            break;
        }

        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x00FF00FF, 1.0f, 0);
        bgfx::touch(0);

        //        bgfx::frame();

        //        Q_ASSERT(m_iWindow == 0);
        bx::mtxIdentity(entry::s_worldTransform);

        entry::s_bgfxInitialized = true;

    } else {
        m_offScreenFBH = bgfx::createFrameBuffer((void*)(uintptr_t)m_nwh, uint16_t(m_width), uint16_t(m_height));
    }

    entry::init(m_view);
    m_view.iCamera = entry::s_scene.m_cameras.size() - 1;
    m_view.ratio = float(m_width) / m_height;

//    s_windows.emplace_back(this); // question
    s_windows.push_back(this);
    entry::s_lastTime = std::chrono::high_resolution_clock::now();
    //    m_init = true;
}

WindowState::~WindowState()
{
    // if last window, program should be done
    if (m_view.id == VIEW_ID_START_WINDOW) {
        entry::s_scene.clear();
        //        s_program.clear();
        //        Program::clear();
        //        Geometry::clear();
        //        Texture::clear();
        //        for (auto & window : s_windows) {
        //            window.clear();
        //        }
        entry::shutdown();

        bgfx::shutdown();

    } else {
        assert(bgfx::isValid(m_offScreenFBH));
        bgfx::destroy(m_offScreenFBH);
        m_offScreenFBH.idx = bgfx::kInvalidHandle;
    }

    s_windows.remove(this);
}

void WindowState::render() const
{
    // Passing `BGFX_INVALID_HANDLE` as frame buffer handle
    // will draw primitives from this view into default back buffer.
    bgfx::setViewFrameBuffer(m_view.id, m_offScreenFBH);


    bgfx::setViewRect(m_view.id, 0, 0, uint16_t(m_width), uint16_t(m_height));

    // This dummy draw call is here to make sure that view 0 is cleared
    // if no other draw calls are submitted to view 0.
    //    bgfx::setViewClear(m_id, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0X555555FF);

    bgfx::setViewClear(m_view.id, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0X555555FF);
    bgfx::touch(m_view.id);

    //    const float ratio = float(m_width) / m_height;
    entry::render(m_view);
    //    entry::s_scene.renderFromCamera(m_iCamera, ratio, m_id, m_shading, entry::g_mtx);
    return;
}

uintptr_t WindowState::renderAllWindow()
{
    //    assert(m_init);
    assert(entry::s_bgfxInitialized);

    const auto currentTime = std::chrono::high_resolution_clock::now();
    //    //    s_currentTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime).count();
    //    //    s_currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count() / 1000000.0;

    entry::s_deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - entry::s_lastTime).count() / 1000000.0;
    entry::s_lastTime = currentTime;

    entry::s_sum += entry::s_deltaTime;
    if (entry::s_counter >= entry::s_epoch) {
        //        entry::s_epoch = (entry::s_fps = entry::s_epoch / entry::s_sum) / 2; // update g_fps each 0.5 sec
        entry::s_epoch = (entry::s_fps = entry::s_epoch / entry::s_sum);
        //        entry::s_epoch = (entry::s_fps = entry::s_epoch / entry::s_sum) * 2;
        entry::s_sum = 0.0;
        entry::s_counter = 0;
    }
    ++entry::s_counter;
    printDebugMessage();

    for (WindowState* window : s_windows) {
        window->updateCameraPos();
    }

    entry::preRender();

    for (const WindowState* window : s_windows) {
        window->render();
        //        entry::render(window->m_view);
    }

    // Advance to next frame. Process submitted rendering primitives.
    bgfx::frame();
    //    usleep(10000);
    ++entry::s_iFrame;
    //    return bgfx::getInternal(m_offScreenFBH);
    return 0;
}

void WindowState::printDebugMessage()
{
    //    bgfx::setDebug(BGFX_DEBUG_NONE);
    //    return;
    if (entry::s_showStats) {
        bgfx::setDebug(BGFX_DEBUG_STATS);
    } else {
//                bgfx::setDebug(BGFX_DEBUG_NONE);
//                return;
        bgfx::setDebug(BGFX_DEBUG_TEXT);
        //        if (m_iWidget == 0) {
        bgfx::dbgTextClear();
        //        if (m_iWig)
        //        const bgfx::Stats* stats = bgfx::getStats();
        int line = -1;
        //        line = 20;
        bgfx::dbgTextPrintf(0, ++line, 0x2F, "F1:Stats | F2:Vsync | F3:Msaa | F4:Sampler | F5:Shading | F6:Debug | F7:");
        line = 3;
        //            bgfx::dbgTextPrintf(0, ++line, 0x0F, "Viewport shading: %s", g_viewportShading.c_str());
        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Arch: " BX_COMPILER_NAME " / " BX_CPU_NAME " / " BX_ARCH_NAME " / " BX_PLATFORM_NAME " ");
        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Renderer: %s", entry::s_renderer.c_str());
        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Graphic Vendor: %s Corporation", entry::s_vendorID.c_str());
        //                    bgfx::dbgTextPrintf(0, ++line, 0x0F, "Fps:%.1f", g_fps);
        //        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Fps: %.2f, Backbuffer: %dx%d, Viewport shading: %s", g_fps, stats->width, stats->height, g_viewportShading.c_str());
        //        for (int i = 0; i < s_windows.size(); ++i) {
        //            //        for (const auto& window : m_windows) {
        //            const auto& window = *s_windows[i];
        //            bgfx::dbgTextPrintf(0, ++line, 0x0F, "Window: %d, Fps: %.2f, Backbuffer: %dx%d, Viewport shading: %s", i, window.m_fps, window.m_width, window.m_height, Program::filename(window.m_shading));
        //        }

        //        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Fps: %.2f", entry::s_fps);
        int i = 0;
        for (const auto& window : s_windows) {
            //            bgfx::dbgTextPrintf(0, ++line, 0x0F, "Window: %d, Fps: %.2f, Backbuffer: %dx%d, Viewport shading: %s", i, window->m_fps, window->m_width, window->m_height, Program::filename(window->m_shading));
            bgfx::dbgTextPrintf(0, ++line, 0x0F, "Window: %d, Backbuffer: %dx%d, Viewport shading: %s", i, window->m_width, window->m_height, Program::filename(window->m_view.shading));
            ++i;
        }
        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Fps: %.2f", entry::s_fps);
        //        entry::s_scene.printStats(line);
    }
}

void WindowState::updateCameraPos()
{
    if (m_view.iCamera == -1)
        return;

    //    auto& window = entry::s_windows[m_iWindow];

    float cameraSpeed = m_slowMotion ? 1.0f : 5.0f;
    cameraSpeed *= entry::s_deltaTime;

    //    auto& camera = *entry::s_scene.m_cameras[m_iCamera];
    auto& camera = entry::s_scene.m_cameras[m_view.iCamera];
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

//void WindowState::resizeOffscreenFB()
//{
//    if (bgfx::isValid(m_offscreenFB)) {
//        bgfx::destroy(m_offscreenFB);
//        m_offscreenFB = BGFX_INVALID_HANDLE;
//    }
//    if (bgfx::isValid(m_offScreenFBH)) {
//        bgfx::destroy(m_offScreenFBH);
//        m_offScreenFBH = BGFX_INVALID_HANDLE;
//    }
//    if (bgfx::isValid(m_depthBuffer)) {
//        bgfx::destroy(m_depthBuffer);
//        m_depthBuffer = BGFX_INVALID_HANDLE;
//    }

//    m_offScreenFBH = bgfx::createTexture2D(m_width, m_height, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_RT, NULL);
//    m_depthBuffer = bgfx::createTexture2D(m_width, m_height, false, 1, bgfx::TextureFormat::D24S8, BGFX_TEXTURE_RT, NULL);
//    bgfx::TextureHandle fbtextures[2] = { m_offScreenFBH, m_depthBuffer };
//    m_offscreenFB = bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, false);
//}

void WindowState::resetWindow()
{
    //    assert(m_view.id == VIEW_ID_START_WINDOW);
    bgfx::reset(m_width, m_height, entry::getResetFlags());
}

// ------------------------------------------------ QT EVENT
void WindowState::resizeEvent(int width, int height)
{
    //    const bgfx::InternalData* internalData = bgfx::getInternalData();
    //    std::cout << "[WindowState] bgfx resizeEvent: " << internalData->context << std::endl;

    //    g_width = size.width();
    m_width = width;
    //    g_height = size.height();
    m_height = height;

    //    m_offScreenFBH = bgfx::createFrameBuffer((void*)(uintptr_t)winId(), uint16_t(width()), uint16_t(height()));

    if (m_view.id != VIEW_ID_START_WINDOW) {
        assert(bgfx::isValid(m_offScreenFBH));
        bgfx::destroy(m_offScreenFBH);
        m_offScreenFBH.idx = bgfx::kInvalidHandle;
        assert(m_nwh != nullptr);
        m_offScreenFBH = bgfx::createFrameBuffer(const_cast<void*>(m_nwh), uint16_t(m_width), uint16_t(m_height));
    } else {
        resetWindow();
    }

    //    resizeOffscreenFB();

    const float ratio = float(m_width) / m_height;
    m_view.ratio = ratio;

    //        bgfx::setViewFrameBuffer(m_view.id, m_offScreenFBH);
    //        bgfx::setViewRect(m_view.id, 0, 0, uint16_t(m_width), uint16_t(m_height));

    //        // This dummy draw call is here to make sure that view 0 is cleared
    //        // if no other draw calls are submitted to view 0.
    //        //    bgfx::setViewClear(m_id, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0X555555FF);

    //        bgfx::setViewClear(m_view.id, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0X555555FF);
    //        bgfx::touch(m_view.id);
    bgfx::frame(); // avoid startup artefact, with long scene charging
}

void WindowState::mouseMoveEvent(int x, int y)
{
    if (m_view.iCamera == -1)
        return;

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

        //        auto& camera = *entry::s_scene.m_cameras[m_view.iCamera];
        if (m_shiftPressed) {
            auto & light = entry::s_scene.m_dirLights[0];
            light.mouseMove(xoffset, yoffset);


        } else {
            auto& camera = entry::s_scene.m_cameras[m_view.iCamera];
            //        auto& camera = entry::s_scene.m_cameras.at(m_view.iCamera);
            camera.mouseMove(xoffset, yoffset);
        }
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

    case Key::Shift:
        m_shiftPressed = false;
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
        entry::s_showStats = !entry::s_showStats;
        break;
    case Key::F2:
        entry::s_vsyncEnable = !entry::s_vsyncEnable;
        //        for (int i = 0; i < s_windows.size(); ++i) {
        //            s_windows[i]->m_epoch = 10;
        //        }
        //        for (auto& window : s_windows) {
        //            window->m_epoch = 10;
        //        }
        entry::s_epoch = 10;
        //        window.m_epoch = 10;
        resetWindow();
        break;
    case Key::F3:
        if (m_shiftPressed) {
            entry::s_mssaLevel = (entry::s_mssaLevel + 4) % 5;
        }
        else {
            entry::s_mssaLevel = ++entry::s_mssaLevel % 5;
        }
        resetWindow();
        break;
    case Key::F4:
        entry::s_textureSamplerFlags = ++entry::s_textureSamplerFlags % 5;
        switch (entry::s_textureSamplerFlags) {
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
        ++m_view.shading;
        //                shading = ++shading % Program::Shading::Count;
        break;
    case Key::F6:
        entry::s_scene.m_drawDebug = !entry::s_scene.m_drawDebug;
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
        m_view.shading = Shading::NORMAL;
        break;
    case Key::M:
        m_view.shading = Shading::MATERIAL;
        break;
    case Key::R:
        m_view.shading = Shading::RENDERED;
        break;
    case Key::E:
        m_view.shading = Shading::EMISSIVE;
        break;

        //    case Key::Q:
        //        if (m_ctrlPressed) {
        //        }
        break;
    case Key::Control:
        m_ctrlPressed = true;
        m_slowMotion = !m_slowMotion;
        break;

    case Key::Shift:
        m_shiftPressed = true;
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

void WindowState::mouseScrollEvent(int offset)
{
    if (m_view.iCamera == -1)
        return;

    //    auto& camera = *entry::s_scene.m_cameras[m_view.iCamera];
    auto& camera = entry::s_scene.m_cameras[m_view.iCamera];
    camera.zoom(offset);
}
