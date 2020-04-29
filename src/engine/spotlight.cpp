#include "spotlight.h"

#include <bgfx/bgfx.h>
#include <cassert>
#include "program.h"

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
    int id = 10;
//    assert(bgfx::isValid(m_shadowMapFB));
//    bgfx::setViewRect(id, 0, 0, m_shadowMapSize, m_shadowMapSize);
//    bgfx::setViewFrameBuffer(id, m_shadowMapFB);
    bgfx::setViewRect(id, 0, 0, Program::m_shadowMapSize, Program::m_shadowMapSize);
    bgfx::setViewFrameBuffer(id, Program::m_shadowMapFB);

    float view[16];
    bx::mtxLookAt(view, m_position, bx::add(m_position, m_direction));

    float proj[16];
    bx::mtxProj(proj, 45.0f, 1.0f, 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
    bgfx::setViewTransform(id, view, proj);

    bgfx::setViewClear(id, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFF0000FF);


//    bgfx::touch(id);

//    bgfx::frame();
//    bgfx::frame();
}
