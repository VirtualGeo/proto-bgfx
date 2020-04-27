#ifndef DIRLIGHT_H
#define DIRLIGHT_H

#include "light.h"
#include <vector>

class DirLight : public Light
{
public:
    DirLight(bx::Vec3 direction);

//protected:
    void updateLightShadowMaps() override;
//private:
    bx::Vec3 m_direction;

    float m_data[16];

};

using DirLights = std::vector<DirLight>;

#endif // DIRLIGHT_H
