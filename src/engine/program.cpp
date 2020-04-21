#include "program.h"
#include <fstream>
#include <iostream>

#include <cassert>
#include <string>
//#include <bx/bx.h>
#include <bx/file.h>
#include <cstring>

enum ShaderType {
    Vertex,
    Fragment
};

const std::string shadingFileNames[Shading::Count] { "normal", "material", "rendered", "emissive" };
static constexpr unsigned int s_num_vec4_uniforms = 6;

bgfx::UniformHandle Program::m_sDiffuse;
bgfx::UniformHandle Program::m_sOpacity;
bgfx::ProgramHandle Program::m_handlePrograms[Shading::Count];
bgfx::UniformHandle Program::m_handleUniform;

//bgfx::ShaderHandle loadShader(const char* filename);
bgfx::ShaderHandle loadShader(const std::string& filename, ShaderType shaderType);

//Program::Program()
//{
//}

//Program::~Program()
//{
//    //    bgfx::destroy(m_handle);
//    //    bgfx::destroy(m_uDiffuse);
//    //    bgfx::destroy(m_sDiffuse);
//}

//void Program::init(const char *shaderName)
void Program::init()
{

    for (int i = 0; i < Shading::Count; ++i) {
        m_handlePrograms[i] = BGFX_INVALID_HANDLE;
        if (i == Shading::EMISSIVE)
            continue;
        //        if (i != (int)Shading::NORMAL)
        //            continue;
        //        int i= Shading::NORMAL;
        //    bgfx::ShaderHandle vsh = loadShader("cubes.vert");
        const std::string& shadingFileName = shadingFileNames[i];

        std::cout << "[program] load vertex shader " << i << std::endl;
        const bgfx::ShaderHandle vsh = loadShader(shadingFileName + ".vert", Vertex);
        bgfx::setName(vsh, shadingFileName.c_str());

        //    bgfx::ShaderHandle fsh = loadShader("cubes.frag");
        std::cout << "[program] load fragment shader " << i << std::endl;
        bgfx::ShaderHandle fsh = loadShader(shadingFileName + ".frag", Fragment);
        bgfx::setName(fsh, shadingFileName.c_str());
        // bgfx::ShaderHandle vsh = loadShader("mesh.vert");
        // bgfx::ShaderHandle fsh = loadShader("mesh.frag");
        std::cout << "[program] create program " << i << std::endl;
        m_handlePrograms[i] = bgfx::createProgram(vsh, fsh, true);
        bgfx::frame();
        //        bgfx::setName(m_handlePrograms[i], shadingFileName.c_str());
    }

    //        m_uDiffuse = bgfx::createUniform("u_diffuse", bgfx::UniformType::Vec4);

    m_sDiffuse = bgfx::createUniform("s_diffuse", bgfx::UniformType::Sampler);
    m_sOpacity = bgfx::createUniform("s_opacity", bgfx::UniformType::Sampler);
    m_handleUniform = bgfx::createUniform("u_params", bgfx::UniformType::Vec4, s_num_vec4_uniforms);

    //    m_uHasDiffuseTexture = bgfx::createUniform("u_hasDiffuseTexture", bgfx::UniformType::Vec4);
    //        m_uTexturesEnable = bgfx::createUniform("u_texturesEnable", bgfx::UniformType::Vec4);
}

void Program::clear()
{
    //        int i= Shading::NORMAL;
    for (int i = 0; i < Shading::Count; ++i) {
        if (bgfx::isValid(m_handlePrograms[i]))
            bgfx::destroy(m_handlePrograms[i]);
    }
    //    bgfx::destroy(m_handle);
    //    bgfx::destroy(m_uDiffuse);
    bgfx::destroy(m_sDiffuse);

    bgfx::destroy(m_sOpacity);
    //    bgfx::destroy(m_uTexturesEnable);

    bgfx::destroy(m_handleUniform);
}

