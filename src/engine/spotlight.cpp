#include "spotlight.h"

SpotLight::SpotLight(bx::Vec3 &&direction)
    : m_direction(std::move(direction))
{

}
