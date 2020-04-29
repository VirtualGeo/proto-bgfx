#ifndef LIGHT_H
#define LIGHT_H

#include <bx/file.h>
#include <vector>
#include <bgfx/bgfx.h>

class Light {
public:
    Light(bx::Vec3 ambient = bx::Vec3(0.0f),
          bx::Vec3 diffuse = bx::Vec3(1.0f),
          bx::Vec3 specular = bx::Vec3(1.0f));
    //    Light(bx::Vec3 ambient, bx::Vec3 diffuse, bx::Vec3 specular);
    Light(const Light & light) = delete;
    Light(Light && light);

    ~Light();

//protected:
    virtual void updateLightShadowMaps() = 0;

    //private:
    bx::Vec3 m_ambient;
    bx::Vec3 m_diffuse;
    bx::Vec3 m_specular;


//    bgfx::FrameBufferHandle m_shadowMapFB = BGFX_INVALID_HANDLE;
//    bgfx::TextureHandle m_shadowMapTexture = BGFX_INVALID_HANDLE;
//    const int m_shadowMapSize = 100;

private:
//    bool m_last = true;
};

using Lights = std::vector<Light>;

#endif // LIGHT_H
