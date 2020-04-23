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

    Program() = delete;
    ~Program() = delete;

    //    void init(const char* shaderName);
    static void init(const bgfx::Caps * caps);
    static void clear();
    static void submit(const bgfx::ViewId id, const Shading & shading, const Material& material, const SpotLights& spotLights, const Textures& textures, const Camera& camera);
//    static bgfx::ProgramHandle handleProgram(const Shading &shading);
    static const char *filename(const Shading & shading) ;

    //    bgfx::ProgramHandle program() const;

private:
    //    bgfx::ProgramHandle m_handle;
    //    bgfx::ShaderHandle m_vsh;
    //    bgfx::ShaderHandle m_fsh;
    static const bgfx::Caps * m_caps;

    //    bgfx::UniformHandle m_uDiffuse;
    static bgfx::UniformHandle m_sDiffuse;
    static bgfx::UniformHandle m_sOpacity;

    //    bgfx::UniformHandle m_uTexturesEnable;

    //private:
    static bgfx::ProgramHandle m_programs[Shading::Count];
    //    int m_iHandleProgram = Shading::RENDERED;

    static bgfx::UniformHandle m_uniformArray;
//    static bgfx::UniformHandle m_vrMaterialParameters;
//    static bgfx::UniformHandle m_diffuse;

    static bgfx::UniformHandle m_sShadowMap;
    static bgfx::UniformHandle m_uLightPos;
    static bgfx::UniformHandle m_uLightMtx;
    static bgfx::UniformHandle m_uDepthScaleOffset;

public:
//    bgfx::ProgramHandle handleProgram(Shading & shading) const;
    //    void setShading(Shading shading);
//    Shading shading() const;
};

#endif // PROGRAM_H
