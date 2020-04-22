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
//bgfx::UniformHandle Program::m_vrMaterialParameters;
bgfx::UniformHandle Program::m_diffuse;

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
    m_sDiffuse = bgfx::createUniform("s_diffuse", bgfx::UniformType::Sampler);
    m_sOpacity = bgfx::createUniform("s_opacity", bgfx::UniformType::Sampler);
    m_handleUniform = bgfx::createUniform("u_params", bgfx::UniformType::Vec4, s_num_vec4_uniforms);
    //    m_vrMaterialParameters = bgfx::createUniform("uVrMateralParamaters", bgfx::UniformType::Vec4, 4);
    m_diffuse = bgfx::createUniform("diffuse", bgfx::UniformType::Vec4);


    for (int i = 0; i < Shading::Count; ++i) {
        m_handlePrograms[i] = BGFX_INVALID_HANDLE;
//        if (i != Shading::EMISSIVE)
//                if (i != Shading::NORMAL)
//            continue;
        //        int i= Shading::NORMAL;
        //    bgfx::ShaderHandle vsh = loadShader("cubes.vert");
        const std::string& shadingFileName = shadingFileNames[i];

//        std::cout << "[program] load vertex shader " << i << std::endl;
        const bgfx::ShaderHandle vsh = loadShader(shadingFileName + ".vert", Vertex);
        if (! bgfx::isValid(vsh))
            continue;

        bgfx::setName(vsh, shadingFileName.c_str());

        //    bgfx::ShaderHandle fsh = loadShader("cubes.frag");
//        std::cout << "[program] load fragment shader " << i << std::endl;
        bgfx::ShaderHandle fsh = loadShader(shadingFileName + ".frag", Fragment);
        bgfx::setName(fsh, shadingFileName.c_str());
        // bgfx::ShaderHandle vsh = loadShader("mesh.vert");
        // bgfx::ShaderHandle fsh = loadShader("mesh.frag");
//        std::cout << "[program] create program " << i << std::endl;
        m_handlePrograms[i] = bgfx::createProgram(vsh, fsh, true);
//        bgfx::frame();
        //        bgfx::setName(m_handlePrograms[i], shadingFileName.c_str());
    }

    //        m_uDiffuse = bgfx::createUniform("u_diffuse", bgfx::UniformType::Vec4);

    //    m_uHasDiffuseTexture = bgfx::createUniform("u_hasDiffuseTexture", bgfx::UniformType::Vec4);
    //        m_uTexturesEnable = bgfx::createUniform("u_texturesEnable", bgfx::UniformType::Vec4);
    bgfx::frame();
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
    //    bgfx::destroy(m_vrMaterialParameters);
    bgfx::destroy(m_diffuse);
}

