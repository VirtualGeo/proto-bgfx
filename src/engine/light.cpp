#include "light.h"
#include <cassert>

#include <iostream>

Light::Light(bx::Vec3 ambient, bx::Vec3 diffuse, bx::Vec3 specular)
    : m_ambient(ambient)
    , m_diffuse(diffuse)
    , m_specular(specular)
{
    //    bgfx::TextureHandle shadowMapTexture;

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

}

Light::Light(Light&& light)
    : m_ambient(light.m_ambient)
    , m_diffuse(light.m_diffuse)
    , m_specular(light.m_specular)
//    , m_shadowMapFB(light.m_shadowMapFB)
//    , m_shadowMapTexture(light.m_shadowMapTexture)
//    , m_shadowMapSize(light.m_shadowMapSize)
{
//    light.m_last = false;
    //#ifdef DEBUG
    std::cout << "\033[34m";
    std::cout << "[Light] " << this << " right moving from " << &light << std::endl;
    std::cout << "\033[0m";
    //#endif
}

Light::~Light()
{
//    if (m_last) {
//        bgfx::destroy(m_shadowMapFB);
//        m_shadowMapFB = BGFX_INVALID_HANDLE;
////        m_shadowMapFB.idx = bgfx::kInvalidHandle;
//    }
}
