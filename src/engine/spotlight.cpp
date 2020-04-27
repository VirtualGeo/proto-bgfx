#include "spotlight.h"


SpotLight::SpotLight(bx::Vec3 &&direction, bx::Vec3 &&position, float cutOff, float outerCutOff, float constant, float linear, float quadratic)
    : m_direction(std::move(direction))
    , m_position(std::move(position))
    , m_cutOff(cutOff)
    , m_outerCutOff(outerCutOff)
    , m_constant(constant)
    , m_linear(linear)
    , m_quadratic(quadratic)
    , m_data {m_ambient.x, m_ambient.y, m_ambient.z, m_cutOff,
              m_diffuse.x, m_diffuse.y, m_diffuse.z, m_outerCutOff,
              m_specular.x, m_specular.y, m_specular.z, m_constant,
              m_direction.x, m_direction.y, m_direction.z, m_linear,
              m_position.x, m_position.y, m_position.z, m_quadratic
              }
{

}

void SpotLight::updateLightShadowMaps()
{

}
