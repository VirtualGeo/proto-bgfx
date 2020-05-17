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
//#include <shaderc/shaderc.h>
//#include <shaderc/shaderc.h>

#include <bgfx/bgfx.h>
//#include <tools/shaderc/shaderc.h>
//#include <shaderc/shaderc.h>
//#include "tools/shaderc/shaderc.h"
//#include <shaderc/shaderc.h>

#include <utility.h>


std::list<WindowState*> s_windows;
float WindowState::s_fps;
size_t WindowState::s_epoch = 10;
double WindowState::s_deltaTime;
std::chrono::time_point<std::chrono::high_resolution_clock> WindowState::s_lastTime;
//std::chrono::time_point<std::chrono::high_resolution_clock> WindowState::s_currentTime;
//double WindowState::s_currentTime;
size_t WindowState::s_iFrame = 0;
double WindowState::s_sum = 0.0;
size_t WindowState::s_counter = 0;

WindowState::WindowState(void* nwh, void* ndt, int width, int height)
    : m_nwh(nwh)
    , m_ndt(ndt)
    , m_width(width)
    , m_height(height)
    , m_id(s_windows.size())
    , m_iCamera(entry::s_scene.m_cameras.size())
{

    m_fbh.idx = bgfx::kInvalidHandle;
    assert(!bgfx::isValid(m_fbh));

//    shaderc::compileShader(5, );

    if (m_id == 0) {
        //        s_bgfxInit = true;
        // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
        // Most graphics APIs must be used on the same thread that created the window.
        bgfx::renderFrame();

        bgfx::Init bgfxInit = {};
        //    bgfxInit.platformData.ndt = this->winId;
        //    bgfx::PlatformData pd;
        bgfxInit.platformData.ndt = m_ndt;
        bgfxInit.platformData.nwh = m_nwh;
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
        //            bgfxInit.type = bgfx::RendererType::Direct3D9;
        //                            bgfxInit.type = bgfx::RendererType::Direct3D11;
        //                    bgfxInit.type = bgfx::RendererType::Direct3D12;
        //        bgfxInit.type = bgfx::RendererType::OpenGL;
        //    bgfxInit.type = bgfx::RendererType::OpenGLES;
        //                            bgfxInit.type = bgfx::RendererType::Vulkan; // no swap chain
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
        assert(caps->supported & BGFX_CAPS_TEXTURE_COMPARE_LEQUAL); // sampler supported
        //        assert(caps->homogeneousDepth);

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

        Program::init(caps);
        Geometry::init();
        Texture::init();
        // ------------------------------- LOAD MODEL
        //                entry::s_scene.addModel(std::string(PROJECT_DIR) + "assets/sponza/sponza.obj");

        //                entry::s_scene.addModel("/home/gauthier/Downloads/Cougar2/cougar.obj");
        //                entry::s_scene.addModel("/home/gauthier/Downloads/San_Miguel/san-miguel-low-poly.obj");
        //                entry::s_scene.addModel("/home/gauthier/Downloads/San_Miguel/san-miguel.obj");
        //                entry::s_scene.addModel("/home/gauthier/Downloads/San_Miguel/san-miguel-blend.obj");

        //        entry::s_scene.addModel("C:\\Users\\gauthier.bouyjou\\Downloads\\CornellBox\\CornellBox-Original.obj");
        //        entry::s_scene.addModel("C:\\Users\\gauthier.bouyjou\\Downloads\\export\\cougar.obj");

        //        entry::s_scene.addLight(DirLight({ 0.0f, -1.0f, 0.5f }));
        //        entry::s_scene.addSpotLight(SpotLight({ 1.0f, 0.0f, 0.0f }, { -5.0f, 1.0f, 0.0f }));
        //        entry::s_scene.addLight({ 1.0f, 0.0f, 0.0f }, { -5.0f, 1.0f, 0.0f });
        //        entry::s_scene.addLight(bx::Vec3(1.0f, 0.0f, 0.0f), bx::Vec3(-5.0f, 1.0f, 0.0f));
        //        entry::s_scene.addSpotLight(bx::Vec3(1.0f, -0.1f, 0.1f), bx::Vec3(-3.0f, 1.0f, 0.0f));

        //        entry::s_scene.addSpotLight(bx::Vec3(0.0f, 0.0f, 1.0f), bx::Vec3(0.0f, 1.0f, 0.0f));
        //        entry::s_scene.addLight<SpotLight>({});

        //        entry::s_scene.addLight(PointLight({ 0.0f, 1.0f, 0.0f }));

        //        Q_ASSERT(m_iWindow == 0);
        bx::mtxIdentity(entry::g_mtx);
        //        entry::s_scene.m_cameras.emplace_back(bx::Vec3 { -5.0f, 1.0f, -0.5f }); // question : push_back ?
        entry::s_scene.m_cameras.emplace_back(bx::Vec3 { 0.0f, 0.0f, 300.0f }); // question : push_back ?
        s_lastTime = std::chrono::high_resolution_clock::now();

        const uint32_t nbCube = 100;
        for (uint32_t i = 0; i < nbCube; ++i) {
            for (uint32_t j = 0; j < nbCube; ++j) {
                float mtx[16];
                //                bgfx::setTexture(0, Program::m_sDiffuse,
                //                    Texture::m_sampleTextures[Texture::Sample((i + j) % 3 + Texture::RED)].textureHandle(), Texture::s_textureSamplerFlags);
                //            bx::mtxIdentity(mtx);
                bx::mtxIdentity(mtx);
                //                bx::mtxRotateXY(mtx, s_iFrame * 0.021f, s_iFrame * 0.037f);
                mtx[12] = -(float)nbCube / 2.0f * 3.0f + float(i) * 3.0f;
                mtx[13] = -(float)nbCube / 2.0f * 3.0f + float(j) * 3.0f;
                mtx[14] = 0.0f;

                const float color[] { 0.5f, 0.5f, 0.5f, 0.0f };
                Cube cube;
                memcpy(cube.mtx, mtx, 16 * sizeof(float));
                cube.hasDiffuseTexture = i > nbCube / 2;
                cube.hasSpecularTexture = j > nbCube / 2;
                memcpy(cube.diffuseColor, color, 4 * sizeof(float));

                m_cubes.push_back(cube);
                //                Geometry::drawCube(mtx);
                //                bgfx::submit(m_id, Program::m_programs[m_shading]);
            }
        }

        std::random_device rd;
        std::mt19937 g(rd());

        std::shuffle(m_cubes.begin(), m_cubes.end(), g);

        m_branchingTests[0] = Program::loadProgram("branchingTest1", "");
        m_branchingTests[1] = Program::loadProgram("branchingTest2", "");
        m_branchingTests[2] = Program::loadProgram("branchingTest3", "");

        m_branching3Tests[0] = Program::loadProgram("branchingTest3", "");
        m_branching3Tests[1] = Program::loadProgram("branchingTest3", "HAS_DIFFUSE_TEXTURE");
        m_branching3Tests[2] = Program::loadProgram("branchingTest3", "HAS_SPECULAR_TEXTURE");
        m_branching3Tests[3] = Program::loadProgram("branchingTest3", "HAS_DIFFUSE_TEXTURE;HAS_SPECULAR_TEXTURE");

    } else {
        entry::s_scene.m_cameras.emplace_back(bx::Vec3 { 5.0, 1.0f, -1.0f }); // question : push_back ?
        m_fbh = bgfx::createFrameBuffer((void*)(uintptr_t)m_nwh, uint16_t(m_width), uint16_t(m_height));
    }
    //    auto& window = entry::s_windows[m_id];

    s_windows.emplace_back(this);
    m_init = true;


}

