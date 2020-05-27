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
#include <unistd.h>

//#include <optick.h>

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
            bgfxInit.resolution.reset = BGFX_RESET_NONE;
//        bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
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
//        entry::s_scene.m_cameras.emplace_back(bx::Vec3 { 0.0f, 0.0f, 3.0f }); // question : push_back ?
        s_lastTime = std::chrono::high_resolution_clock::now();

//                const uint32_t nbCube = 2;
//                const uint32_t nbCube = 16;
//                        const uint32_t nbCube = 32;
                const uint32_t nbCube = 64;
//        const uint32_t nbCube = 128;
        //        const uint32_t nbCube = 256;
        //                const uint32_t nbCube = Program::s_nTexture * 4;
        //        const uint32_t nbCube = Program::s_nTexture;

        //        const int nArea = 2 + Program::s_nAditionalTexture / 2;
        //        const int nArea = std::ceil(std::sqrt(Program::s_nTexture));
        //        const int nArea = 1 + std::ceil(Program::s_nTexture / 2.0);
        //        const int nArea = 2;
        //        const int nArea = 1 + std::ceil(Program::s_nTexture / 2.0f);
        //        const int nArea = std::ceil(std::sqrt(std::pow(2, Program::s_nTexture)));
        const int nArea = std::pow(2, Program::s_nTexture / 2.0f);
        int iShader = 0;
        for (int iArea = 0; iArea < nArea; ++iArea) {
            for (int jArea = 0; jArea < nArea; ++jArea) {
                //                std::string hasAdditional = "N_TEXTURE=" + (std::to_string(Program::s_nTexture));
                for (uint32_t iCube = 0; iCube < nbCube / nArea; ++iCube) {
                    int i = iCube + (nbCube / nArea) * iArea;
                    for (uint32_t jCube = 0; jCube < nbCube / nArea; ++jCube) {
                        int j = jCube + (nbCube / nArea) * jArea;
                        float mtx[16];
                        //                bgfx::setTexture(0, Program::m_sDiffuse,
                        //                    Texture::m_sampleTextures[Texture::Sample((i + j) % 3 + Texture::RED)].textureHandle(), Texture::s_textureSamplerFlags);
                        //            bx::mtxIdentity(mtx);
                        bx::mtxIdentity(mtx);
                        //                bx::mtxRotateXY(mtx, s_iFrame * 0.021f, s_iFrame * 0.037f);
                        mtx[12] = -(float)nbCube / 2.0f * 3.0f + float(i) * 3.0f;
                        mtx[13] = -(float)nbCube / 2.0f * 3.0f + float(j) * 3.0f;
                        mtx[14] = 0.0f;

                        //                        const float color[] { 0.5f, 0.5f, 0.5f, 0.0f };
                        Cube cube;
                        memcpy(cube.mtx, mtx, 16 * sizeof(float));
                        //                        cube.hasDiffuseTexture = i > nbCube / nArea;
                        //                        cube.hasSpecularTexture = j > nbCube / nArea;
                        //                        memcpy(cube.diffuseColor, color, 4 * sizeof(float));

                        if (iShader < std::pow(2, Program::s_nTexture)) {
                            int temp = iShader;
                            for (int k = 0; k < Program::s_nTexture; ++k) {
                                int bit = temp % 2;

                                cube.hasTexture[k] = bit;
                                //                                cube.hasTexture[k] = 0;
                                //                                if (iCube == 0 && jCube == 0 && bit) {
                                //                                    hasAdditional += ";HAS_TEXTURE_" + std::to_string(k);
                                //                                }
                                //                            temp >>= 1;
                                temp /= 2;
                                //                            cube.hasTexture[iTex * 2] = i > ((2 + iTex) * nbCube / nArea);
                                //                            cube.hasTexture[iTex * 2 + 1] = j > ((2 + iTex) * nbCube / nArea);
                                //                        cube.hasAdditionalTexture[0] = true;
                                //                        cube.hasAdditionalTexture[1] = true;
                            }
                        }
                        cube.iShader = iShader;

                        m_cubes.push_back(cube);
                        //                Geometry::drawCube(mtx);
                        //                bgfx::submit(m_id, Program::m_programs[m_shading]);
                    }
                }

                //                ++iTex;
                //                m_branching3Tests[iShader] = Program::loadProgram("branchingTest3", (hasAdditional).c_str());
                //                bgfx::frame();
                ++iShader;
            }
        }

        //                std::random_device rd;
        //                std::mt19937 g(rd());
        //                std::shuffle(m_cubes.begin(), m_cubes.end(), g);

        m_branchingTests[0] = Program::loadProgram("branchingTest1", ("N_TEXTURE=" + std::to_string(Program::s_nTexture)).c_str());
        m_branchingTests[1] = Program::loadProgram("branchingTest2", ("N_TEXTURE=" + std::to_string(Program::s_nTexture)).c_str());
        //        m_branchingTests[1] = Program::loadProgram("branchingTest2", "");
        m_branchingTests[2] = Program::loadProgram("branchingTest3", "");
        //        m_branchingTests[2] = Program::loadProgram("branchingTest3", ("N_TEXTURE=" + std::to_string(Program::s_nTexture) + ";HAS_TEXTURE_0;HAS_TEXTURE_3;HAS_TEXTURE_5;HAS_TEXTURE_6;HAS_TEXTURE_7;HAS_TEXTURE_8").c_str());

        //        m_branching3Tests[0] = Program::loadProgram("branchingTest3", ("N_ADDITIONAL_TEXTURE=" + std::to_string(Program::s_nAditionalTexture)).c_str());
        //        m_branching3Tests[1] = Program::loadProgram("branchingTest3", ("N_ADDITIONAL_TEXTURE=" + (std::to_string(Program::s_nAditionalTexture)) + ";HAS_DIFFUSE_TEXTURE").c_str());
        //        m_branching3Tests[2] = Program::loadProgram("branchingTest3", ("N_ADDITIONAL_TEXTURE=" + (std::to_string(Program::s_nAditionalTexture)) + ";HAS_SPECULAR_TEXTURE").c_str());
        //        m_branching3Tests[3] = Program::loadProgram("branchingTest3", ("N_ADDITIONAL_TEXTURE=" + (std::to_string(Program::s_nAditionalTexture)) + ";HAS_DIFFUSE_TEXTURE;HAS_SPECULAR_TEXTURE").c_str());

        //        for (int i = 0; i < std::pow(2, 2 + Program::s_nAditionalTexture); ++i) {
        //                        if (iShader < std::pow(2, Program::s_nTexture)) {

        for (int i = 0; i < std::pow(2, Program::s_nTexture); ++i) {
            std::string hasAdditional = "N_TEXTURE=" + (std::to_string(Program::s_nTexture));
            int iTemp = i;
            for (int j = 0; j < Program::s_nTexture; ++j) {
                int bit = iTemp % 2;

                //                        hasAdditional += ";HAS_TEXTURE_" + std::to_string(j) + "=" + std::to_string(bit);
                if (bit) {
                    hasAdditional += ";HAS_TEXTURE_" + std::to_string(j);
                }

                iTemp /= 2;
            }
            std::cout << "[WINDOWSTATE] loadProgram branchingTest3 " << hasAdditional << std::endl;
            m_branching3Tests[i] = Program::loadProgram("branchingTest3", (hasAdditional).c_str());

            //            m_branching3Tests[i] = Program::loadProgram("branchingTest3", "");
            //                                bgfx::frame();
            //                                if (i  % 100 == 0) {
            //                                    bgfx::frame();
            //                                }
        }

        //            for (int j =0; j < std::pow(2, i); ++j) {
        //            m_branching3Tests[4] = Program::loadProgram("branchingTest3", ("N_ADDITIONAL_TEXTURE=" + (std::to_string(Program::s_nAditionalTexture)) + ";HAS_DIFFUSE_TEXTURE;HAS_SPECULAR_TEXTURE;HAS_ADDITIONAL_TEXTURE_" + std::to_string(0)).c_str());

        //            m_branching3Tests[5] = Program::loadProgram("branchingTest3", ("N_ADDITIONAL_TEXTURE=" + (std::to_string(Program::s_nAditionalTexture)) + ";HAS_DIFFUSE_TEXTURE;HAS_SPECULAR_TEXTURE;HAS_ADDITIONAL_TEXTURE_" + std::to_string(1)).c_str());
        //            m_branching3Tests[5] = Program::loadProgram("branchingTest3", ("N_ADDITIONAL_TEXTURE=" + (std::to_string(Program::s_nAditionalTexture)) + ";HAS_DIFFUSE_TEXTURE;HAS_SPECULAR_TEXTURE;HAS_ADDITIONAL_TEXTURE_" + std::to_string(0) + ";HAS_ADDITIONAL_TEXTURE_" + std::to_string(1)).c_str());
        //            }
        //        }

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
    int iShader = -1;
    //    bgfx::ProgramHandle programHandle;
    //        float ones[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    //        float zeros[] = { 0.0f };

    //        bgfx::setUniform(Program::m_diffuseColor, cube.diffuseColor);

    if (m_iBranchingTest == 0) {
        //            float additionalTexture[4] = { Program::s_nTexture - 2 };
        //            bgfx::setUniform(Program::m_nAdditionalTexture, additionalTexture);
        for (const auto& cube : m_cubes) {

            //            if (cube.hasTexture[0]) {
            //                //            bgfx::setTexture(0, Program::m_diffuseTexture, Texture::m_sampleTextures[Texture::RED].textureHandle());
            //                bgfx::setTexture(0, Program::m_diffuseTexture, Texture::getSampleTexture(Texture::RED));
            //                bgfx::setUniform(Program::m_hasDiffuseTexture, ones);
            //            } else {
            //                bgfx::setUniform(Program::m_hasDiffuseTexture, zeros);
            //            }
            //            if (cube.hasTexture[1]) {
            //                //            bgfx::setTexture(1, Program::m_specularTexture, Texture::m_sampleTextures[Texture::GREEN].textureHandle());
            //                bgfx::setTexture(1, Program::m_specularTexture, Texture::getSampleTexture(Texture::GREEN));
            //                bgfx::setUniform(Program::m_hasSpecularTexture, ones);
            //            } else {
            //                bgfx::setUniform(Program::m_hasSpecularTexture, zeros);
            //            }

            if (iShader != cube.iShader) {
                float buffer[(Program::s_nTexture)*4] = { 0.0f };
                for (int i = 0; i < (Program::s_nTexture); ++i) {
                    if (cube.hasTexture[i]) {
                        bgfx::setTexture(i, Program::m_texture[i], Texture::getSampleTexture(Texture::Sample(i + Texture::RED)));
                        //                    bgfx::setTexture(2 + i, Program::m_additionalTexture[i], Texture::getSampleTexture(Texture::BLUE));
                        buffer[4 * i] = 1.0f;
                    } else {

                        buffer[4 * i] = 0.0f;
                    }
                    //                    buffer[4 * i] = 1.0f;
                    //                buffer[4 * i] = cube.hasAdditionalTexture[i];
                }
                bgfx::setUniform(Program::m_hasTexture, buffer, Program::s_nTexture);
                iShader = cube.iShader;
            }
            //            buffer[1] = Program::s_nAditionalTexture;

//            Geometry::drawCube(cube.mtx);
            Geometry::drawUVSphere(cube.mtx);
            bgfx::submit(m_id, m_branchingTests[m_iBranchingTest]);
        }

    } else if (m_iBranchingTest == 1) {
        for (const auto& cube : m_cubes) {
            //            float additionalTexture[4] = { Program::s_nTexture };
            //            bgfx::setUniform(Program::m_nAdditionalTexture, additionalTexture);
            //                bgfx::setUniform(Program::m_diffuseColor, cube.diffuseColor);
            //            if (cube.hasTexture[0]) {
            //                //            bgfx::setTexture(0, Program::m_diffuseTexture, Texture::m_sampleTextures[Texture::RED].textureHandle());
            //                bgfx::setTexture(0, Program::m_diffuseTexture, Texture::getSampleTexture(Texture::RED));
            //                //                bgfx::setUniform(Program::m_hasDiffuseTexture, ones);
            //            } else {
            //                bgfx::setTexture(0, Program::m_diffuseTexture, Texture::getSampleTexture(Texture::WHITE));
            //                //                bgfx::setUniform(Program::m_diffuseColor, cube.diffuseColor);
            //                //                bgfx::setUniform(Program::m_hasDiffuseTexture, zeros);
            //            }
            //            if (cube.hasTexture[1]) {
            //                //            bgfx::setTexture(1, Program::m_specularTexture, Texture::m_sampleTextures[Texture::GREEN].textureHandle());
            //                bgfx::setTexture(1, Program::m_specularTexture, Texture::getSampleTexture(Texture::GREEN));
            //                //                bgfx::setUniform(Program::m_hasSpecularTexture, ones);
            //            } else {
            //                //                bgfx::setUniform(Program::m_hasSpecularTexture, zeros);
            //                bgfx::setTexture(1, Program::m_specularTexture, Texture::getSampleTexture(Texture::BLACK));
            //            }

            if (iShader != cube.iShader) {
                for (int i = 0; i < Program::s_nTexture; ++i) {
                    if (cube.hasTexture[i]) {
                        bgfx::setTexture(i, Program::m_texture[i], Texture::getSampleTexture(Texture::Sample(i + Texture::RED)));
                    } else {
                        bgfx::setTexture(i, Program::m_texture[i], Texture::getSampleTexture(Texture::Sample::BLACK));
                    }
                }
                iShader = cube.iShader;
            }
//            Geometry::drawCube(cube.mtx);
            Geometry::drawUVSphere(cube.mtx);
//            bgfx::submit(m_id, m_branchingTests[m_iBranchingTest]);
            bgfx::submit(m_id, m_branchingTests[m_iBranchingTest]);
        }

    } else if (m_iBranchingTest == 2) {
        for (const auto& cube : m_cubes) {
            //            float additionalTexture[4] = { Program::s_nAditionalTexture };
            //            bgfx::setUniform(Program::m_nAdditionalTexture, additionalTexture);

            //            int iShader = 0;
            //            if (cube.hasTexture[0]) {
            //                //            bgfx::setTexture(0, Program::m_diffuseTexture, Texture::m_sampleTextures[Texture::RED].textureHandle());
            //                bgfx::setTexture(0, Program::m_diffuseTexture, Texture::getSampleTexture(Texture::RED));
            ////                ++iShader;
            //                //                bgfx::setUniform(Program::m_hasDiffuseTexture, ones);
            //                //            } else {
            //                //                bgfx::setTexture(0, Program::m_diffuseTexture, Texture::getSampleTexture(Texture::WHITE));
            //                //                bgfx::setUniform(Program::m_diffuseColor, cube.diffuseColor);
            //                //                bgfx::setUniform(Program::m_hasDiffuseTexture, zeros);
            //            }
            //            if (cube.hasTexture[1]) {
            //                //            bgfx::setTexture(1, Program::m_specularTexture, Texture::m_sampleTextures[Texture::GREEN].textureHandle());
            //                bgfx::setTexture(1, Program::m_specularTexture, Texture::getSampleTexture(Texture::GREEN));
            //                //                bgfx::setUniform(Program::m_hasSpecularTexture, ones);
            ////                iShader += 2;
            //                //            } else {
            //                //                bgfx::setUniform(Program::m_hasSpecularTexture, zeros);
            //                //                bgfx::setTexture(1, Program::m_specularTexture, Texture::getSampleTexture(Texture::BLACK));
            //            }

            //            int jump = 4;
            if (iShader != cube.iShader) {
                for (int i = 0; i < Program::s_nTexture; ++i) {
                    if (cube.hasTexture[i]) {
                        bgfx::setTexture(i, Program::m_texture[i], Texture::getSampleTexture(Texture::Sample(i + Texture::RED)));
                        //                    iShader += jump;
                        //                                    } else {
                        //                                        bgfx::setTexture(2 + i, Program::m_texture[i], Texture::getSampleTexture(Texture::Sample::BLACK));
                        //                                        bgfx::setTexture(i, Program::m_texture[i], Texture::getSampleTexture(Texture::Sample::BLACK));
                    }
                    //                jump *= 2;
                }
                iShader = cube.iShader;
            }
//            Geometry::drawCube(cube.mtx);
            Geometry::drawUVSphere(cube.mtx);
            //            assert(iShader < 4)
            bgfx::submit(m_id, m_branching3Tests[cube.iShader]);
            //            continue;
        }
        //        bgfx::submit(m_id, Program::m_programs[m_shading]);
        //                bgfx::submit(m_id, m_branchingTests[m_iBranchingTest], 0, BGFX_DISCARD_NONE | BGFX_DISCARD_TEXTURE_SAMPLERS);
        //                bgfx::submit(m_id, m_branchingTests[m_iBranchingTest], 0, BGFX_DISCARD_NONE);
        //                bgfx::submit(m_id, m_branchingTests[m_iBranchingTest], 0, BGFX_DISCARD_ALL);
    }
}

