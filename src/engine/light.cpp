#include "light.h"
#include <cassert>

#include <iostream>
#include "program.h"
#include "geometry.h"

int Light::m_nbLight = 0;

Light::Light(bx::Vec3 ambient, bx::Vec3 diffuse, bx::Vec3 specular)
    : m_ambient(ambient)
    , m_diffuse(diffuse)
    , m_specular(specular)
    , m_id(m_nbLight)
{
    ++m_nbLight;
    //    bgfx::TextureHandle shadowMapTexture;
    m_sShadowMap = bgfx::createUniform("s_shadowMap", bgfx::UniformType::Sampler);

    //    bgfx::TextureHandle fbtextures[] = {
    //        bgfx::createTexture2D(
    //          m_shadowMapSize
    //        , m_shadowMapSize
    //        , false
    //        , 1
    //        , bgfx::TextureFormat::D16
    //        , BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LEQUAL
    //        ),
    //    };

    //    m_shadowMapTexture = fbtextures[0];
    //    m_shadowMapFB = bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, true);

    //    assert(bgfx::isValid(m_shadowMapFB));

//    bgfx::TextureHandle fbtextures[] = {
//    m_shadowMapTexture = {
//        bgfx::createTexture2D(
//          m_shadowMapSize
//        , m_shadowMapSize
//        , false
//        , 1
//        , bgfx::TextureFormat::D16
//        , BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LEQUAL
//        ),
//    };

////    m_shadowMapFB.idx = bgfx::kInvalidHandle;
////    m_shadowMapTexture = fbtextures[0];
//    m_shadowMapFB = bgfx::createFrameBuffer(1, &m_shadowMapTexture, false);

////    bgfx::setName(m_shadowMapFB, "shadowMapFB");

//    assert(bgfx::isValid(m_shadowMapFB));
//    assert(bgfx::isValid(m_shadowMapTexture));
    bgfx::TextureHandle fbtextures[] =
    {
        bgfx::createTexture2D(
              m_shadowMapSize
            , m_shadowMapSize
            , false
            , 1
            , bgfx::TextureFormat::BGRA8
            , BGFX_TEXTURE_RT
            ),
        bgfx::createTexture2D(
              m_shadowMapSize
            , m_shadowMapSize
            , false
            , 1
            , bgfx::TextureFormat::D16
            , BGFX_TEXTURE_RT_WRITE_ONLY
            ),
    };
    m_shadowMapTexture = fbtextures[0];
    m_shadowMapFB = bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, true);



}

Light::Light(Light&& light)
    : m_ambient(light.m_ambient)
    , m_diffuse(light.m_diffuse)
    , m_specular(light.m_specular)

    , m_shadowMapFB(light.m_shadowMapFB)
    , m_shadowMapTexture(light.m_shadowMapTexture)
//    , m_shadowMapSize(light.m_shadowMapSize)
    , m_id(m_nbLight)
{
    light.m_last = false;
    //#ifdef DEBUG
    std::cout << "\033[34m";
    std::cout << "[Light] " << this << " right moving from " << &light << std::endl;
    std::cout << "\033[0m";
    //#endif
}

Light::~Light()
{
    if (m_last) {
        bgfx::destroy(m_shadowMapFB);
        bgfx::destroy(m_shadowMapTexture);
        --m_nbLight;
//        m_shadowMapFB = BGFX_INVALID_HANDLE;
//        m_shadowMapFB.idx = bgfx::kInvalidHandle;
    }
}

void Light::drawDebug()
{
    int viewId = VIEWID_SHADOW + m_id;
//    int viewId = 21;
//    std::cout << viewId << std::endl;

    bgfx::setViewRect(viewId, 50 + m_id *200, 200, 200, 200);
    bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x00FF00FF);
//    bgfx::touch(1);
    assert(bgfx::isValid(m_shadowMapTexture));
//    bgfx::setTexture(3, Program::m_sShadowMap, m_shadowMapTexture);
    bgfx::setTexture(3, m_sShadowMap, m_shadowMapTexture);
//    bgfx::setTexture(3, Program::m_sShadowMap, Texture::m_sampleTextures[Texture::CHECKER_BOARD].textureHandle(), Texture::s_textureSamplerFlags);
//    bgfx::submit(1, Program::m_programs[DEBUG_QUAD]);
    Geometry::drawQuad();

    bgfx::submit(viewId, Program::m_programs[DEBUG_QUAD]);
//    bgfx::touch(viewId);
//    bgfx::frame();
}
