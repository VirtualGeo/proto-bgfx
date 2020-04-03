#include "light.h"


Light::Light(bx::Vec3 ambient, bx::Vec3 diffuse, bx::Vec3 specular)
    : m_ambient(ambient)
    , m_diffuse(diffuse)
    , m_specular(specular)
{

}