void WindowState::renderAllWindow()
{
    if (!m_init)
        return;
    //        if (m_id != 0)
    //            return;
    //    OPTICK_FRAME("MainThread");

    const auto currentTime = std::chrono::high_resolution_clock::now();
    //    s_currentTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime).count();
    //    s_currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count() / 1000000.0;

    //    const auto currentTime = g_timer.now();
    s_deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - s_lastTime).count() / 1000000.0;
    s_lastTime = currentTime;
    s_sum += s_deltaTime;

    //    qDebug() << g_epoch << m_sum << g_fps;
    if (s_counter >= s_epoch) {
        //        s_epoch = (s_fps = s_epoch / s_sum) / 2; // update g_fps each 0.5 sec
        s_epoch = (s_fps = s_epoch / s_sum); // update g_fps each sec
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

//    for (int i =0; i < 100; ++i) {
    for (auto & cube : m_cubes ) {
        cube.velocity = bx::add(cube.velocity, bx::mul(bx::Vec3{0.0f, -9.81f, 0.0f}, s_deltaTime));
//        cube.velocity.y *= 0.99f;
        bx::Vec3 temp = bx::mul(cube.velocity, s_deltaTime);
//        memcpy(&cube.mtx[12], &temp, sizeof (temp));
        cube.mtx[12] += temp.x;
        cube.mtx[13] += temp.y;
        cube.mtx[14] += temp.z;

        if (cube.mtx[13] < 0.0f && cube.velocity.y < 0.0f) {
            cube.velocity.y *= -1.0f;
        }
//        sleep(0.001);
    }
//    }

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
//    usleep(16000);
    ++s_iFrame;
    //        entry::s_scene.updateLightShadowMaps();
    //        entry::s_scene.setLightUniforms();
//                                m_iBranchingTest = (m_iBranchingTest + 1) % 3;
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
