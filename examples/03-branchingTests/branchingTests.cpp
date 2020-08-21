#include <engine/geometry.h>
#include <entry/entry.h>

//#define SMART
#define UNSORTED
//#define DRAW_SPHERE

#include <cassert>
#include <cstring>
#include <math.h>

//#include <algorithm>
#include <random>

namespace entry {
int s_nWindow = 1;

//    static constexpr int s_nArea = 2;
//static unsigned int s_nTexture;
constexpr unsigned int s_nMaxTexture = 3;
//constexpr unsigned int s_nMaxTexture = 6;
//constexpr unsigned int s_nMaxTexture = 7;
//constexpr unsigned int s_nMaxTexture = 14;
//constexpr unsigned int s_nMaxTexture = 16;
constexpr uint32_t s_nbCubePerLine = 128;
constexpr size_t s_nbCubes = s_nbCubePerLine * s_nbCubePerLine;
//constexpr unsigned int s_nMaxTexture = 3;
//static bgfx::UniformHandle m_hasTexture;
//static bgfx::UniformHandle m_texture[];
//    static bgfx::UniformHandle m_nAdditionalTexture;
unsigned int s_nTexture = 1;
bgfx::UniformHandle m_hasTexture;
bgfx::UniformHandle m_texture[s_nMaxTexture];

struct Cube {
    float mtx[16];
    //        bool hasDiffuseTexture;
    //        bool hasSpecularTexture;
    float diffuseColor[4];
    bx::Vec3 velocity = { 0.0f, 0.0f, 0.0f };

