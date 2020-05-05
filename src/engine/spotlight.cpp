#include "spotlight.h"

#include <bgfx/bgfx.h>
#include <cassert>
#include <iostream>
#include <cstring>

SpotLight::SpotLight(bx::Vec3 direction, bx::Vec3 position, float cutOff, float outerCutOff, float constant, float linear, float quadratic)
    : m_direction(std::move(direction))
    , m_position(std::move(position))
    , m_cutOff(cutOff)
    , m_outerCutOff(outerCutOff)
    , m_constant(constant)
    , m_linear(linear)
    , m_quadratic(quadratic)
//    , m_data {m_ambient.x, m_ambient.y, m_ambient.z, m_cutOff,
//              m_diffuse.x, m_diffuse.y, m_diffuse.z, m_outerCutOff,
//              m_specular.x, m_specular.y, m_specular.z, m_constant,
//              m_direction.x, m_direction.y, m_direction.z, m_linear,
//              m_position.x, m_position.y, m_position.z, m_quadratic
//              }
//    , Light()
{
//    updatePos(position);
//    updateDirection(direction);
    updateData();
}

void SpotLight::updateLightShadowMaps(int viewId)
{
//    int id = 3;
//    assert(bgfx::isValid(m_shadowMapFB));

//    bgfx::setViewRect(id, 500, 50, 400, 400);

//    bgfx::setViewFrameBuffer(id, m_shadowMapFB);

    bgfx::setViewRect(viewId, 50 + 210 * m_id, 200, 200, 200);
//    bgfx::setViewRect(viewId, 0, 0, m_shadowMapSize, m_shadowMapSize);
//    bgfx::setViewFrameBuffer(id, Program::m_shadowMapFB);
//    bgfx::setViewFrameBuffer(viewId, m_shadowMapFB);

//    float view[16];
//    bx::mtxLookAt(view, m_position, bx::add(m_position, m_direction));

//    float proj[16];
//    bx::mtxProj(proj, 45.0f, 1.0f, 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
//    bgfx::setViewTransform(id, view, proj);

    bgfx::setUniform(Program::m_uLightSpaceMatrix, m_lightSpaceMatrix);


    bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFF00FFFF);


//    drawDebug();

//    bgfx::touch(id);

//    bgfx::frame();
    //    bgfx::frame();
}

void SpotLight::updatePos(bx::Vec3 position)
{
    m_position = position;

    float lightView[16];
    bx::mtxLookAt(lightView, m_position, bx::add(m_position, m_direction), {0.0f, -1.0f, 0.0f}); // question why y = -1

    float lightProj[16];
    bx::mtxProj(lightProj, 45.0f, 1.0f, 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);

//    float mtxShadow[16];
//    const bgfx::Caps * caps = bgfx::getCaps();
//    const float sy = caps->originBottomLeft ? 0.5f : -0.5f;
//    const float sz = caps->homogeneousDepth ? 0.5f :  1.0f;
//    const float tz = caps->homogeneousDepth ? 0.5f :  0.0f;
//    const float mtxCrop[16] =
//    {
//        0.5f, 0.0f, 0.0f, 0.0f,
//        0.0f,   sy, 0.0f, 0.0f,
//        0.0f, 0.0f, sz,   0.0f,
//        0.5f, 0.5f, tz,   1.0f,
//    };

//    float mtxTmp[16];
//    bx::mtxMul(mtxTmp,    lightProj, mtxCrop);
//    bx::mtxMul(m_lightSpaceMatrix, lightView, mtxTmp);

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
    bx::mtxLookAt(lightView, m_position, bx::add(m_position, m_direction), {0.0f, -1.0f, 0.0f}); // question why y = -1

    float lightProj[16];
    bx::mtxProj(lightProj, 45.0f, 1.0f, 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);

//    float mtxShadow[16];
//    const bgfx::Caps * caps = bgfx::getCaps();
//    const float sy = caps->originBottomLeft ? 0.5f : -0.5f;
//    const float sz = caps->homogeneousDepth ? 0.5f :  1.0f;
//    const float tz = caps->homogeneousDepth ? 0.5f :  0.0f;
//    const float mtxCrop[16] =
//    {
//        0.5f, 0.0f, 0.0f, 0.0f,
//        0.0f,   sy, 0.0f, 0.0f,
//        0.0f, 0.0f, sz,   0.0f,
//        0.5f, 0.5f, tz,   1.0f,
//    };

//    float mtxTmp[16];
//    bx::mtxMul(mtxTmp,    lightProj, mtxCrop);
//    bx::mtxMul(m_lightSpaceMatrix, lightView, mtxTmp);

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
    bx::mtxLookAt(lightView, m_position, bx::add(m_position, m_direction), {0.0f, -1.0f, 0.0f}); // question why y = -1

    float lightProj[16];
    bx::mtxProj(lightProj, 45.0f, 1.0f, 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);

//    float mtxShadow[16];
//    const bgfx::Caps * caps = bgfx::getCaps();
//    const float sy = caps->originBottomLeft ? 0.5f : -0.5f;
//    const float sz = caps->homogeneousDepth ? 0.5f :  1.0f;
//    const float tz = caps->homogeneousDepth ? 0.5f :  0.0f;
//    const float mtxCrop[16] =
//    {
//        0.5f, 0.0f, 0.0f, 0.0f,
//        0.0f,   sy, 0.0f, 0.0f,
//        0.0f, 0.0f, sz,   0.0f,
//        0.5f, 0.5f, tz,   1.0f,
//    };

//    float mtxTmp[16];
//    bx::mtxMul(mtxTmp,    lightProj, mtxCrop);
//    bx::mtxMul(m_lightSpaceMatrix, lightView, mtxTmp);

    bx::mtxMul(m_lightSpaceMatrix, lightView, lightProj);



    assert(36 == 4 * Program::s_num_vec4_spotLight);
    float newData[20] = {m_ambient.x, m_ambient.y, m_ambient.z, m_cutOff,
              m_diffuse.x, m_diffuse.y, m_diffuse.z, m_outerCutOff,
              m_specular.x, m_specular.y, m_specular.z, m_constant,
              m_direction.x, m_direction.y, m_direction.z, m_linear,
              m_position.x, m_position.y, m_position.z, m_quadratic
              };
    memcpy(m_data, newData, 20 * sizeof (float));
    memcpy(&m_data[20], m_lightSpaceMatrix, 16 * sizeof(float));
}



//void SpotLight::updateData()
//{

//}
