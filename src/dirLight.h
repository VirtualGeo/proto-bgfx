#ifndef DIRLIGHT_H
#define DIRLIGHT_H

#include "light.h"

class DirLight : public Light
{
public:
    DirLight(bx::Vec3 direction);

//private:
    bx::Vec3 m_direction;
};

#endif // DIRLIGHT_H
