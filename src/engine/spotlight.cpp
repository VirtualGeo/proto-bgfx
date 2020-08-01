#include "spotlight.h"

#include <bgfx/bgfx.h>
#include <cassert>
#include <cstring>
#include <iostream>

//#include <entry/entry.h>
#include <entry/windowstate.h>
#include <engine/geometry.h>

bx::Vec3 g_up = { 0.0, 1.0, 0.0 };

static const int s_shadowMapSize = 2048;
size_t SpotLight::s_nSpotLight = 0;

bgfx::UniformHandle SpotLight::s_uSpotLightsUH = BGFX_INVALID_HANDLE;
static bgfx::UniformHandle s_uLightSpaceMatrixUH = BGFX_INVALID_HANDLE;
static bgfx::UniformHandle s_sShadowMapUH = BGFX_INVALID_HANDLE;

SpotLight::SpotLight(bx::Vec3 direction, bx::Vec3 position, float cutOff, float outerCutOff, float constant, float linear, float quadratic)
    : m_direction(std::move(direction))
    , m_position(std::move(position))
    , m_cutOff(cutOff)
    , m_outerCutOff(outerCutOff)
    , m_constant(constant)
    , m_linear(linear)
    , m_quadratic(quadratic)
{
    updateData();

    if (s_nSpotLight == 0) {
        //        assert(! bgfx::isValid(m_sShadowMap));
        s_uSpotLightsUH = bgfx::createUniform("u_spotLights", bgfx::UniformType::Vec4, s_numSpotLightMax * s_num_vec4_spotLight);
        s_uLightSpaceMatrixUH = bgfx::createUniform("u_lightSpaceMatrix", bgfx::UniformType::Mat4);
        s_sShadowMapUH = bgfx::createUniform("s_shadowMap", bgfx::UniformType::Sampler);
        //        m_sShadowMap = bgfx::createUniform("s_shadowMap", bgfx::UniformType::Sampler);
    }
    //        m_sShadowMap = bgfx::createUniform("s_shadowMap", bgfx::UniformType::Sampler);
    m_sShadowMapUH = bgfx::createUniform((std::string("s_shadowMap_spotLight_") + std::to_string(s_nSpotLight)).c_str(), bgfx::UniformType::Sampler);

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

    ++s_nSpotLight;
}

SpotLight::SpotLight(SpotLight &&spotLight)
{
#ifdef DEBUG
    std::cout << "\033[34m";
    std::cout << "[CameraFps] " << this << "  right moving from " << &spotLight << std::endl;
    std::cout << "\033[0m" << std::endl;
#endif
    assert(false);
}

SpotLight::~SpotLight()
{
    bgfx::destroy(m_sShadowMapUH);
    bgfx::destroy(m_shadowMapTH);
    bgfx::destroy(m_shadowMapFBH);

    --s_nSpotLight;
    if (s_nSpotLight == 0) {
        bgfx::destroy(s_uSpotLightsUH);
        bgfx::destroy(s_uLightSpaceMatrixUH);
        bgfx::destroy(s_sShadowMapUH);
    }
#ifdef DEBUG
    std::cout << "\033[31m";
    std::cout << "[SpotLight] " << this << " deleted" << std::endl;
    std::cout << "\033[0m";
#endif
}

void SpotLight::updateLightShadowMaps(int viewId)
{
    //    bgfx::setViewRect(viewId, 0, 0, m_shadowMapSize, m_shadowMapSize);
    bgfx::setViewRect(viewId, 0, 0, s_shadowMapSize, s_shadowMapSize);
    bgfx::setViewFrameBuffer(viewId, m_shadowMapFBH);

    //    bx::mtxRotateXY(m_lightSpaceMatrix, entry::s_iFrame, entry::s_iFrame);

    bgfx::setUniform(s_uLightSpaceMatrixUH, m_lightSpaceMatrix);

    bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFF00FFFF);
}

void SpotLight::updatePos(bx::Vec3 position)
{
    m_position = position;

    float lightView[16];
    bx::mtxLookAt(lightView, m_position, bx::add(m_position, m_direction), g_up); // question why y = -1

    float lightProj[16];
    bx::mtxProj(lightProj, 2 * m_outerCutOff, 1.0f, m_near, m_far, bgfx::getCaps()->homogeneousDepth);

    bx::mtxMul(m_lightSpaceMatrix, lightView, lightProj);

    assert(36 == 4 * s_num_vec4_spotLight);
    float newData[3] = { m_position.x, m_position.y, m_position.z };
    memcpy(&m_data[16], newData, 3 * sizeof(float));
    memcpy(&m_data[20], m_lightSpaceMatrix, 16 * sizeof(float));
}

void SpotLight::updateDirection(bx::Vec3 direction)
{
    m_direction = direction;

    float lightView[16];
    bx::mtxLookAt(lightView, m_position, bx::add(m_position, m_direction), g_up); // question why y = -1

    float lightProj[16];
    bx::mtxProj(lightProj, 2 * m_outerCutOff, 1.0f, m_near, m_far, bgfx::getCaps()->homogeneousDepth);

    bx::mtxMul(m_lightSpaceMatrix, lightView, lightProj);

    assert(36 == 4 * s_num_vec4_spotLight);
    float newData[3] = { m_direction.x, m_direction.y, m_direction.z };
    memcpy(&m_data[12], newData, 3 * sizeof(float));
    memcpy(&m_data[20], m_lightSpaceMatrix, 16 * sizeof(float));
}

void SpotLight::updateData()
{
    float lightView[16];
    bx::mtxLookAt(lightView, m_position, bx::add(m_position, m_direction), g_up); // question why y = -1

    float lightProj[16];
    bx::mtxProj(lightProj, 2 * m_outerCutOff, 1.0f, m_near, m_far, bgfx::getCaps()->homogeneousDepth);

    bx::mtxMul(m_lightSpaceMatrix, lightView, lightProj);

    assert(36 == 4 * s_num_vec4_spotLight);
    float newData[20] = { m_ambient.x, m_ambient.y, m_ambient.z, bx::cos(bx::toRad(m_cutOff)),
        m_diffuse.x, m_diffuse.y, m_diffuse.z, bx::cos(bx::toRad(m_outerCutOff)),
        m_specular.x, m_specular.y, m_specular.z, m_constant,
        m_direction.x, m_direction.y, m_direction.z, m_linear,
        m_position.x, m_position.y, m_position.z, m_quadratic };
    memcpy(m_data, newData, 20 * sizeof(float));
    memcpy(&m_data[20], m_lightSpaceMatrix, 16 * sizeof(float));
}


void SpotLight::drawDebug()
{
    int viewId = VIEW_ID_START_DEBUG_SHADOW + m_id;

    bgfx::setViewRect(viewId, 50 + m_id *210, 200, 200, 200);
    bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x00FF00FF);

    bgfx::setTexture(3, s_sShadowMapUH, m_shadowMapTH);
//    bgfx::setTexture(3, Program::m_sShadowMap, Texture::getSampleTexture(Texture::CHECKER_BOARD));
    Geometry::drawQuad();

    bgfx::submit(viewId, Program::m_programs[DEBUG_QUAD]);
}
