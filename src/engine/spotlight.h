#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include <vector>
#include "light.h"

class SpotLight : public Light
{
public:
    SpotLight(bx::Vec3 && direction);

//private:

    bx::Vec3 m_direction;
};

using SpotLights = std::vector<SpotLight>;

#endif // SPOTLIGHT_H