WindowState::~WindowState()
{

    if (m_id == 0) {
        entry::s_scene.clear();
        //        s_program.clear();
        Program::clear();
        Geometry::clear();
        Texture::clear();
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

void WindowState::render() const
{


    if (!m_init)
        return;
    if (m_id != 0) {
        //            assert(bgfx::isValid(m_fbh));
        bgfx::setViewFrameBuffer(m_id, m_fbh);
    }
    //    bgfx::setViewFrameBuffer(m_id, m_fbh);
    // Set view 0 default viewport.
    bgfx::setViewRect(m_id, 0, 0, uint16_t(m_width), uint16_t(m_height));
    // This dummy draw call is here to make sure that view 0 is cleared
    // if no other draw calls are submitted to view 0.
    bgfx::setViewClear(m_id, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0X555555FF);
    //            bgfx::touch(m_id);

    //    return;
    const float ratio = float(m_width) / m_height;
    //        entry::s_scene.renderFromCamera(m_iCamera, ratio, m_id, m_shading, entry::g_mtx);

    const auto& camera = entry::s_scene.m_cameras[m_iCamera];
    camera.setViewTransform(ratio, m_id);

    //    Geometry::drawQuad();
    //    Geometry::drawCube();
    //    bgfx::submit(m_id, Program::m_programs[m_shading]);

    //    bgfx::setTexture(0, Program::m_sDiffuse, Texture::m_sampleTextures[Texture::BLUE].textureHandle(), Texture::s_textureSamplerFlags);
    //    const uint32_t nbCube = 100;
    //    for (uint32_t i = 0; i < nbCube; ++i) {
    //        for (uint32_t j = 0; j < nbCube; ++j) {
    //            float mtx[16];
    //            bgfx::setTexture(0, Program::m_sDiffuse,
    //                Texture::m_sampleTextures[Texture::Sample((i + j) % 3 + Texture::RED)].textureHandle(), Texture::s_textureSamplerFlags);
    //            //            bx::mtxIdentity(mtx);
    //            bx::mtxRotateXY(mtx, s_iFrame * 0.021f, s_iFrame * 0.037f);
    //            mtx[12] = -(float)nbCube / 2.0f * 3.0f + float(i) * 3.0f;
    //            mtx[13] = -(float)nbCube / 2.0f * 3.0f + float(j) * 3.0f;
    //            mtx[14] = 0.0f;

    //            Geometry::drawCube(mtx);
    //            bgfx::submit(m_id, Program::m_programs[m_shading]);
    //        }
    //    }
    for (const auto& cube : m_cubes) {
        float ones[] = { 1.0f };
        float zeros[] = { 0.0f };

                bgfx::setUniform(Program::m_diffuseColor, cube.diffuseColor);
        if (m_iBranchingTest == 0) {
            if (cube.hasDiffuseTexture) {
                //            bgfx::setTexture(0, Program::m_diffuseTexture, Texture::m_sampleTextures[Texture::RED].textureHandle());
                bgfx::setTexture(0, Program::m_diffuseTexture, Texture::getSampleTexture(Texture::RED));
                bgfx::setUniform(Program::m_hasDiffuseTexture, ones);
            } else {
                bgfx::setUniform(Program::m_hasDiffuseTexture, zeros);
            }
            if (cube.hasSpecularTexture) {
                //            bgfx::setTexture(1, Program::m_specularTexture, Texture::m_sampleTextures[Texture::GREEN].textureHandle());
                bgfx::setTexture(1, Program::m_specularTexture, Texture::getSampleTexture(Texture::GREEN));
                bgfx::setUniform(Program::m_hasSpecularTexture, ones);
            } else {
                bgfx::setUniform(Program::m_hasSpecularTexture, zeros);
            }

        } else if (m_iBranchingTest == 1) {
//                bgfx::setUniform(Program::m_diffuseColor, cube.diffuseColor);
            if (cube.hasDiffuseTexture) {
                //            bgfx::setTexture(0, Program::m_diffuseTexture, Texture::m_sampleTextures[Texture::RED].textureHandle());
                bgfx::setTexture(0, Program::m_diffuseTexture, Texture::getSampleTexture(Texture::RED));
//                bgfx::setUniform(Program::m_hasDiffuseTexture, ones);
            } else {
                bgfx::setTexture(0, Program::m_diffuseTexture, Texture::getSampleTexture(Texture::WHITE));
//                bgfx::setUniform(Program::m_diffuseColor, cube.diffuseColor);
//                bgfx::setUniform(Program::m_hasDiffuseTexture, zeros);
            }
            if (cube.hasSpecularTexture) {
                //            bgfx::setTexture(1, Program::m_specularTexture, Texture::m_sampleTextures[Texture::GREEN].textureHandle());
                bgfx::setTexture(1, Program::m_specularTexture, Texture::getSampleTexture(Texture::GREEN));
//                bgfx::setUniform(Program::m_hasSpecularTexture, ones);
            } else {
//                bgfx::setUniform(Program::m_hasSpecularTexture, zeros);
                bgfx::setTexture(1, Program::m_specularTexture, Texture::getSampleTexture(Texture::BLACK));
            }

        } else if (m_iBranchingTest == 2) {
            int iShader = 0;
            if (cube.hasDiffuseTexture) {
                //            bgfx::setTexture(0, Program::m_diffuseTexture, Texture::m_sampleTextures[Texture::RED].textureHandle());
                bgfx::setTexture(0, Program::m_diffuseTexture, Texture::getSampleTexture(Texture::RED));
                ++iShader;
//                bgfx::setUniform(Program::m_hasDiffuseTexture, ones);
//            } else {
//                bgfx::setTexture(0, Program::m_diffuseTexture, Texture::getSampleTexture(Texture::WHITE));
//                bgfx::setUniform(Program::m_diffuseColor, cube.diffuseColor);
//                bgfx::setUniform(Program::m_hasDiffuseTexture, zeros);
            }
            if (cube.hasSpecularTexture) {
                //            bgfx::setTexture(1, Program::m_specularTexture, Texture::m_sampleTextures[Texture::GREEN].textureHandle());
                bgfx::setTexture(1, Program::m_specularTexture, Texture::getSampleTexture(Texture::GREEN));
//                bgfx::setUniform(Program::m_hasSpecularTexture, ones);
                iShader += 2;
//            } else {
//                bgfx::setUniform(Program::m_hasSpecularTexture, zeros);
//                bgfx::setTexture(1, Program::m_specularTexture, Texture::getSampleTexture(Texture::BLACK));
            }
            Geometry::drawCube(cube.mtx);
            bgfx::submit(m_id, m_branching3Tests[iShader]);
            continue;
        }
        Geometry::drawCube(cube.mtx);
        //        bgfx::submit(m_id, Program::m_programs[m_shading]);
        bgfx::submit(m_id, m_branchingTests[m_iBranchingTest]);
    }
}

void WindowState::renderAllWindow()
{
    if (!m_init)
        return;
    //        if (m_id != 0)
    //            return;

    const auto currentTime = std::chrono::high_resolution_clock::now();
    //    s_currentTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime).count();
    //    s_currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count() / 1000000.0;

    //    const auto currentTime = g_timer.now();
    s_deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - s_lastTime).count() / 1000000.0;
    s_lastTime = currentTime;
    s_sum += s_deltaTime;

    //    qDebug() << g_epoch << m_sum << g_fps;
    if (s_counter >= s_epoch) {
        s_epoch = (s_fps = s_epoch / s_sum) / 2; // update g_fps each 0.5 sec
        //        m_epoch = (m_fps = m_epoch / m_sum); // update g_fps each sec
        s_sum = 0.0f;
        s_counter = 0;
    }
    ++s_counter;

    //    updateCameraPos();
    for (WindowState* window : s_windows) {
        window->updateCameraPos();
    }

    //    if (m_id == 0) {

    //    entry::s_scene.setLightUniforms();
    //    entry::s_scene.updateLightShadowMaps();
    //        bgfx::frame();

    //        int iWindow = 0;
    for (const WindowState* window : s_windows) {
        //        entry::s_scene.setLightShadowSamplers();
        window->render();
        //        ++iWindow;
    }
    //    render();

    //        g_scene.draw(1, g_program, g_mtx, state, g_cameraPos);

    // Advance to next frame. Process submitted rendering primitives.
    //    if (m_id == 0) { // avoid flipping, put F1 to show (only with direct3D)
    printDebugMessage();
    bgfx::frame();
    ++s_iFrame;
    //        entry::s_scene.updateLightShadowMaps();
    //        entry::s_scene.setLightUniforms();
//        m_iBranchingTest = (m_iBranchingTest + 1) % 2;
    //    }
    //    return;
}

//void WindowState::paint()
//{

//}

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
        bgfx::dbgTextPrintf(0, ++line, 0x0F, "Fps: %.2f", s_fps);
        int i = 0;
        for (const auto& window : s_windows) {
            //            bgfx::dbgTextPrintf(0, ++line, 0x0F, "Window: %d, Fps: %.2f, Backbuffer: %dx%d, Viewport shading: %s", i, window->m_fps, window->m_width, window->m_height, Program::filename(window->m_shading));
            bgfx::dbgTextPrintf(0, ++line, 0x0F, "Window: %d, Backbuffer: %dx%d, Viewport shading: %s", i, window->m_width, window->m_height, Program::filename(window->m_shading));
            ++i;
        }
        entry::s_scene.printStats(line);
    }
}

void WindowState::updateCameraPos()
{
    //    auto& window = entry::s_windows[m_iWindow];

    float cameraSpeed = m_slowMotion ? 1.0f : 5.0f;
    cameraSpeed *= s_deltaTime;

    //    auto& camera = *entry::s_scene.m_cameras[m_iCamera];
    auto& camera = entry::s_scene.m_cameras[m_iCamera];
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

        //        auto& camera = *entry::s_scene.m_cameras[m_iCamera];
        auto& camera = entry::s_scene.m_cameras[m_iCamera];
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
        //        for (auto& window : s_windows) {
        //            window->m_epoch = 10;
        //        }
        s_epoch = 10;
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
    //    auto& camera = *entry::s_scene.m_cameras[m_iCamera];
    auto& camera = entry::s_scene.m_cameras[m_iCamera];
    camera.zoom(offset);
}