    bool hasTexture[s_nMaxTexture] = { 0 };
    int iShader;
};
static std::vector<Cube> m_cubes;

static bgfx::ProgramHandle m_branchingTests[3];
static bgfx::ProgramHandle m_branching3Tests[2048];
static uint32_t m_iBranchingTest = 0;

static std::chrono::time_point<std::chrono::high_resolution_clock> s_testStart;
static const int s_nbTestFrame = 100;
//static const int s_nbTestFrame = 500;
//    static const int s_nbTestFrame = 1000;

void initCubeScene();
void clearCubeScene();

int bitSum(size_t number) {
    int sum = 0;
    while (number != 0) {
        int bit = number % 2;
        sum += bit;
        number >>= 1;
    }
    return sum;
}

void init(View& view)
{
    m_cubes.clear();
    m_cubes.reserve(s_nbCubes);

    Geometry::init();
    Texture::init();
    Program::init();
    //        initCubeScene();
    //    assert(entry::s_scene.m_cameras.size() != 0);
    entry::s_scene.m_cameras.emplace_back(bx::Vec3 { 0.0f, 0.0f, 200.0f }); // question : push_back ?
    //    entry::s_scene.m_cameras[0].setTarget();

    view.projection = Projection::ORTHOGRAPHIC;

    m_hasTexture = bgfx::createUniform("hasTexture", bgfx::UniformType::Vec4, s_nMaxTexture);
    for (int i = 1; i <= s_nMaxTexture; ++i) {
        m_texture[i] = bgfx::createUniform((std::string("texture") + std::to_string(i)).c_str(), bgfx::UniformType::Sampler);
    }

    std::cout << "theorical prediction of branching comparizon due of number of operation per test" << std::endl;
    //    std::cout << "num texture\t| 1\t| 2\t| 3\t| 4\t| 5\t| 6\t| 7\t| 8\t| 9\t| 10" << std::endl;
    std::cout << "num texture\t| ";
    for (int i =0; i <s_nMaxTexture; ++i) {
        std::cout << std::to_string(i) << "\t| ";
    }
    std::cout << std::endl;

    const int testPrices[]{2, 2, 2};
    const int setTexturePrice = 3;
//    const int nbCubes = s_nbCubePerLine * s_nbCubePerLine;
#ifdef	UNSORTED
    //    for (int iTest = 0; iTest <2; ++iTest) {
    //        std::cout << "test " << std::to_string(iTest) << "\t";
    std::cout << "test 0\t";
    for (int iNTexture = 1; iNTexture <= s_nMaxTexture; ++iNTexture) {
        const size_t nbCombination = pow(2, iNTexture);
        size_t sum = 0;
        for (size_t i = 0; i < nbCombination; ++i) {
            for (size_t j = 0; j < nbCombination; ++j) {
                size_t k = ~i & j;
                sum += bitSum(k);
            }
        }
        float res;
        //            res = (sum * setTexturePrice + nbCombination * nbCombination * (testPrices[iTest] + (2 - iTest) * iNTexture)) / float(nbCombination * nbCombination);
        res = sum / float(nbCombination * nbCombination) * setTexturePrice + testPrices[0] + 2 * iNTexture;
        std::cout << "\t" << res;
    }
    std::cout << std::endl;
    //    }
    std::cout << "test 1\t";
    for (int iNTexture = 1; iNTexture <= s_nMaxTexture; ++iNTexture) {
        const size_t nbCombination = pow(2, iNTexture);
        size_t sum = 0;
        for (size_t i = 0; i < nbCombination; ++i) {
            for (size_t j = 0; j < nbCombination; ++j) {
                size_t k = i ^ j;
                sum += bitSum(k);
            }
        }
        float res;
        //            res = (sum * setTexturePrice + nbCombination * nbCombination * (testPrices[iTest] + (2 - iTest) * iNTexture)) / float(nbCombination * nbCombination);
        res = sum / float(nbCombination * nbCombination) * setTexturePrice + testPrices[1] + iNTexture;
        std::cout << "\t" << res;
    }
    std::cout << std::endl;
#else
        std::cout << "test 0\t";
        for (int iNTexture = 1; iNTexture <= s_nMaxTexture; ++iNTexture) {
            size_t sum = 0;
            const size_t nbCombination = pow(2, iNTexture);
            for (size_t i = 0; i < nbCombination - 1; ++i) {
                size_t k = ~i & (i + 1);
                sum += bitSum(k);
            }
            float res = (sum * setTexturePrice + s_nbCubes * (2 + 2 * iNTexture)) / (float)s_nbCubes;
            std::cout << "\t" << res;
        }
        std::cout << std::endl;

        std::cout << "test 1\t";
        for (int iNTexture = 1; iNTexture <= s_nMaxTexture; ++iNTexture) {
            size_t sum = 0;
            const size_t nbCombination = pow(2, iNTexture);
            for (size_t i = 0; i < nbCombination - 1; ++i) {
                size_t k = i ^ (i + 1);
                sum += bitSum(k);
            }
            float res = (sum * setTexturePrice + s_nbCubes * (2 +  iNTexture)) / (float)s_nbCubes;
            std::cout << "\t" << res;
        }
        std::cout << std::endl;

#endif

    std::cout << "test 2\t";
    for (int iNTexture = 1; iNTexture <= s_nMaxTexture; ++iNTexture) {
        size_t sum = 0;
        const size_t nbCombination = pow(2, iNTexture);
        for (size_t i = 0; i < nbCombination; ++i) {
            sum += (bitSum(i) + testPrices[2]);
        }
        float res = (((sum * s_nbCubes) / (float)nbCombination) + (10 + iNTexture * setTexturePrice) * nbCombination) / s_nbCubes;
        //        sum = sum / (float)nbCombination + 13.0f * (float)nbCombination / (s_nbCubePerLine * s_nbCubePerLine);
        //        float res = (sum * s_nbCubes + 13 * nbCombination * nbCombination) / float(s_nbCubes * nbCombination);
        //        sum /= (float)nbCombination;
        //        sum += 13.0f * nbCombination;
        std::cout << "\t" << res;
    }
    std::cout << std::endl;

//    exit(0);
}

void shutdown()
{
    Geometry::shutdown();
    Texture::shutdown();
    Program::shutdown();
    clearCubeScene();

    bgfx::destroy(m_hasTexture);
    for (int i = 0; i < s_nMaxTexture; ++i) {
        bgfx::destroy(m_texture[i]);
    }
}

void preRender()
{
}

void render(const View& view)
{
    assert(0 <= view.iCamera && view.iCamera < entry::s_scene.m_cameras.size());
    const Camera& camera = entry::s_scene.m_cameras[view.iCamera];
    camera.setViewTransform(view);

    if (s_iFrame == 0) {
        initCubeScene();
        bgfx::frame();
        s_testStart = std::chrono::high_resolution_clock::now();
        std::cout << "practical branching comparizon" << std::endl;
        std::cout << "num texture\t| 1\t| 2\t| 3\t| 4\t| 5\t| 6\t| 7\t| 8\t| 9\t| 10" << std::endl;
        std::cout << "test 0\t";
    }
    //            if (m_id != 0)
    //                return;
    //        OPTICK_FRAME("MainThread");

    const auto currentTime = std::chrono::high_resolution_clock::now();

    if (s_iFrame != 0 && s_iFrame % s_nbTestFrame == 0) {
        const auto deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - s_testStart).count() / 1000000.0;
        //        std::cout << "test " << m_iBranchingTest << ", num texture: " << s_nTexture << ", mean fps: " << s_nbTestFrame / deltaTime << std::endl;
        std::cout << "\t" << (s_nbTestFrame - 10) / deltaTime;
        clearCubeScene();

        ++s_nTexture;
        if (s_nTexture == s_nMaxTexture + 1) {
            s_nTexture = 1;
            ++m_iBranchingTest;
            if (m_iBranchingTest == 3) {
                std::cout << std::endl;
                exit(0);
            }
            std::cout << std::endl
                      << "test " << m_iBranchingTest << "\t";
        }
        initCubeScene();
        //        bgfx::frame();
        //        s_testStart = std::chrono::high_resolution_clock::now();
    }
    if (s_iFrame % s_nbTestFrame == 10) {
        //        initCubeScene();
        //        bgfx::frame();
        s_testStart = std::chrono::high_resolution_clock::now();
    }
    //    s_currentTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime).count();
    //        s_currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count() / 1000000.0;

