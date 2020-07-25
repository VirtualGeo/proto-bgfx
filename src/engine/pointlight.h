#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "light.h"
#include <vector>

class PointLight : public Light
{
public:
//    SpotLight(bx::Vec3 && direction, bx::Vec3 && position, float cutOff =0.97,
//              float outerCutOff = 0.95, float constant = 1.0, float linear = 0.09,
//              float quadratic = 0.032);
    PointLight(bx::Vec3 &&position, float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032);
    ~PointLight();

    void updateLightShadowMaps(int viewId) override;
    void drawDebug() override;

    bx::Vec3 m_position;
    float m_constant;
    float m_linear;
    float m_quadratic;

    float m_data[16];
    static size_t s_nPointLight;
};
using PointLights = std::vector<PointLight>;

#endif // POINTLIGHT_H