void Program::submit(const bgfx::ViewId id, const Shading& shading, const Material& material, const DirLight& dirLight, const Textures& textures, const Camera& camera)
{

    switch (shading) {
    case Shading::NORMAL:
        break;

    case Shading::MATERIAL:
    case Shading::RENDERED:
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
            { material.m_diffuse[0], material.m_diffuse[1], material.m_diffuse[2], material.m_diffuse[3] }, // question how do smaller, without glm::vec4
            //            {material.m_specular[0], material.m_specular[1], material.m_specular[2], material.m_specular[3]}, // question how do smaller, without glm::vec4
            { material.m_specular[0], material.m_specular[1], material.m_specular[2], material.m_texturesEnable[0] }, // question how do smaller, without glm::vec4
            { material.m_ambient[0], material.m_ambient[1], material.m_ambient[2], material.m_shininess }, // question how do smaller, without glm::vec4
            //            {material.m_texturesEnable[0]},
            { dirLight.m_direction.x, dirLight.m_direction.y, dirLight.m_direction.z },
            { dirLight.m_ambient.x, dirLight.m_ambient.y, dirLight.m_ambient.z },
            { camera.m_pos.x, camera.m_pos.y, camera.m_pos.z },
            //            {dirLight.m_diffuse.x, dirLight.m_diffuse.y, dirLight.m_diffuse.z},
            //            {dirLight.m_specular.x, dirLight.m_specular.y, dirLight.m_specular.z},
            //            {}
        };
        //        buffer[0] = material.m_diffuse;
        bgfx::setUniform(m_handleUniform, buffer, s_num_vec4_uniforms);
        //        bgfx::setUniform(m_handleUniform, buffer.data(), s_num_vec4_uniforms);

        break;
    }

    bgfx::submit(id, m_handlePrograms[shading]);
}

//bgfx::ProgramHandle Program::program() const
//{
//    return m_program;
//}

bgfx::ShaderHandle loadShader(const std::string& filename, ShaderType shaderType)
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

    //    std::string filePath = PROJECT_DIR + shaderPath + filename + ".bin";
    //    const bgfx::Memory* mem;
    std::ifstream file;
    std::string filePath;

    //    if (bgfx::RendererType::OpenGL) {
    filePath = PROJECT_DIR + shaderPath + filename + ".glsl";

    //    std::fstream file;
    file = std::ifstream(filePath, std::ios::binary);
    //    file.open(filePath, std::ios::in | std::ios::binary);

    //    if (file == NULL) {
    bool binFile = false;
    if (!file.is_open()) {
        //            std::cout << "[main] Failed to load '" << filePath << "' '" << filename << "'"
        //                      << std::endl;

        //            exit(-1);
        file.close();
        filePath = PROJECT_DIR + shaderPath + filename + ".bin";

        //    std::fstream file;
        //    std::ifstream file(filePath, std::ios::binary);
        file = std::ifstream(filePath, std::ios::binary);
        //    file.open(filePath, std::ios::in | std::ios::binary);

        //    if (file == NULL) {
        if (!file.is_open()) {
            std::cout << "[main] Failed to load '" << filePath << "' '" << filename << "'"
                      << std::endl;

            exit(-1);
            file.close();
        }
        binFile = true;
    }
    //    }
    long begin = file.tellg();
    file.seekg(0, std::ios::end);
    long end = file.tellg();
    long fileSize = end - begin;
    file.seekg(0, std::ios::beg);

    const bgfx::Memory* mem;
    if (!binFile) {
        const int headerSize = 18;
        char header[headerSize] = { 0 };
        switch (shaderType) {
        case Vertex:
            header[0] = 'V';
            break;
        case Fragment:
            header[0] = 'F';
            break;
        }
        header[1] = 'S';
        header[2] = 'H';
        header[3] = 6; // bgfx_shader_bin_version

        uint32_t hash = 0;
        memcpy(&header[4], &hash, sizeof(hash));

        uint32_t shaderSize = fileSize;
        memcpy(&header[14], &shaderSize, sizeof(shaderSize));
        //        for (int i = 0; i < 18; ++i) {
        //            std::cout << (int)header[i] << " ";
        //        }
        //        std::cout << std::endl;
        //        std::cout << "file size = " << fileSize << std::endl;
        mem = bgfx::alloc(headerSize + fileSize + 1);
        //    fread(mem->data, 1, fileSize, file);
        memcpy((char*)mem->data, header, sizeof(header));

        file.read((char*)&mem->data[headerSize], fileSize);
    } else {

        mem = bgfx::alloc(fileSize + 1);
        //    fread(mem->data, 1, fileSize, file);
        //        memcpy((char*)mem->data, header, sizeof(header));

        file.read((char*)mem->data, fileSize);
    }
    file.close();
    //    mem->data[mem->size - 1] = '\0';
    mem->data[mem->size - 1] = 0;
    //    fclose(file);
    //    free(filePath);
    bgfx::ShaderHandle handle = bgfx::createShader(mem);
    bgfx::setName(handle, filename.c_str());

    return handle;
    //    return bgfx::createShader(mem);
}

// ------------------------------------ GETTERS AND SETTERS
//bgfx::ProgramHandle Program::handleProgram(const Shading &shading)
//{
//    return m_handlePrograms[shading];
//}

const char* Program::filename(const Shading& shading)
{
    return shadingFileNames[shading].c_str();
}

//void Program::setShading(Program::Shading shading)
//{
//    m_iHandleProgram = shading;
//}

//Program::Shading Program::shading() const
//{
//    return Shading(m_iHandleProgram);
//}
