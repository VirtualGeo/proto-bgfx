#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include <vector>
#include "light.h"
#include "program.h"

class SpotLight : public Light
{
public:
    SpotLight(bx::Vec3 direction, bx::Vec3 position, float cutOff =20.0f, float outerCutOff = 25.0f, float constant = 1.0, float linear = 0.09, float quadratic = 0.032);

//private:
//protected:
    void updateLightShadowMaps(int viewId) override;
//    void updateData();
    void updatePos(bx::Vec3 position);
    void updateDirection(bx::Vec3 direction);
    void updateData();


    bx::Vec3 m_direction;
    bx::Vec3 m_position;
    float m_cutOff;
    float m_outerCutOff;
    float m_constant;
    float m_linear;
    float m_quadratic;

    float m_lightSpaceMatrix[16];

    float m_data[4 * Program::s_num_vec4_spotLight];

};

using SpotLights = std::vector<SpotLight>;

#endif // SPOTLIGHT_H
