#include "pointlight.h"

size_t PointLight::s_nPointLight = 0;
static const int s_shadowMapSize = 2048;
//static bgfx::UniformHandle s_sShadowMapUH = BGFX_INVALID_HANDLE;

PointLight::PointLight(bx::Vec3 && position, float constant, float linear, float quadratic)
    : m_position(std::move(position))
    , m_constant(constant)
    , m_linear(linear)
    , m_quadratic(quadratic)
    , m_data {m_ambient.x, m_ambient.y, m_ambient.z, 0.0f,
              m_diffuse.x, m_diffuse.y, m_diffuse.z, m_constant,
              m_specular.x, m_specular.y, m_specular.z, m_linear,
              m_position.x, m_position.y, m_position.z, m_quadratic
              }
//    , Light(bx::Vec3(0.0f), bx::Vec3(0.8f), bx::Vec3(1.0f))
{

    ++s_nPointLight;
}

PointLight::~PointLight()
{

    --s_nPointLight;
}

void PointLight::updateLightShadowMaps(int viewId)
{

}


void PointLight::drawDebug()
{
//    int viewId = VIEW_ID_START_DEBUG_SHADOW + m_id;

//    bgfx::setViewRect(viewId, 50 + m_id *210, 200, 200, 200);
//    bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x00FF00FF);

//    bgfx::setTexture(3, s_sShadowMapUH, m_shadowMapTH);
////    bgfx::setTexture(3, Program::m_sShadowMap, Texture::getSampleTexture(Texture::CHECKER_BOARD));
//    Geometry::drawQuad();

//    bgfx::submit(viewId, Program::m_programs[DEBUG_QUAD]);
}
