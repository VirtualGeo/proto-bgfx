#ifndef PROGRAM_H
#define PROGRAM_H

#include <bgfx/bgfx.h>

class Program
{
public:
    Program();

    void init(const char * shaderName);

//    bgfx::ProgramHandle program() const;

//private:
    bgfx::ProgramHandle m_handle;
//    bgfx::ShaderHandle m_vsh;
//    bgfx::ShaderHandle m_fsh;

    bgfx::UniformHandle m_uDiffuse;
    bgfx::UniformHandle m_sDiffuse;

};

#endif // PROGRAM_H
