#include "dirLight.h"
#include <engine/program.h>

#include <cassert>
#include <entry/windowstate.h>
#include <engine/geometry.h>

size_t DirLight::s_nDirLight = 0;
static const int s_shadowMapSize = 2048;

bgfx::UniformHandle DirLight::s_uDirLightsUH = BGFX_INVALID_HANDLE;
static bgfx::UniformHandle s_uLightSpaceMatrixUH = BGFX_INVALID_HANDLE;
static bgfx::UniformHandle s_sShadowMapUH = BGFX_INVALID_HANDLE;

DirLight::DirLight(bx::Vec3 direction)
//    : Light() // question : std_move(ambient) ?
    : m_direction(bx::normalize(direction))
    , m_data {m_ambient.x, m_ambient.y, m_ambient.z, 0.0f,
              m_diffuse.x, m_diffuse.y, m_diffuse.z, 0.0f,
              m_specular.x, m_specular.y, m_specular.z, 0.0f,
              m_direction.x, m_direction.y, m_direction.z, 0.0f,
              }
{

    const bx::Vec3 center {0.0f};
    const float radius = 10.0f;

    bx::Vec3 eye = bx::sub(center, bx::mul(direction, radius));
    float lightView[16];
    bx::mtxLookAt(lightView, eye, center);

    float lightProj[16];
//    bx::mtxProj(lightProj, 2 * m_outerCutOff, 1.0f, m_near, m_far, bgfx::getCaps()->homogeneousDepth);
    bx::mtxOrtho(lightProj, -radius, radius, -radius, radius, 0.1f, 2 * radius, 0.0f, bgfx::getCaps()->homogeneousDepth);

    bx::mtxMul(m_lightSpaceMatrix, lightView, lightProj);


    if (s_nDirLight == 0) {
        //        assert(! bgfx::isValid(m_sShadowMap));
        s_uDirLightsUH = bgfx::createUniform("u_dirLights", bgfx::UniformType::Vec4, s_numDirLightMax * s_num_vec4_dirLight);
        s_uLightSpaceMatrixUH = bgfx::createUniform("u_lightSpaceMatrix", bgfx::UniformType::Mat4);
        s_sShadowMapUH = bgfx::createUniform("s_shadowMap", bgfx::UniformType::Sampler);
        //        m_sShadowMap = bgfx::createUniform("s_shadowMap", bgfx::UniformType::Sampler);
    }
    //        m_sShadowMap = bgfx::createUniform("s_shadowMap", bgfx::UniformType::Sampler);
    m_sShadowMapUH = bgfx::createUniform((std::string("s_shadowMap_dirLight_") + std::to_string(s_nDirLight)).c_str(), bgfx::UniformType::Sampler);

    bgfx::TextureHandle fbtextures[] = {
        bgfx::createTexture2D(
            s_shadowMapSize,
            s_shadowMapSize,
            false,
            1,
            //                bgfx::TextureFormat::BGRA32,
            bgfx::TextureFormat::RGBA16,
            BGFX_TEXTURE_RT),

        bgfx::createTexture2D(
            s_shadowMapSize,
            s_shadowMapSize,
            false,
            1,
            bgfx::TextureFormat::D16,
            //                bgfx::TextureFormat::D32F,
            BGFX_TEXTURE_RT_WRITE_ONLY),
    };
    m_shadowMapTH = fbtextures[0];
    assert(BX_COUNTOF(fbtextures) == 2);
    m_shadowMapFBH = bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, true);


    ++s_nDirLight;
}

DirLight::~DirLight()
{
    bgfx::destroy(m_sShadowMapUH);
    bgfx::destroy(m_shadowMapTH);
    bgfx::destroy(m_shadowMapFBH);

    --s_nDirLight;
    if (s_nDirLight == 0) {
        bgfx::destroy(s_uDirLightsUH);
        bgfx::destroy(s_uLightSpaceMatrixUH);
        bgfx::destroy(s_sShadowMapUH);
    }
}

void DirLight::updateLightShadowMaps(int viewId)
{
//    bgfx::setViewRect(viewId, 0, 0, m_shadowMapSize, m_shadowMapSize);
    bgfx::setViewRect(viewId, 0, 0, s_shadowMapSize, s_shadowMapSize);
    bgfx::setViewFrameBuffer(viewId, m_shadowMapFBH);

//    bx::mtxRotateXY(m_lightSpaceMatrix, entry::s_iFrame, entry::s_iFrame);

    bgfx::setUniform(s_uLightSpaceMatrixUH, m_lightSpaceMatrix);


    bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFF00FFFF);
}


void DirLight::drawDebug()
{
    int viewId = VIEW_ID_START_DEBUG_SHADOW + m_id;

    bgfx::setViewRect(viewId, 50 + m_id *210, 200, 200, 200);
    bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x00FF00FF);

    bgfx::setTexture(3, s_sShadowMapUH, m_shadowMapTH);
//    bgfx::setTexture(3, Program::m_sShadowMap, Texture::getSampleTexture(Texture::CHECKER_BOARD));
    Geometry::drawQuad();

    bgfx::submit(viewId, Program::m_programs[DEBUG_QUAD]);
}