    //    for (int i =0; i < 100; ++i) {
    // --------------------------- physics
    //    for (auto& cube : m_cubes) {
    //        cube.velocity = bx::add(cube.velocity, bx::mul(bx::Vec3 { 0.0f, -9.81f, 0.0f }, s_deltaTime));
    //        //        cube.velocity.y *= 0.99f;
    //        bx::Vec3 temp = bx::mul(cube.velocity, s_deltaTime);
    //        //        memcpy(&cube.mtx[12], &temp, sizeof (temp));
    //        cube.mtx[12] += temp.x;
    //        cube.mtx[13] += temp.y;
    //        cube.mtx[14] += temp.z;

    //        if (cube.mtx[13] < 0.0f && cube.velocity.y < 0.0f) {
    //            cube.velocity.y *= -1.0f;
    //        }
    //        //        sleep(0.001);
    //    }
    //    //    }

    // --------------------------------------------------------------------
    //    Geometry::drawQuad();
    //    Geometry::drawCube();
    //        return;
    //    bgfx::submit(m_id, m_programs[m_shading]);

//        bgfx::setTexture(0, m_sDiffuse, Texture::m_sampleTextures[Texture::BLUE].textureHandle(), Texture::s_textureSamplerFlags);
//        const uint32_t nbCube = 100;
//        for (uint32_t i = 0; i < nbCube; ++i) {
//            for (uint32_t j = 0; j < nbCube; ++j) {
//                float mtx[16];
//                bgfx::setTexture(0, m_sDiffuse,
//                    Texture::m_sampleTextures[Texture::Sample((i + j) % 3 + Texture::RED)].textureHandle(), Texture::s_textureSamplerFlags);
//                //            bx::mtxIdentity(mtx);
//                bx::mtxRotateXY(mtx, s_iFrame * 0.021f, s_iFrame * 0.037f);
//                mtx[12] = -(float)nbCube / 2.0f * 3.0f + float(i) * 3.0f;
//                mtx[13] = -(float)nbCube / 2.0f * 3.0f + float(j) * 3.0f;
//                mtx[14] = 0.0f;

//                Geometry::drawCube(mtx);
//                bgfx::submit(view.id, m_programs[view.shading]);
//            }
//        }
#ifdef SMART
    int iShader = -1;
#endif
    //    bgfx::ProgramHandle programHandle;
    //        float ones[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    //        float zeros[] = { 0.0f };

    //        bgfx::setUniform(m_diffuseColor, cube.diffuseColor);

