#include "windowstate.h"
#include "engine/geometry.h"
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <cassert>
#include <engine/camerafps.h>
#include <engine/geometry.h>
#include <entry/entry.h>
#include <list>

#include <algorithm>
#include <cstring>
#include <random>

#include <bgfx/bgfx.h>
#include <utility.h>

#include <thread>

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
    m_view.iCamera = entry::s_scene.m_cameras.size();
    m_view.ratio = float(m_width) / m_height;
    m_view.id = s_windows.size();
//    m_view.shading = Shading::MATERIAL;

    m_fbh.idx = bgfx::kInvalidHandle;
    assert(!bgfx::isValid(m_fbh));

    if (m_view.id == 0) {
        // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
        // Most graphics APIs must be used on the same thread that created the window.
        bgfx::renderFrame();
        std::cout << "[THREAD] bgfx init: " << std::this_thread::get_id() << std::endl;

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
        //        bgfxInit.type = bgfx::RendererType::Direct3D9;
        //        bgfxInit.type = bgfx::RendererType::Direct3D11;
        //        bgfxInit.type = bgfx::RendererType::Direct3D12;
        bgfxInit.type = bgfx::RendererType::OpenGL;
        //        bgfxInit.type = bgfx::RendererType::OpenGLES;
        //        bgfxInit.type = bgfx::RendererType::Vulkan; // no swap chain
        //        bgfxInit.type = bgfx::RendererType::Metal;

        bgfxInit.resolution.width = width;
        bgfxInit.resolution.height = height;
        bgfxInit.resolution.reset = BGFX_RESET_NONE;
        //        bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
        bgfxInit.vendorId = BGFX_PCI_ID_NONE;
        //        bgfxInit.deviceId;
        if (!bgfx::init(bgfxInit)) {
            assert(false);
        }

        const bgfx::InternalData* internalData = bgfx::getInternalData();
        std::cout << "[CONTEXT] bgfx init: " << internalData->context << std::endl;
        if (m_context != nullptr) {
            assert(internalData->context == m_context);
        }

        const bgfx::Caps* caps = bgfx::getCaps();
        assert(caps->supported & BGFX_CAPS_TEXTURE_COMPARE_LEQUAL); // sampler supported
        //        assert(caps->homogeneousDepth);

        entryFake::g_renderer = bgfx::getRendererName(caps->rendererType);
        //        Q_ASSERT(0 != (caps->supported & BGFX_CAPS_SWAP_CHAIN)); // vulkan no swap chain

        switch (caps->vendorId) {
        case BGFX_PCI_ID_AMD:
            entryFake::g_vendorID = "AMD";
            break;
        case BGFX_PCI_ID_INTEL:
            entryFake::g_vendorID = "INTEL";
            break;
        case BGFX_PCI_ID_NVIDIA:
            entryFake::g_vendorID = "NVIDIA";
            break;

        default:
            std::cout << "[WindowState] unknown vendor graphic id" << std::endl;
            exit(1);
            break;
        }

        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFF0000FF, 1.0f, 0);
        bgfx::touch(0);

        Program::init(caps);
        Geometry::init();
        Texture::init();
//        bgfx::frame();

        //        Q_ASSERT(m_iWindow == 0);
        bx::mtxIdentity(entry::s_worldTransform);
//        entry::s_scene.m_cameras.emplace_back(bx::Vec3 { -5.0f, 1.0f, -0.5f }); // question : push_back ?
//                entry::s_scene.m_cameras.emplace_back(bx::Vec3 { 0.0f, 0.0f, 200.0f }); // question : push_back ?
        //        entry::s_scene.m_cameras.emplace_back(bx::Vec3 { 0.0f, 0.0f, 3.0f }); // question : push_back ?
        entry::s_lastTime = std::chrono::high_resolution_clock::now();

        //                const uint32_t nbCube = 2;
        //                const uint32_t nbCube = 16;
        //                        const uint32_t nbCube = 32;
        //                const uint32_t nbCube = 64;
        //        initCubeScene();

        entry::init(m_view);

    } else {
//        entry::s_scene.m_cameras.emplace_back(bx::Vec3 { 5.0, 1.0f, -1.0f }); // question : push_back ?
        m_fbh = bgfx::createFrameBuffer((void*)(uintptr_t)m_nwh, uint16_t(m_width), uint16_t(m_height));
        entry::init(m_view);
    }

    s_windows.emplace_back(this);
    m_init = true;
}

