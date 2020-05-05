#include "dirLight.h"

DirLight::DirLight(bx::Vec3 direction)
//    : Light() // question : std_move(ambient) ?
    : m_direction(bx::normalize(direction))
    , m_data {m_ambient.x, m_ambient.y, m_ambient.z, 0.0f,
              m_diffuse.x, m_diffuse.y, m_diffuse.z, 0.0f,
              m_specular.x, m_specular.y, m_specular.z, 0.0f,
              m_direction.x, m_direction.y, m_direction.z, 0.0f,
              }
{

}

void DirLight::updateLightShadowMaps(int viewId)
{

}