    if (m_iBranchingTest == 0) {
        //            float additionalTexture[4] = { s_nTexture - 2 };
        //            bgfx::setUniform(m_nAdditionalTexture, additionalTexture);
        for (const auto& cube : m_cubes) {

            //            if (cube.hasTexture[0]) {
            //                //            bgfx::setTexture(0, m_diffuseTexture, Texture::m_sampleTextures[Texture::RED].textureHandle());
            //                bgfx::setTexture(0, m_diffuseTexture, Texture::getSampleTexture(Texture::RED));
            //                bgfx::setUniform(m_hasDiffuseTexture, ones);
            //            } else {
            //                bgfx::setUniform(m_hasDiffuseTexture, zeros);
            //            }
            //            if (cube.hasTexture[1]) {
            //                //            bgfx::setTexture(1, m_specularTexture, Texture::m_sampleTextures[Texture::GREEN].textureHandle());
            //                bgfx::setTexture(1, m_specularTexture, Texture::getSampleTexture(Texture::GREEN));
            //                bgfx::setUniform(m_hasSpecularTexture, ones);
            //            } else {
            //                bgfx::setUniform(m_hasSpecularTexture, zeros);
            //            }

#ifdef SMART
            if (iShader != cube.iShader) {
#endif
                float buffer[(s_nMaxTexture)*4] = { 0.0f };
                for (int i = 0; i < (s_nTexture); ++i) {
                    if (cube.hasTexture[i]) {
                        bgfx::setTexture(i, m_texture[i], Texture::getSampleTexture(Texture::Sample(i + Texture::RED)));
                        //                    bgfx::setTexture(2 + i, m_additionalTexture[i], Texture::getSampleTexture(Texture::BLUE));
                        buffer[4 * i] = 1.0f;
                    } else {

                        buffer[4 * i] = 0.0f;
                    }
                    //                    buffer[4 * i] = 1.0f;
                    //                buffer[4 * i] = cube.hasAdditionalTexture[i];
                }
                bgfx::setUniform(m_hasTexture, buffer, s_nTexture);
#ifdef SMART
                iShader = cube.iShader;
            }
#endif
            //            buffer[1] = s_nAditionalTexture;

            //            Geometry::drawCube(cube.mtx);
#ifdef DRAW_SPHERE
            Geometry::drawUVSphere(cube.mtx);
#else
                Geometry::drawCube(cube.mtx);
#endif
            bgfx::submit(view.id, m_branchingTests[m_iBranchingTest]);
        }

    } else if (m_iBranchingTest == 1) {
        for (const auto& cube : m_cubes) {
            //            float additionalTexture[4] = { s_nTexture };
            //            bgfx::setUniform(m_nAdditionalTexture, additionalTexture);
            //                bgfx::setUniform(m_diffuseColor, cube.diffuseColor);
            //            if (cube.hasTexture[0]) {
            //                //            bgfx::setTexture(0, m_diffuseTexture, Texture::m_sampleTextures[Texture::RED].textureHandle());
            //                bgfx::setTexture(0, m_diffuseTexture, Texture::getSampleTexture(Texture::RED));
            //                //                bgfx::setUniform(m_hasDiffuseTexture, ones);
            //            } else {
            //                bgfx::setTexture(0, m_diffuseTexture, Texture::getSampleTexture(Texture::WHITE));
            //                //                bgfx::setUniform(m_diffuseColor, cube.diffuseColor);
            //                //                bgfx::setUniform(m_hasDiffuseTexture, zeros);
            //            }
            //            if (cube.hasTexture[1]) {
            //                //            bgfx::setTexture(1, m_specularTexture, Texture::m_sampleTextures[Texture::GREEN].textureHandle());
            //                bgfx::setTexture(1, m_specularTexture, Texture::getSampleTexture(Texture::GREEN));
            //                //                bgfx::setUniform(m_hasSpecularTexture, ones);
            //            } else {
            //                //                bgfx::setUniform(m_hasSpecularTexture, zeros);
            //                bgfx::setTexture(1, m_specularTexture, Texture::getSampleTexture(Texture::BLACK));
            //            }

#ifdef SMART
            if (iShader != cube.iShader) {
#endif
                //                for (int i = 0; i < s_nTexture; ++i) {
                //                        bgfx::setTexture(i, m_texture[i], Texture::getSampleTexture(Texture::Sample(Texture::CHARTREUSE)));
                //                }
                //                float mtx[16];
                //                float scale[16];
                //                bx::mtxScale(scale, 1.5f);
                //                float translate[16];
                //                bx::mtxTranslate(translate, 0.0f, 0.0f, -10.0f);
                //                bx::mtxMul(mtx, translate, cube.mtx);
                //                bx::mtxMul(mtx, scale, mtx);
                //                Geometry::drawCube(mtx);
                //                bgfx::submit(m_id, m_branchingTests[m_iBranchingTest]);

                for (int i = 0; i < s_nTexture; ++i) {
                    if (cube.hasTexture[i]) {
                        bgfx::setTexture(i, m_texture[i], Texture::getSampleTexture(Texture::Sample(i + Texture::RED)));
                    } else {
                        bgfx::setTexture(i, m_texture[i], Texture::getSampleTexture(Texture::Sample::BLACK));
                    }
                }
#ifdef SMART
                iShader = cube.iShader;
            }
#endif
//            Geometry::drawCube(cube.mtx);
#ifdef DRAW_SPHERE
            Geometry::drawUVSphere(cube.mtx);
#else
            Geometry::drawCube(cube.mtx);
#endif
            //            Geometry::drawUVSphere(cube.mtx);
            //            bgfx::submit(m_id, m_branchingTests[m_iBranchingTest]);
            bgfx::submit(view.id, m_branchingTests[m_iBranchingTest]);
        }

    } else if (m_iBranchingTest == 2) {
        for (const auto& cube : m_cubes) {
            //            float additionalTexture[4] = { s_nAditionalTexture };
            //            bgfx::setUniform(m_nAdditionalTexture, additionalTexture);

            //            int iShader = 0;
            //            if (cube.hasTexture[0]) {
            //                //            bgfx::setTexture(0, m_diffuseTexture, Texture::m_sampleTextures[Texture::RED].textureHandle());
            //                bgfx::setTexture(0, m_diffuseTexture, Texture::getSampleTexture(Texture::RED));
            ////                ++iShader;
            //                //                bgfx::setUniform(m_hasDiffuseTexture, ones);
            //                //            } else {
            //                //                bgfx::setTexture(0, m_diffuseTexture, Texture::getSampleTexture(Texture::WHITE));
            //                //                bgfx::setUniform(m_diffuseColor, cube.diffuseColor);
            //                //                bgfx::setUniform(m_hasDiffuseTexture, zeros);
            //            }
            //            if (cube.hasTexture[1]) {
            //                //            bgfx::setTexture(1, m_specularTexture, Texture::m_sampleTextures[Texture::GREEN].textureHandle());
            //                bgfx::setTexture(1, m_specularTexture, Texture::getSampleTexture(Texture::GREEN));
            //                //                bgfx::setUniform(m_hasSpecularTexture, ones);
            ////                iShader += 2;
            //                //            } else {
            //                //                bgfx::setUniform(m_hasSpecularTexture, zeros);
            //                //                bgfx::setTexture(1, m_specularTexture, Texture::getSampleTexture(Texture::BLACK));
            //            }

            //            int jump = 4;
#ifdef SMART
            if (iShader != cube.iShader) {
#endif
                for (int i = 0; i < s_nTexture; ++i) {
                    if (cube.hasTexture[i]) {
                        bgfx::setTexture(i, m_texture[i], Texture::getSampleTexture(Texture::Sample(i + Texture::RED)));
                        //                    iShader += jump;
                        //                                    } else {
                        //                                        bgfx::setTexture(2 + i, m_texture[i], Texture::getSampleTexture(Texture::Sample::BLACK));
                        //                                        bgfx::setTexture(i, m_texture[i], Texture::getSampleTexture(Texture::Sample::BLACK));
                    }
                    //                jump *= 2;
                }
#ifdef SMART
                iShader = cube.iShader;
            }
#endif

#ifdef DRAW_SPHERE
            Geometry::drawUVSphere(cube.mtx);
#else
                Geometry::drawCube(cube.mtx);
#endif
            //            Geometry::drawCube(cube.mtx);
            //            Geometry::drawUVSphere(cube.mtx);
            //            assert(iShader < 4)
            bgfx::submit(view.id, m_branching3Tests[cube.iShader]);
            //            continue;
        }
        //        bgfx::submit(m_id, m_programs[m_shading]);
        //                bgfx::submit(m_id, m_branchingTests[m_iBranchingTest], 0, BGFX_DISCARD_NONE | BGFX_DISCARD_TEXTURE_SAMPLERS);
        //                bgfx::submit(m_id, m_branchingTests[m_iBranchingTest], 0, BGFX_DISCARD_NONE);
        //                bgfx::submit(m_id, m_branchingTests[m_iBranchingTest], 0, BGFX_DISCARD_ALL);
    }
}

