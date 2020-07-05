#include "spotlight.h"

#include <bgfx/bgfx.h>
#include <cassert>
#include <iostream>
#include <cstring>

//#include <entry/entry.h>

bx::Vec3 g_up = {0.0, 1.0, 0.0};

SpotLight::SpotLight(bx::Vec3 direction, bx::Vec3 position, float cutOff, float outerCutOff, float constant, float linear, float quadratic)
    : m_direction(std::move(direction))
    , m_position(std::move(position))
    , m_cutOff(cutOff)
    , m_outerCutOff(outerCutOff)
    , m_constant(constant)
    , m_linear(linear)
    , m_quadratic(quadratic)
{
    updateData();
}

void SpotLight::updateLightShadowMaps(int viewId)
{
//    bgfx::setViewRect(viewId, 0, 0, m_shadowMapSize, m_shadowMapSize);
    bgfx::setViewRect(viewId, 0, 0, Program::m_shadowMapSize, Program::m_shadowMapSize);
    bgfx::setViewFrameBuffer(viewId, Program::m_shadowMapFB[m_id]);

//    bx::mtxRotateXY(m_lightSpaceMatrix, entry::s_iFrame, entry::s_iFrame);

    bgfx::setUniform(Program::m_uLightSpaceMatrix, m_lightSpaceMatrix);


    bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFF00FFFF);


}

void SpotLight::updatePos(bx::Vec3 position)
{
    m_position = position;

    float lightView[16];
    bx::mtxLookAt(lightView, m_position, bx::add(m_position, m_direction), g_up); // question why y = -1

    float lightProj[16];
    bx::mtxProj(lightProj, 2 * m_outerCutOff, 1.0f, m_near, m_far, bgfx::getCaps()->homogeneousDepth);


    bx::mtxMul(m_lightSpaceMatrix, lightView, lightProj);



    assert(36 == 4 * Program::s_num_vec4_spotLight);
    float newData[3] = {m_position.x, m_position.y, m_position.z
              };
    memcpy(&m_data[16], newData, 3 * sizeof (float));
    memcpy(&m_data[20], m_lightSpaceMatrix, 16 * sizeof(float));
}

void SpotLight::updateDirection(bx::Vec3 direction)
{
    m_direction = direction;

    float lightView[16];
    bx::mtxLookAt(lightView, m_position, bx::add(m_position, m_direction), g_up); // question why y = -1

    float lightProj[16];
    bx::mtxProj(lightProj, 2 * m_outerCutOff, 1.0f, m_near, m_far, bgfx::getCaps()->homogeneousDepth);


    bx::mtxMul(m_lightSpaceMatrix, lightView, lightProj);



    assert(36 == 4 * Program::s_num_vec4_spotLight);
    float newData[3] = {m_direction.x, m_direction.y, m_direction.z
              };
    memcpy(&m_data[12], newData, 3 * sizeof (float));
    memcpy(&m_data[20], m_lightSpaceMatrix, 16 * sizeof(float));

}

void SpotLight::updateData()
{
    float lightView[16];
    bx::mtxLookAt(lightView, m_position, bx::add(m_position, m_direction), g_up); // question why y = -1

    float lightProj[16];
    bx::mtxProj(lightProj, 2 * m_outerCutOff, 1.0f, m_near, m_far, bgfx::getCaps()->homogeneousDepth);


    bx::mtxMul(m_lightSpaceMatrix, lightView, lightProj);



    assert(36 == 4 * Program::s_num_vec4_spotLight);
    float newData[20] = {m_ambient.x, m_ambient.y, m_ambient.z, bx::cos(bx::toRad(m_cutOff)),
              m_diffuse.x, m_diffuse.y, m_diffuse.z, bx::cos(bx::toRad(m_outerCutOff)),
              m_specular.x, m_specular.y, m_specular.z, m_constant,
              m_direction.x, m_direction.y, m_direction.z, m_linear,
              m_position.x, m_position.y, m_position.z, m_quadratic
              };
    memcpy(m_data, newData, 20 * sizeof (float));
    memcpy(&m_data[20], m_lightSpaceMatrix, 16 * sizeof(float));
}

