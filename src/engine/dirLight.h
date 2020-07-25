#ifndef DIRLIGHT_H
#define DIRLIGHT_H

#include "light.h"
#include <vector>
#include "program.h"


class DirLight : public Light
{
public:
    DirLight(bx::Vec3 direction);
    ~DirLight();

//protected:
    void updateLightShadowMaps(int viewId) override;
    void drawDebug() override;
//private:
    bx::Vec3 m_direction;

    float m_lightSpaceMatrix[16];
    static constexpr unsigned int s_num_vec4_dirLight = 4;
    float m_data[4 * s_num_vec4_dirLight];
    static size_t s_nDirLight;

    bgfx::UniformHandle m_sShadowMapUH = BGFX_INVALID_HANDLE;
    bgfx::FrameBufferHandle m_shadowMapFBH = BGFX_INVALID_HANDLE;
    bgfx::TextureHandle m_shadowMapTH = BGFX_INVALID_HANDLE;

};

using DirLights = std::vector<DirLight>;

#endif // DIRLIGHT_H