WindowState::~WindowState()
{

    if (m_view.id == 0) {
        entry::s_scene.clear();
        //        s_program.clear();
        Program::clear();
        Geometry::clear();
        Texture::clear();
        //        for (auto & window : s_windows) {
        //            window.clear();
        //        }
        entry::shutdown();

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

void WindowState::render() const
{
    if (!m_init)
        return;

    if (m_view.id != 0) {
        //            assert(bgfx::isValid(m_fbh));
        bgfx::setViewFrameBuffer(m_view.id, m_fbh);
    }
    //    bgfx::PlatformData pdata;
    //    pdata.context = const_cast<void*>(m_context);
    //    bgfx::setPlatformData(pdata);
    //        bgfx::setViewFrameBuffer(m_id, m_fbh);
    //    bgfx::setViewFrameBuffer(0, BGFX_INVALID_HANDLE); // default back buffer
    // Set view 0 default viewport.
    bgfx::setViewRect(m_view.id, 0, 0, uint16_t(m_width), uint16_t(m_height));
    // This dummy draw call is here to make sure that view 0 is cleared
    // if no other draw calls are submitted to view 0.
    //    bgfx::setViewClear(m_id, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0X555555FF);
    bgfx::setViewClear(m_view.id, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0X555555FF);
    bgfx::touch(m_view.id);
    //            bgfx::touch(m_id);

    //    return;



//    const float ratio = float(m_width) / m_height;
    entry::update(m_view);
    //    entry::s_scene.renderFromCamera(m_iCamera, ratio, m_id, m_shading, entry::g_mtx);
    return;

//    const auto& camera = entry::s_scene.m_cameras[m_iCamera];
//    camera.setViewTransform(ratio, m_id);
}

uintptr_t WindowState::renderAllWindow()
{
    if (!m_init)
        return 0;

    const auto currentTime = std::chrono::high_resolution_clock::now();

    //    //    s_currentTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime).count();
    //    //    s_currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count() / 1000000.0;

    entry::s_deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - entry::s_lastTime).count() / 1000000.0;
    entry::s_lastTime = currentTime;
    entry::s_sum += entry::s_deltaTime;

    //    qDebug() << g_epoch << m_sum << g_fps;
    if (entry::s_counter >= entry::s_epoch) {
        //        entry::s_epoch = (s_fps = entry::s_epoch / s_sum) / 2; // update g_fps each 0.5 sec
        //        entry::s_epoch = (s_fps = entry::s_epoch / s_sum); // update g_fps each sec
        entry::s_epoch = (entry::s_fps = entry::s_epoch / entry::s_sum) * 2;
        entry::s_sum = 0.0f;
        entry::s_counter = 0;
    }
    ++entry::s_counter;
    printDebugMessage();

    //    updateCameraPos();
    for (WindowState* window : s_windows) {
        window->updateCameraPos();
    }

    //    if (m_id == 0) {

    //    entry::s_scene.setLightUniforms();
    //    entry::s_scene.updateLightShadowMaps();
    //        bgfx::frame();

    //        int iWindow = 0;
    //    bgfx::setViewFrameBuffer(0, m_offscreenFB);
    for (const WindowState* window : s_windows) {
        //        entry::s_scene.setLightShadowSamplers();
        window->render();
        //        ++iWindow;
    }
    //    bgfx::frame();
    //    bgfx::setViewClear(m_id, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0X00FF00FF);
    //    bgfx::setViewFrameBuffer(0, BGFX_INVALID_HANDLE);
    //    render();

    //        g_scene.draw(1, g_program, g_mtx, state, g_cameraPos);

    // Advance to next frame. Process submitted rendering primitives.
    //    if (m_id == 0) { // avoid flipping, put F1 to show (only with direct3D)
    bgfx::frame();
    //    usleep(10000);
    ++entry::s_iFrame;
    //        entry::s_scene.updateLightShadowMaps();
    //        entry::s_scene.setLightUniforms();
    //    }
    //    return bgfx::getInternal(m_backBuffer);
    return 0;
}

void WindowState::printDebugMessage()
{
    //    bgfx::setDebug(BGFX_DEBUG_NONE);
    //    return;
    if (entryFake::g_showStats) {
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
        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Renderer: %s", entryFake::g_renderer.c_str());
        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Graphic Vendor: %s Corporation", entryFake::g_vendorID.c_str());
        //            bgfx::dbgTextPrintf(0, ++line, 0x0F, "Fps:%.1f", g_fps);
        //        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Fps: %.2f, Backbuffer: %dx%d, Viewport shading: %s", g_fps, stats->width, stats->height, g_viewportShading.c_str());
        //        for (int i = 0; i < s_windows.size(); ++i) {
        //            //        for (const auto& window : m_windows) {
        //            const auto& window = *s_windows[i];
        //            bgfx::dbgTextPrintf(0, ++line, 0x0F, "Window: %d, Fps: %.2f, Backbuffer: %dx%d, Viewport shading: %s", i, window.m_fps, window.m_width, window.m_height, Program::filename(window.m_shading));
        //        }
        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Fps: %.2f", entry::s_fps);
        int i = 0;
        for (const auto& window : s_windows) {
            //            bgfx::dbgTextPrintf(0, ++line, 0x0F, "Window: %d, Fps: %.2f, Backbuffer: %dx%d, Viewport shading: %s", i, window->m_fps, window->m_width, window->m_height, Program::filename(window->m_shading));
            bgfx::dbgTextPrintf(0, ++line, 0x0F, "Window: %d, Backbuffer: %dx%d, Viewport shading: %s", i, window->m_width, window->m_height, Program::filename(window->m_view.shading));
            ++i;
        }
        entry::s_scene.printStats(line);
    }
}

void WindowState::updateCameraPos()
{
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

void WindowState::resetWindow()
{
    assert(m_view.id == 0);
    bgfx::reset(m_width, m_height, entryFake::getResetFlags());
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

        //        auto& camera = *entry::s_scene.m_cameras[m_view.iCamera];
        auto& camera = entry::s_scene.m_cameras[m_view.iCamera];
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
        entryFake::g_showStats = !entryFake::g_showStats;
        break;
    case Key::F2:
        entryFake::g_vsyncEnable = !entryFake::g_vsyncEnable;
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
        entryFake::g_mssaLevel = ++entryFake::g_mssaLevel % 5;
        resetWindow();
        break;
    case Key::F4:
        entryFake::g_textureSamplerFlags = ++entryFake::g_textureSamplerFlags % 5;
        switch (entryFake::g_textureSamplerFlags) {
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
    const bgfx::InternalData* internalData = bgfx::getInternalData();
    std::cout << "[CONTEXT] bgfx resizeEvent: " << internalData->context << std::endl;

    //    g_width = size.width();
    m_width = width;
    //    g_height = size.height();
    m_height = height;

    //    m_fbh = bgfx::createFrameBuffer((void*)(uintptr_t)winId(), uint16_t(width()), uint16_t(height()));

    if (m_view.id != 0) {
        assert(bgfx::isValid(m_fbh));
        bgfx::destroy(m_fbh);
        m_fbh.idx = bgfx::kInvalidHandle;
        assert(m_nwh != nullptr);
        m_fbh = bgfx::createFrameBuffer(const_cast<void*>(m_nwh), uint16_t(m_width), uint16_t(m_height));
    } else {
        resetWindow();
    }

    resizeOffscreenFB();

    const float ratio = float(m_width) / m_height;
    m_view.ratio = ratio;
}

void WindowState::resizeOffscreenFB()
{
//    if (bgfx::isValid(m_offscreenFB)) {
//        bgfx::destroy(m_offscreenFB);
//        m_offscreenFB = BGFX_INVALID_HANDLE;
//    }
//    if (bgfx::isValid(m_backBuffer)) {
//        bgfx::destroy(m_backBuffer);
//        m_backBuffer = BGFX_INVALID_HANDLE;
//    }
//    if (bgfx::isValid(m_depthBuffer)) {
//        bgfx::destroy(m_depthBuffer);
//        m_depthBuffer = BGFX_INVALID_HANDLE;
//    }

//    m_backBuffer = bgfx::createTexture2D(m_width, m_height, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_RT, NULL);
//    m_depthBuffer = bgfx::createTexture2D(m_width, m_height, false, 1, bgfx::TextureFormat::D24S8, BGFX_TEXTURE_RT, NULL);
//    bgfx::TextureHandle fbtextures[2] = { m_backBuffer, m_depthBuffer };
//    m_offscreenFB = bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, false);
}

void WindowState::mouseScrollEvent(int offset)
{
    //    auto& camera = *entry::s_scene.m_cameras[m_view.iCamera];
    auto& camera = entry::s_scene.m_cameras[m_view.iCamera];
    camera.zoom(offset);
}
