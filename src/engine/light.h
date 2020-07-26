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
//    Light(const Light & light) = delete;
//    Light(Light && light);

    ~Light();

//protected:
    virtual void updateLightShadowMaps(int viewId) = 0;
    virtual void drawDebug() = 0;

    //private:
    bx::Vec3 m_ambient;
    bx::Vec3 m_diffuse;
    bx::Vec3 m_specular;

//    static size_t s_counter;

//    bgfx::FrameBufferHandle m_shadowMapFB;
//    bgfx::TextureHandle m_shadowMapTexture;
//    const int m_shadowMapSize = 1024;

//    bgfx::UniformHandle m_sShadowMap;


//private:
protected:
    int m_id;
    bool m_last = true;
private:
//    static int m_nbLight;
};


using Lights = std::vector<Light>;

#endif // LIGHT_H
