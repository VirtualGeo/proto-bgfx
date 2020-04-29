#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include <vector>
#include "light.h"

class SpotLight : public Light
{
public:
    SpotLight(bx::Vec3 && direction, bx::Vec3 && position, float cutOff =0.97, float outerCutOff = 0.95, float constant = 1.0, float linear = 0.09, float quadratic = 0.032);

//private:
//protected:
    void updateLightShadowMaps() override;


    bx::Vec3 m_direction;
    bx::Vec3 m_position;
    float m_cutOff;
    float m_outerCutOff;
    float m_constant;
    float m_linear;
    float m_quadratic;

    float m_data[20];

};

using SpotLights = std::vector<SpotLight>;

#endif // SPOTLIGHT_H
