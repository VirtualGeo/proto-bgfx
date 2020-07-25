#ifndef PROGRAM_H
#define PROGRAM_H

//#include "camera.h"
#include "dirLight.h"
#include "light.h"
#include "material.h"
#include "texture.h"
#include <bgfx/bgfx.h>
#include <iostream>
//#include "spotlight.h"
#include <set>

//#define RENDERVIEW_0_ID 0
//#define VIEWID_CAMERA 20
//#define VIEWID_SHADOW 10

enum Shading {
    NORMAL = 0,
    MATERIAL,
    RENDERED,
    EMISSIVE,
    SHADOW,
    DEBUG_QUAD,
    Count
};
//inline Shading operator++(Shading& shading, int)
//{
//    Shading temp = shading;
//    shading = static_cast<Shading>((static_cast<int>(shading) + 1) % Shading::Count);
//    return temp;
//}
inline Shading& operator++(Shading& shading)
{
    shading = static_cast<Shading>((static_cast<int>(shading) + 1) % Shading::Count);
    return shading;
}

class Program {
public:

    static bgfx::ProgramHandle loadProgram(const std::string& shaderName, const char * pShaderDefines);
    //    static const std::string shadingFileNames[Program::Shading::Count];
//    static constexpr unsigned int s_num_vec4_uniforms = 6;
//    static constexpr unsigned int s_num_vec4_material = 4;

//    static constexpr unsigned int s_num_vec4_dirLight = 4;
//    static constexpr unsigned int s_numDirLightMax = 1;
//    static unsigned int s_nDirLight;
//    static constexpr unsigned int s_dirLightSizeMax = s_num_vec4_dirLight * s_numDirLightMax * 4;

//    static constexpr unsigned int s_num_vec4_spotLight = 9;
//    static constexpr unsigned int s_numSpotLightMax = 2;
//    static unsigned int s_nSpotLight;
//    static constexpr unsigned int s_spotLightSizeMax = s_num_vec4_spotLight * s_numSpotLightMax * 4;

//    static constexpr unsigned int s_num_vec4_pointLight = 4;
//    static constexpr unsigned int s_numPointLightMax = 1;
//    static unsigned int s_nPointLight;
//    static constexpr unsigned int s_pointLightSizeMax = s_num_vec4_pointLight * s_numPointLightMax * 4;

//    static constexpr unsigned int s_num_lightMax = s_numDirLightMax + s_numSpotLightMax + s_numSpotLightMax;

//    static constexpr unsigned int s_num_lightMax = 2;

    Program() = delete;
    ~Program() = delete;

    //    void init(const char* shaderName);
    static void init();
    static void shutdown();
    static void submit(const bgfx::ViewId id, const Shading & shading, const Material& material);
//    static bgfx::ProgramHandle handleProgram(const Shading &shading);
    static const char *filename(const Shading & shading) ;

    //    bgfx::ProgramHandle program() const;

//private:
    //    bgfx::ProgramHandle m_handle;
    //    bgfx::ShaderHandle m_vsh;
    //    bgfx::ShaderHandle m_fsh;
//    static const bgfx::Caps * m_caps;


    //    bgfx::UniformHandle m_uDiffuse;

    //    bgfx::UniformHandle m_uTexturesEnable;

    //private:
    static bgfx::ProgramHandle m_programs[Shading::Count];
    //    int m_iHandleProgram = Shading::RENDERED;

//    static bgfx::UniformHandle m_vrMaterialParameters;
//    static bgfx::UniformHandle m_diffuse;
//    static bgfx::UniformHandle m_uMaterial;
//    static bgfx::UniformHandle m_sDiffuse;
//    static bgfx::UniformHandle m_sOpacity;

//    static bgfx::UniformHandle m_uDirLights[s_numDirLightMax][s_num_vec4_dirLight];

//    static bgfx::UniformHandle m_uDirLights;
//    static bgfx::UniformHandle m_uSpotLights;
//    static bgfx::UniformHandle m_uPointLights;
//    static bgfx::UniformHandle m_uNbDirLights;

    static bgfx::UniformHandle m_uViewPos;
    static bgfx::UniformHandle m_uInvModel;

//    static bgfx::UniformHandle m_sShadowMap;
//    static bgfx::UniformHandle m_sShadowMaps[];
//    static bgfx::UniformHandle m_sShadowMap_light_0;
//    static bgfx::UniformHandle m_sShadowMap_light_1;
//    static bgfx::UniformHandle m_sShadowMap_light_2;
//    static bgfx::UniformHandle m_uLightSpaceMatrix;
//    static bgfx::TextureHandle m_shadowMapTexture;

//    static bgfx::TextureHandle m_shadowMapTexture[];
//    static const int m_shadowMapSize = 1024;
//    static const int m_shadowMapSize = 2048;
//    static const int m_shadowMapSize = 4096;
//    static bgfx::FrameBufferHandle m_shadowMapFB[];


//    static bgfx::UniformHandle m_uLightPos;
//    static bgfx::UniformHandle m_uLightMtx;
//    static bgfx::UniformHandle m_uDepthScaleOffset;


//    static bgfx::UniformHandle m_hasDiffuseTexture;
//    static bgfx::UniformHandle m_hasSpecularTexture;
//    static bgfx::UniformHandle m_diffuseTexture;
//    static bgfx::UniformHandle m_diffuseColor;
//    static bgfx::UniformHandle m_specularTexture;
//    static bgfx::UniformHandle m_hasDiffuseTexture;
//    static bgfx::UniformHandle m_diffuseTexture;


//    uint16_t m_shadowMapSize = 512;
//    static bgfx::ProgramHandle m_progShadow;


public:
//    bgfx::ProgramHandle handleProgram(Shading & shading) const;
    //    void setShading(Shading shading);
//    Shading shading() const;
private:
    static bool s_initialized;
    static bool s_shutdowned;
};

#endif // PROGRAM_H
