#include "pointlight.h"

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

}

void PointLight::updateLightShadowMaps()
{

}
