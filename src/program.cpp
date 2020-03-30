#include "program.h"
#include <fstream>
#include <iostream>

#include <string>
#include <cassert>
//#include <bx/bx.h>
#include <bx/file.h>

const std::string Program::shadingFileNames[Program::Shading::Count] { "normal", "material", "rendered" };

//bgfx::ShaderHandle loadShader(const char* filename);
bgfx::ShaderHandle loadShader(const std::string& filename);

Program::Program()
{
}

Program::~Program()
{
    //    bgfx::destroy(m_handle);
    //    bgfx::destroy(m_uDiffuse);
    //    bgfx::destroy(m_sDiffuse);
}

//void Program::init(const char *shaderName)
void Program::init()
{

    for (int i =0; i < Shading::Count; ++i) {
        //    bgfx::ShaderHandle vsh = loadShader("cubes.vert");
        const std::string & shadingFileName = shadingFileNames[i];

        const bgfx::ShaderHandle vsh = loadShader(shadingFileName + ".vert");
        bgfx::setName(vsh, shadingFileName.c_str());

        //    bgfx::ShaderHandle fsh = loadShader("cubes.frag");
        bgfx::ShaderHandle fsh = loadShader(shadingFileName + ".frag");
        bgfx::setName(fsh, shadingFileName.c_str());
        // bgfx::ShaderHandle vsh = loadShader("mesh.vert");
        // bgfx::ShaderHandle fsh = loadShader("mesh.frag");
        m_handlePrograms[i] = bgfx::createProgram(vsh, fsh, true);
//        bgfx::setName(m_handlePrograms[i], shadingFileName.c_str());
    }

//    m_uDiffuse = bgfx::createUniform("u_diffuse", bgfx::UniformType::Vec4);
    m_sDiffuse = bgfx::createUniform("s_diffuse", bgfx::UniformType::Sampler);

    m_sOpacity = bgfx::createUniform("s_opacity", bgfx::UniformType::Sampler);
    //    m_uHasDiffuseTexture = bgfx::createUniform("u_hasDiffuseTexture", bgfx::UniformType::Vec4);
//    m_uTexturesEnable = bgfx::createUniform("u_texturesEnable", bgfx::UniformType::Vec4);

    m_handleUniform = bgfx::createUniform("u_params", bgfx::UniformType::Vec4, s_num_vec4_uniforms);
}

void Program::clear()
{
    for (int i =0; i <Shading::Count; ++i) {
        bgfx::destroy(m_handlePrograms[i]);
    }
//    bgfx::destroy(m_handle);
//    bgfx::destroy(m_uDiffuse);
    bgfx::destroy(m_sDiffuse);

    bgfx::destroy(m_sOpacity);
//    bgfx::destroy(m_uTexturesEnable);

    bgfx::destroy(m_handleUniform);
}

void Program::setShading(Program::Shading shading)
{
    m_iHandleProgram = shading;
}

Program::Shading Program::shading() const
{
    return Shading(m_iHandleProgram);
}

void Program::submit(const Material &material, const DirLight &dirLight, const Textures &textures) const
{

    switch (m_iHandleProgram) {
    case Program::Shading::NORMAL:
        break;

    case Program::Shading::MATERIAL:
    case Program::Shading::RENDERED:
        //    const int iMaterial = m_iMaterial;

//        bgfx::setUniform(m_uDiffuse, material.diffuse());

        //    		if (group.m_texture != nullptr) {
//        const int iTexDiffuse = material.iTexDiffuse();
        const int iTexDiffuse = material.m_iTexDiffuse;
        if (iTexDiffuse >= 0) {

            assert(iTexDiffuse < textures.size());
            const Texture& texture = textures[iTexDiffuse];
            //                }
            // uint32_t flags = UINT32_MAX;
            // uint32_t flags = BGFX_TEXTURE_USE_DEFAULT;

            // const uint64_t textureFlags =
            // 0 | BGFX_CAPS_FORMAT_TEXTURE_MIP_AUTOGEN;
            // const uint64_t textureFlags = 0 | BGFX_TEXTURE_NONE;
            //        const uint64_t samplerFlags = 0 | BGFX_SAMPLER_MAG_ANISOTROPIC | BGFX_SAMPLER_MIN_ANISOTROPIC;
            // const uint64_t samplerFlags = 0 | BGFX_SAMPLER_NONE;
            bgfx::setTexture(0, m_sDiffuse,
                texture.textureHandle(), Texture::s_textureSamplerFlags);

            //  textureFlags | samplerFlags);

            //                 bgfx::setUniform(m_uHasDiffuseTexture, (void*)true);
            //        float temp[1] = { 1.0 };
            //        bgfx::setUniform(m_uHasDiffuseTexture, temp);
            //                 bgfx::setUniform(m_uHasDiffuseTexture);
        } else {
            bgfx::setTexture(0, m_sDiffuse, textures.front().textureHandle(), Texture::s_textureSamplerFlags);
        }

//        bgfx::setUniform(m_uTexturesEnable, material.texturesEnable());
//        const int iTexOpacity = material.iTexOpacity();
        const int iTexOpacity = material.m_iTexOpacity;
        if (iTexOpacity >= 0) {
            assert(iTexOpacity < textures.size());
            const Texture& texture = textures[iTexOpacity];

            bgfx::setTexture(1, m_sOpacity, texture.textureHandle(), Texture::s_textureSamplerFlags);
        } else {
            bgfx::setTexture(1, m_sOpacity, textures.front().textureHandle(), Texture::s_textureSamplerFlags);
        }

//        const std::array<float[4], s_num_vec4_uniforms> buffer = {{
        const float buffer[s_num_vec4_uniforms][4] = {
            {material.m_diffuse[0], material.m_diffuse[1], material.m_diffuse[2], material.m_diffuse[3]}, // question how do smaller, without glm::vec4
            {material.m_specular[0], material.m_specular[1], material.m_specular[2], material.m_specular[3]}, // question how do smaller, without glm::vec4
            {material.m_ambient[0], material.m_ambient[1], material.m_ambient[2], material.m_shininess}, // question how do smaller, without glm::vec4
            {material.m_texturesEnable[0]},
            {dirLight.m_direction.x, dirLight.m_direction.y, dirLight.m_direction.z},
            {dirLight.m_ambient.x, dirLight.m_ambient.y, dirLight.m_ambient.z},
            {dirLight.m_diffuse.x, dirLight.m_diffuse.y, dirLight.m_diffuse.z},
            {dirLight.m_specular.x, dirLight.m_specular.y, dirLight.m_specular.z},
//            {}
        };
//        buffer[0] = material.m_diffuse;
        bgfx::setUniform(m_handleUniform, buffer, s_num_vec4_uniforms);
//        bgfx::setUniform(m_handleUniform, buffer.data(), s_num_vec4_uniforms);

        break;
    }
}


