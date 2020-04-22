#ifndef LIGHT_H
#define LIGHT_H

#include <bx/file.h>
#include <vector>

class Light {
public:
    Light(bx::Vec3 ambient = bx::Vec3(0.2f), bx::Vec3 diffuse = bx::Vec3(1.0f), bx::Vec3 specular = bx::Vec3(1.0f));
    //    Light(bx::Vec3 ambient, bx::Vec3 diffuse, bx::Vec3 specular);

    //private:
    bx::Vec3 m_ambient;
    bx::Vec3 m_diffuse;
    bx::Vec3 m_specular;
};

using Lights = std::vector<Light>;

#endif // LIGHT_H
