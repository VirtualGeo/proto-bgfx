#ifndef PROGRAM_H
#define PROGRAM_H

#include <bgfx/bgfx.h>
#include <iostream>
#include "material.h"
#include "texture.h"
#include "light.h"
#include "dirLight.h"

class Program {
public:
    enum Shading {
        NORMAL = 0,
        MATERIAL,
        RENDERED,
        Count
    };
    static const std::string shadingFileNames[Program::Shading::Count];
    static constexpr unsigned int s_num_vec4_uniforms = 8;

    Program();
    ~Program();

    //    void init(const char* shaderName);
    void init();

    void clear();
    void setShading(Shading shading);
    Shading shading() const;
    void submit(const Material & material, const DirLight & light, const Textures & textures) const;

    //    bgfx::ProgramHandle program() const;

    private:
    //    bgfx::ProgramHandle m_handle;
    //    bgfx::ShaderHandle m_vsh;
    //    bgfx::ShaderHandle m_fsh;

//    bgfx::UniformHandle m_uDiffuse;
    bgfx::UniformHandle m_sDiffuse;
    bgfx::UniformHandle m_sOpacity;

//    bgfx::UniformHandle m_uTexturesEnable;

//private:
    bgfx::ProgramHandle m_handlePrograms[Shading::Count];
    int m_iHandleProgram = Shading::RENDERED;

    bgfx::UniformHandle m_handleUniform;

public:
    bgfx::ProgramHandle handleProgram() const;
};

#endif // PROGRAM_H