bgfx::ProgramHandle Program::handleProgram() const
{
    return m_handlePrograms[m_iHandleProgram];
}

//bgfx::ProgramHandle Program::program() const
//{
//    return m_program;
//}

bgfx::ShaderHandle loadShader(const std::string& filename)
//bgfx::ShaderHandle loadShader(const char* filename)
{
    //    const char* shaderPath = "???";
    std::string shaderPath;

    switch (bgfx::getRendererType()) {
    case bgfx::RendererType::Noop:
    case bgfx::RendererType::Direct3D9:
        shaderPath = "shaders/bin/dx9/";
        break;
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12:
        shaderPath = "shaders/bin/dx11/";
        break;
    case bgfx::RendererType::Gnm:
        shaderPath = "shaders/bin/pssl/";
        break;
    case bgfx::RendererType::Metal:
        shaderPath = "shaders/bin/metal/";
        break;
    case bgfx::RendererType::OpenGL:
        shaderPath = "shaders/bin/glsl/";
        break;
    case bgfx::RendererType::OpenGLES:
        shaderPath = "shaders/bin/essl/";
        break;
    case bgfx::RendererType::Vulkan:
        shaderPath = "shaders/bin/spirv/";
        break;
    }
    // shaderPath = "shaders/bin/";
    // shaderPath = "/home/gauthier/tmp2/proto-bgfx/shaders/bin/";

    //    size_t shaderLen = strlen(shaderPath);
    //    size_t fileLen = strlen(FILENAME);
    //    char* filePath = (char*)malloc(shaderLen + fileLen + 5);
    std::string filePath = PROJECT_DIR + shaderPath + filename + ".bin";
    //    memcpy(filePath, shaderPath, shaderLen);
    //    memcpy(&filePath[shaderLen], FILENAME, fileLen);
    //    memcpy(&filePath[shaderLen + fileLen], ".bin\0", 5);

    // std::cout << "current path : " << std::filesystem::current_path() <<
    // std::endl; char buff[80]; GetCurrentDirectory(buff, 80);

    // FILE *file = fopen(FILENAME, "rb");
    // FILE *file = fopen(filePath, "rb");
    // bx::FileReaderI * fileReader = nullptr;
    // bx::AllocatorI * allocator = nullptr;
    // allocator = & bx::DefaultAllocator;
    // fileReader = BX_NEW(allocator, FileReader);

    // FILE *file = fopen("shaders/bin/cubes.vert.bin", "rb");
    //    FILE* file = fopen(filePath, "rb");
    //    std::fstream file;
    std::ifstream file(filePath, std::ios::binary);
    //    file.open(filePath, std::ios::in | std::ios::binary);

    //    if (file == NULL) {
    if (!file.is_open()) {
        // perror(std::string("Failed to load ")+  std::string(FILENAME));
        std::cout << "[main] Failed to load '" << filePath << "' '" << filename << "'"
                  << std::endl;

        // throw std::runtime_error("Failed to load " + std::string(FILENAME));

        exit(-1);
    }
    //    fseek(file, 0, SEEK_END);
    //    file.seekg(0, std::ios::seekdir::_S_end);
    //    long fileSize = ftell(file);
    //    long fileSize = file.tellg();
    long begin = file.tellg();
    file.seekg(0, std::ios::end);
    long end = file.tellg();
    long fileSize = end - begin;
    //    fseek(file, 0, SEEK_SET);
    //    file.seekg(0)

    file.seekg(0, std::ios::beg);

    const bgfx::Memory* mem = bgfx::alloc(fileSize + 1);
    //    fread(mem->data, 1, fileSize, file);
    file.read((char*)mem->data, fileSize);
    mem->data[mem->size - 1] = '\0';
    //    fclose(file);
    file.close();

    //    free(filePath);
    bgfx::ShaderHandle handle = bgfx::createShader(mem);
    bgfx::setName(handle, filename.c_str());

    return handle;
    //    return bgfx::createShader(mem);
}
