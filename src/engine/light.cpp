#include "light.h"
#include <cassert>

#include <iostream>
#include "program.h"
#include "geometry.h"

int Light::m_nbLight = 0;

Light::Light(bx::Vec3 ambient, bx::Vec3 diffuse, bx::Vec3 specular)
    : m_ambient(ambient)
    , m_diffuse(diffuse)
    , m_specular(specular)
    , m_id(m_nbLight)
{
    ++m_nbLight;
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
    std::cout << "nbLight : " << m_nbLight << std::endl;
}

Light::~Light()
{
    if (m_last) {
        --m_nbLight;
    }
}

void Light::drawDebug()
{
    int viewId = VIEWID_SHADOW + m_id;

    bgfx::setViewRect(viewId, 50 + m_id *210, 200, 200, 200);
    bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x00FF00FF);
    bgfx::setTexture(3, Program::m_sShadowMap, Program::m_shadowMapTexture[m_id]);
//    bgfx::setTexture(3, Program::m_sShadowMap, Texture::getSampleTexture(Texture::CHECKER_BOARD));
    Geometry::drawQuad();

    bgfx::submit(viewId, Program::m_programs[DEBUG_QUAD]);
}
