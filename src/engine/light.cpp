#include "light.h"
#include <cassert>

#include <iostream>
#include "program.h"
#include "geometry.h"
#include <entry/windowstate.h>

static size_t s_nLight = 0;
static bgfx::UniformHandle s_sShadowMapUH = BGFX_INVALID_HANDLE;

//size_t s_counter;

Light::Light(bx::Vec3 ambient, bx::Vec3 diffuse, bx::Vec3 specular)
    : m_ambient(ambient)
    , m_diffuse(diffuse)
    , m_specular(specular)
    , m_id(s_nLight)
{

    if (s_nLight == 0) {
        s_sShadowMapUH = bgfx::createUniform("s_shadowMap", bgfx::UniformType::Sampler);
    }
    ++s_nLight;
}

Light::Light(Light&& light)
    : m_ambient(light.m_ambient)
    , m_diffuse(light.m_diffuse)
    , m_specular(light.m_specular)

    , m_id(light.m_id)
{
    light.m_last = false;
    std::cout << "\033[34m";
    std::cout << "[Light] " << this << " right moving from " << &light << std::endl;
    std::cout << "\033[0m";
    std::cout << "nbLight : " << s_nLight << std::endl;
    assert(false);
}

Light::~Light()
{
    if (m_last) {
        --s_nLight;
    }
}