void initCubeScene()
{

    //    const uint32_t nbCube = 64;
    //        const uint32_t nbCube = 256;
    //                const uint32_t nbCube = s_nTexture * 4;
    //        const uint32_t nbCube = s_nTexture;

    //        const int nArea = 2 + s_nAditionalTexture / 2;
    //        const int nArea = std::ceil(std::sqrt(s_nTexture));
    //        const int nArea = 1 + std::ceil(s_nTexture / 2.0);
    //        const int nArea = 2;
    //        const int nArea = 1 + std::ceil(s_nTexture / 2.0f);
    //        const int nArea = std::ceil(std::sqrt(std::pow(2, s_nTexture)));
    const int nIArea = std::pow(2, std::ceil(s_nTexture / 2.0f));
    //        const int nJArea = std::pow(2, s_nTexture / 2.0f);
    const int nJArea = nIArea / (1 + s_nTexture % 2);

    //        const int nArea = std::pow(2, s_nTexture / 2.0f);
    int iShader = 0;
    for (int iArea = 0; iArea < nIArea; ++iArea) {
        for (int jArea = 0; jArea < nJArea; ++jArea) {
            //                std::string hasAdditional = "N_TEXTURE=" + (std::to_string(s_nTexture));
            for (uint32_t iCube = 0; iCube < s_nbCubePerLine / nIArea; ++iCube) {
                const int i = iCube + (s_nbCubePerLine / nIArea) * iArea;
                for (uint32_t jCube = 0; jCube < s_nbCubePerLine / nJArea; ++jCube) {
                    const int j = jCube + (s_nbCubePerLine / nJArea) * jArea;
                    float mtx[16];
                    //                bgfx::setTexture(0, m_sDiffuse,
                    //                    Texture::m_sampleTextures[Texture::Sample((i + j) % 3 + Texture::RED)].textureHandle(), Texture::s_textureSamplerFlags);
                    //            bx::mtxIdentity(mtx);
                    bx::mtxIdentity(mtx);
                    //                bx::mtxRotateXY(mtx, s_iFrame * 0.021f, s_iFrame * 0.037f);
                    mtx[12] = -(float)s_nbCubePerLine / 2.0f * 3.0f + float(i) * 3.0f;
                    mtx[13] = -(float)s_nbCubePerLine / 2.0f * 3.0f + float(j) * 3.0f;
                    mtx[14] = 0.0f;

                    //                        const float color[] { 0.5f, 0.5f, 0.5f, 0.0f };
                    Cube cube;
                    memcpy(cube.mtx, mtx, 16 * sizeof(float));
                    //                        cube.hasDiffuseTexture = i > nbCube / nArea;
                    //                        cube.hasSpecularTexture = j > nbCube / nArea;
                    //                        memcpy(cube.diffuseColor, color, 4 * sizeof(float));

                    if (iShader < std::pow(2, s_nTexture)) {
                        //                        if (iShader < nArea * nArea) {
                        int temp = iShader;
                        for (int k = 0; k < s_nTexture; ++k) {
                            const int bit = temp % 2;

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

                    assert(m_cubes.size() < s_nbCubes);
                    m_cubes.push_back(cube);
                    //                Geometry::drawCube(mtx);
                    //                bgfx::submit(m_id, m_programs[m_shading]);
                }
            }

            //                ++iTex;
            //                m_branching3Tests[iShader] = loadProgram("branchingTest3", (hasAdditional).c_str());
            //                bgfx::frame();
            ++iShader;
        }
    }

#ifdef UNSORTED
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(m_cubes.begin(), m_cubes.end(), g);
#endif

    m_branchingTests[0] = Program::loadProgram("branchingTest1", ("N_TEXTURE=" + std::to_string(s_nTexture)).c_str());
    m_branchingTests[1] = Program::loadProgram("branchingTest2", ("N_TEXTURE=" + std::to_string(s_nTexture)).c_str());
    //        m_branchingTests[1] = loadProgram("branchingTest2", "");
    m_branchingTests[2] = Program::loadProgram("branchingTest3", "");
    //        m_branchingTests[2] = loadProgram("branchingTest3", ("N_TEXTURE=" + std::to_string(s_nTexture) + ";HAS_TEXTURE_0;HAS_TEXTURE_3;HAS_TEXTURE_5;HAS_TEXTURE_6;HAS_TEXTURE_7;HAS_TEXTURE_8").c_str());

    //        m_branching3Tests[0] = loadProgram("branchingTest3", ("N_ADDITIONAL_TEXTURE=" + std::to_string(s_nAditionalTexture)).c_str());
    //        m_branching3Tests[1] = loadProgram("branchingTest3", ("N_ADDITIONAL_TEXTURE=" + (std::to_string(s_nAditionalTexture)) + ";HAS_DIFFUSE_TEXTURE").c_str());
    //        m_branching3Tests[2] = loadProgram("branchingTest3", ("N_ADDITIONAL_TEXTURE=" + (std::to_string(s_nAditionalTexture)) + ";HAS_SPECULAR_TEXTURE").c_str());
    //        m_branching3Tests[3] = loadProgram("branchingTest3", ("N_ADDITIONAL_TEXTURE=" + (std::to_string(s_nAditionalTexture)) + ";HAS_DIFFUSE_TEXTURE;HAS_SPECULAR_TEXTURE").c_str());

    //        for (int i = 0; i < std::pow(2, 2 + s_nAditionalTexture); ++i) {
    //                        if (iShader < std::pow(2, s_nTexture)) {

    for (int i = 0; i < std::pow(2, s_nTexture); ++i) {
        std::string hasAdditional = "N_TEXTURE=" + (std::to_string(s_nTexture));
        int iTemp = i;
        for (int j = 0; j < s_nTexture; ++j) {
            int bit = iTemp % 2;

            //                        hasAdditional += ";HAS_TEXTURE_" + std::to_string(j) + "=" + std::to_string(bit);
            if (bit) {
                hasAdditional += ";HAS_TEXTURE_" + std::to_string(j);
            }

            iTemp /= 2;
        }
        //        std::cout << "[WINDOWSTATE] loadProgram branchingTest3 " << hasAdditional << std::endl;
        m_branching3Tests[i] = Program::loadProgram("branchingTest3", (hasAdditional).c_str());

        //            m_branching3Tests[i] = loadProgram("branchingTest3", "");
        //                                bgfx::frame();
        //                                if (i  % 100 == 0) {
        //                                    bgfx::frame();
        //                                }
    }

    //            for (int j =0; j < std::pow(2, i); ++j) {
    //            m_branching3Tests[4] = loadProgram("branchingTest3", ("N_ADDITIONAL_TEXTURE=" + (std::to_string(s_nAditionalTexture)) + ";HAS_DIFFUSE_TEXTURE;HAS_SPECULAR_TEXTURE;HAS_ADDITIONAL_TEXTURE_" + std::to_string(0)).c_str());

    //            m_branching3Tests[5] = loadProgram("branchingTest3", ("N_ADDITIONAL_TEXTURE=" + (std::to_string(s_nAditionalTexture)) + ";HAS_DIFFUSE_TEXTURE;HAS_SPECULAR_TEXTURE;HAS_ADDITIONAL_TEXTURE_" + std::to_string(1)).c_str());
    //            m_branching3Tests[5] = loadProgram("branchingTest3", ("N_ADDITIONAL_TEXTURE=" + (std::to_string(s_nAditionalTexture)) + ";HAS_DIFFUSE_TEXTURE;HAS_SPECULAR_TEXTURE;HAS_ADDITIONAL_TEXTURE_" + std::to_string(0) + ";HAS_ADDITIONAL_TEXTURE_" + std::to_string(1)).c_str());
    //            }
    //        }
}

void clearCubeScene()
{
    m_cubes.clear();
    bgfx::destroy(m_branchingTests[0]);
    bgfx::destroy(m_branchingTests[1]);
    bgfx::destroy(m_branchingTests[2]);
    for (int i = 0; i < std::pow(2, s_nTexture); ++i) {
        bgfx::destroy(m_branching3Tests[i]);
    }
}

} // entry
