#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include <vector>
#include "light.h"
#include "program.h"

class SpotLight : public Light
{
public:
    SpotLight(bx::Vec3 direction, bx::Vec3 position, float cutOff =17.0f, float outerCutOff = 20.0f, float constant = 1.0, float linear = 0.09, float quadratic = 0.032);
    SpotLight(const SpotLight & spotLight) = delete;
    SpotLight(SpotLight && spotLight);
    ~SpotLight();

//private:
//protected:
    void updateLightShadowMaps(int viewId) override;
//    void updateData();
    void updatePos(bx::Vec3 position);
    void updateDirection(bx::Vec3 direction);
    void updateData();

    void drawDebug() override;


    bx::Vec3 m_direction;
    bx::Vec3 m_position;
    float m_cutOff;
    float m_outerCutOff;
    float m_constant;
    float m_linear;
    float m_quadratic;

    float m_lightSpaceMatrix[16];

    static constexpr unsigned int s_num_vec4_spotLight = 9;
    static constexpr unsigned int s_numSpotLightMax = 2;
    static constexpr unsigned int s_spotLightSizeMax = s_num_vec4_spotLight * s_numSpotLightMax * 4;

    static bgfx::UniformHandle s_uSpotLightsUH;
    float m_data[4 * s_num_vec4_spotLight];

    static size_t s_nSpotLight;

//private:
    float m_near = 0.01f;
    float m_far = 100.0f;

    bgfx::UniformHandle m_sShadowMapUH = BGFX_INVALID_HANDLE;
    bgfx::FrameBufferHandle m_shadowMapFBH = BGFX_INVALID_HANDLE;
    bgfx::TextureHandle m_shadowMapTH = BGFX_INVALID_HANDLE;


};

using SpotLights = std::vector<SpotLight>;

#endif // SPOTLIGHT_H