void Program::submit(const bgfx::ViewId id, const Shading& shading, const Material& material, const SpotLights &spotLights, const Textures& textures, const Camera& camera)
{
//    float vec4[4] = { 0.0, 1.0, 1.0, 1.0 };

    switch (shading) {
    case Shading::NORMAL:
        //        break;
    case Shading::EMISSIVE:
        // question : why static ?
        //        static const float buffer2[4][4] = {
        //            { material.m_ambient[0], material.m_ambient[1], material.m_ambient[2], material.m_texturesEnable[0] }, // question how do smaller, without glm::vec4
        //            { material.m_diffuse[0], material.m_diffuse[1], material.m_diffuse[2], material.m_diffuse[3] }, // question how do smaller, without glm::vec4
        //            //            {material.m_specular[0], material.m_specular[1], material.m_specular[2], material.m_specular[3]}, // question how do smaller, without glm::vec4
        //            { material.m_specular[0], material.m_specular[1], material.m_specular[2], material.m_shininess }, // question how do smaller, without glm::vec4
        //            { material.m_diffuse[0], material.m_diffuse[1], material.m_diffuse[2], material.m_diffuse[3] }, // question how do smaller, without glm::vec4
        //            //            {material.m_texturesEnable[0]},
        //            //            {dirLight.m_diffuse.x, dirLight.m_diffuse.y, dirLight.m_diffuse.z},
        //            //            {dirLight.m_specular.x, dirLight.m_specular.y, dirLight.m_specular.z},
        //            //            {}
        //        };
        //        //        buffer[0] = material.m_diffuse;
        //        bgfx::setUniform(m_vrMaterialParameters, buffer2, 4);
        //        float vec4[4] = {0.0, 1.0, 1.0, 1.0};
//        static float vec4[4] = {material.m_diffuse[0], material.m_diffuse[1], material.m_diffuse[2], 1.0f};
        bgfx::setUniform(m_diffuse, material.m_diffuse);
        //        bgfx::setUniform(handle, buffer2, 4);
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

        const SpotLight & spotLight = spotLights[0];
        //        const std::array<float[4], s_num_vec4_uniforms> buffer = {{
        const float buffer[s_num_vec4_uniforms][4] = {
            { material.m_diffuse[0], material.m_diffuse[1], material.m_diffuse[2], material.m_diffuse[3] }, // question how do smaller, without glm::vec4
            //            {material.m_specular[0], material.m_specular[1], material.m_specular[2], material.m_specular[3]}, // question how do smaller, without glm::vec4
            { material.m_specular[0], material.m_specular[1], material.m_specular[2], material.m_texturesEnable[0] }, // question how do smaller, without glm::vec4
            { material.m_ambient[0], material.m_ambient[1], material.m_ambient[2], material.m_shininess }, // question how do smaller, without glm::vec4
            //            {material.m_texturesEnable[0]},
            {spotLight.m_direction.x, spotLight.m_direction.y, spotLight.m_direction.z},
            {spotLight.m_ambient.x, spotLight.m_ambient.y, spotLight.m_ambient.z},
//            { lights.m_direction.x, lights.m_direction.y, lights.m_direction.z },
//            { lights.m_ambient.x, lights.m_ambient.y, lights.m_ambient.z },
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

const char* s_uniformTypeName[] = {
    "int",
    "int",
    NULL,
    NULL,
    "vec4",
    "float4",
    "mat3",
    "float3x3",
    "mat4",
    "float4x4",
};
BX_STATIC_ASSERT(BX_COUNTOF(s_uniformTypeName) == bgfx::UniformType::Count * 2);

bx::StringView nextWord(bx::StringView& _parse)
{
    bx::StringView word = bx::strWord(bx::strLTrimSpace(_parse));
    _parse = bx::strLTrimSpace(bx::StringView(word.getTerm(), _parse.getTerm()));
    return word;
}

bgfx::UniformType::Enum nameToUniformTypeEnum(const char* _name)
{
    for (uint32_t ii = 0; ii < bgfx::UniformType::Count * 2; ++ii) {
        if (NULL != s_uniformTypeName[ii]
            && 0 == bx::strCmp(_name, s_uniformTypeName[ii])) {
            return bgfx::UniformType::Enum(ii / 2);
        }
    }

    return bgfx::UniformType::Count;
}

void strReplace(char* _str, const char* _find, const char* _replace)
{
    const int32_t len = bx::strLen(_find);

    char* replace = (char*)alloca(len + 1);
    bx::strCopy(replace, len + 1, _replace);
    for (int32_t ii = bx::strLen(replace); ii < len; ++ii) {
        replace[ii] = ' ';
    }
    replace[len] = '\0';

    BX_CHECK(len >= bx::strLen(_replace), "");
    for (bx::StringView ptr = bx::strFind(_str, _find); !ptr.isEmpty(); ptr = bx::strFind(ptr.getPtr() + len, _find)) {
        bx::memCopy(const_cast<char*>(ptr.getPtr()), replace, len);
    }
}

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

    bool binFile = false;
    filePath = PROJECT_DIR + shaderPath + filename + ".glsl";
    file = std::ifstream(filePath, std::ios::binary);
    if (!file.is_open()) {
        file.close();

        filePath = PROJECT_DIR + shaderPath + filename + ".bin";

        file = std::ifstream(filePath, std::ios::binary);

        if (!file.is_open()) {
            std::cout << "[main] Failed to load '" << filePath << "' '" << filename << "'"
                      << std::endl;

//            exit(-1);
            file.close();
            return BGFX_INVALID_HANDLE;
        }
        binFile = true;
    }
    long begin = file.tellg();
    file.seekg(0, std::ios::end);
    long end = file.tellg();
    long fileSize = end - begin;
    file.seekg(0, std::ios::beg);

    const bgfx::Memory* mem;
    if (!binFile) {
        char code[fileSize];
        file.read(code, fileSize);


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
                header[3] = 99; // bgfx_shader_bin_version

        uint32_t hash = 0;
        memcpy(&header[4], &hash, sizeof(hash));

        //        std::string uniformHeader;
        //        char buff[9999];
//        std::cout << "uniform size : " << uniforms.size() << std::endl;
//        uint16_t countUniform = uniforms.size();
        uint16_t countUniform = 0;

        memcpy(&header[12], &countUniform, sizeof(countUniform));

        uint32_t shaderSize = fileSize;
        memcpy(&header[14], &shaderSize, sizeof(shaderSize));
        //        for (int i = 0; i < 18; ++i) {
        //            std::cout << (int)header[i] << " ";
        //        }
        //        std::cout << std::endl;
        //        std::cout << "file size = " << fileSize << std::endl;

        mem = bgfx::alloc(headerSize + fileSize + 1);
        //    fread(mem->data, 1, fileSize, file);
        //        mem->data[0] = 1;
        memcpy((char*)&mem->data[0], header, headerSize);

        //                file.read((char*)&mem->data[headerSize], fileSize);
        memcpy((char*)&mem->data[headerSize], code, fileSize);
    } else {

        mem = bgfx::alloc(fileSize + 1);
        //    fread(mem->data, 1, fileSize, file);
        //        memcpy((char*)mem->data, header, sizeof(header));

        file.read((char*)&mem->data[0], fileSize);
        //        mem->data[0] = 0;
    }
    file.close();
    //    mem->data[mem->size - 1] = '\0';
    mem->data[mem->size - 1] = 0;
    //    fclose(file);
    //    free(filePath);
    bgfx::ShaderHandle handle = bgfx::createShader(mem);
    bgfx::setName(handle, filename.c_str());
//    std::cout << "[program] createShader " << filename << std::endl;

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
