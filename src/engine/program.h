#ifndef PROGRAM_H
#define PROGRAM_H

#include "camera.h"
#include "dirLight.h"
#include "light.h"
#include "material.h"
#include "texture.h"
#include <bgfx/bgfx.h>
#include <iostream>
#include "spotlight.h"

enum Shading {
    NORMAL = 0,
    MATERIAL,
    RENDERED,
    EMISSIVE,
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
    //    static const std::string shadingFileNames[Program::Shading::Count];
//    static constexpr unsigned int s_num_vec4_uniforms = 6;
    static constexpr unsigned int s_num_vec4_material = 4;

    static constexpr unsigned int s_num_vec4_dirLight = 4;
    static constexpr unsigned int s_numDirLightMax = 2;
    static constexpr unsigned int s_dirLightSizeMax = s_num_vec4_dirLight * s_numDirLightMax * 4;

    static constexpr unsigned int s_num_vec4_spotLight = 5;
    static constexpr unsigned int s_numSpotLightMax = 20;
    static constexpr unsigned int s_spotLightSizeMax = s_num_vec4_spotLight * s_numSpotLightMax * 4;

    static constexpr unsigned int s_num_vec4_pointLight = 4;
    static constexpr unsigned int s_numPointLightMax = 20;
    static constexpr unsigned int s_pointLightSizeMax = s_num_vec4_pointLight * s_numPointLightMax * 4;

    Program() = delete;
    ~Program() = delete;

    //    void init(const char* shaderName);
    static void init(const bgfx::Caps * caps);
    static void clear();
    static void submit(const bgfx::ViewId id, const Shading & shading, const Material& material, const Textures& textures);
//    static bgfx::ProgramHandle handleProgram(const Shading &shading);
    static const char *filename(const Shading & shading) ;

    //    bgfx::ProgramHandle program() const;

//private:
    //    bgfx::ProgramHandle m_handle;
    //    bgfx::ShaderHandle m_vsh;
    //    bgfx::ShaderHandle m_fsh;
    static const bgfx::Caps * m_caps;

    //    bgfx::UniformHandle m_uDiffuse;

    //    bgfx::UniformHandle m_uTexturesEnable;

    //private:
    static bgfx::ProgramHandle m_programs[Shading::Count];
    //    int m_iHandleProgram = Shading::RENDERED;

//    static bgfx::UniformHandle m_vrMaterialParameters;
//    static bgfx::UniformHandle m_diffuse;
    static bgfx::UniformHandle m_uMaterial;
    static bgfx::UniformHandle m_sDiffuse;
    static bgfx::UniformHandle m_sOpacity;

//    static bgfx::UniformHandle m_uDirLights[s_numDirLightMax][s_num_vec4_dirLight];
    static bgfx::UniformHandle m_uDirLights;
    static bgfx::UniformHandle m_uSpotLights;
    static bgfx::UniformHandle m_uPointLights;
//    static bgfx::UniformHandle m_uNbDirLights;

    static bgfx::UniformHandle m_uViewPos;
    static bgfx::UniformHandle m_uInvModel;

    static bgfx::UniformHandle m_sShadowMap;
    static bgfx::UniformHandle m_uLightPos;
    static bgfx::UniformHandle m_uLightMtx;
    static bgfx::UniformHandle m_uDepthScaleOffset;

    uint16_t m_shadowMapSize = 512;
    static bgfx::ProgramHandle m_progShadow;
    static bgfx::FrameBufferHandle m_shadowMapFB;


public:
//    bgfx::ProgramHandle handleProgram(Shading & shading) const;
    //    void setShading(Shading shading);
//    Shading shading() const;
};

#endif // PROGRAM_H
