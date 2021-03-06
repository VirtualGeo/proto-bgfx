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
    void mouseMove(float dx, float dy);

    void updateLightSpaceMatrix();
    void updateData();
//private:
    bx::Vec3 m_direction;

    float m_lightSpaceMatrix[16];
    static constexpr unsigned int s_numDirLightMax = 1;
    static constexpr unsigned int s_num_vec4_dirLight = 8;
    static constexpr unsigned int s_dirLightSizeMax = s_num_vec4_dirLight * s_numDirLightMax * 4;

    float m_data[4 * s_num_vec4_dirLight];
    static size_t s_nDirLight;

    static bgfx::UniformHandle s_uDirLightsUH;


    bgfx::UniformHandle m_sShadowMapUH = BGFX_INVALID_HANDLE;
    bgfx::FrameBufferHandle m_shadowMapFBH = BGFX_INVALID_HANDLE;
    bgfx::TextureHandle m_shadowMapTH = BGFX_INVALID_HANDLE;

};

using DirLights = std::vector<DirLight>;

#endif // DIRLIGHT_H
