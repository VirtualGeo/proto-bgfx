#include "dirLight.h"

DirLight::DirLight(bx::Vec3 direction)
//    : Light() // question : std_move(ambient) ?
    : m_direction(direction)
{

}
