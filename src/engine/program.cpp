#include "program.h"
#include <fstream>
#include <iostream>

#include <cassert>
#include <string>
//#include <bx/bx.h>
#include <bx/file.h>
#include <bx/hash.h>
#include <cstring>
#include <sstream>

//#include <tools/shaderc/shaderc.h>

#ifndef USE_SHADERC_EXECUTABLE
#include <shaderc/shaderc.h>
#endif

enum ShaderType {
    Vertex,
    Fragment
};

const std::string shadingFileNames[Shading::Count] { "normal", "material", "rendered", "emissive", "shadow", "debugQuad" };

bgfx::UniformHandle Program::m_sDiffuse;
bgfx::UniformHandle Program::m_sOpacity;
bgfx::ProgramHandle Program::m_programs[Shading::Count];
bgfx::UniformHandle Program::m_uMaterial;
//bgfx::UniformHandle Program::m_vrMaterialParameters;
bgfx::UniformHandle Program::m_uViewPos;
bgfx::UniformHandle Program::m_uInvModel;
//bgfx::UniformHandle Program::m_diffuse;
bgfx::UniformHandle Program::m_sShadowMap;
//bgfx::UniformHandle Program::m_sShadowMap_light_0;
//bgfx::UniformHandle Program::m_sShadowMap_light_1;
//bgfx::UniformHandle Program::m_sShadowMap_light_2;
bgfx::UniformHandle Program::m_sShadowMaps[s_num_lightMax];
bgfx::UniformHandle Program::m_uLightSpaceMatrix;
bgfx::UniformHandle Program::m_uLightPos;
bgfx::UniformHandle Program::m_uLightMtx;
bgfx::UniformHandle Program::m_uDepthScaleOffset;
const bgfx::Caps* Program::m_caps = nullptr;
//bgfx::ProgramHandle Program::m_progShadow;
bgfx::FrameBufferHandle Program::m_shadowMapFB[s_num_lightMax];
bgfx::TextureHandle Program::m_shadowMapTexture[s_num_lightMax];

//bgfx::UniformHandle Program::m_uDirLights[s_numDirLightMax][s_num_vec4_dirLight];
bgfx::UniformHandle Program::m_uDirLights;
bgfx::UniformHandle Program::m_uSpotLights;
bgfx::UniformHandle Program::m_uPointLights;

//bgfx::UniformHandle Program::m_hasDiffuseTexture;
//bgfx::UniformHandle Program::m_hasSpecularTexture;
//bgfx::UniformHandle Program::m_diffuseTexture;
//bgfx::UniformHandle Program::m_diffuseColor;
//bgfx::UniformHandle Program::m_specularTexture;

bgfx::UniformHandle Program::m_hasTexture;
bgfx::UniformHandle Program::m_texture[s_nTexture];
//bgfx::UniformHandle Program::m_nAdditionalTexture;

//bgfx::ShaderHandle loadShader(const char* filename);
bgfx::ShaderHandle loadShader(const std::string& filename, ShaderType shaderType, const char* pShaderDefines);

//bool compileShader(const char* pFilename, const char* pOutput, bgfx::RendererType::Enum pType, const char* pDefines = nullptr)
//{

//    bool bVertex = !bx::strFind(pFilename, ".vert.sc").isEmpty();
//    bool bFragment = !bx::strFind(pFilename, ".frag.sc").isEmpty();
//    if (bVertex || bFragment)
//    {
//        // PATH should be bgfx\examples\runtime
//        const char* argv[] =
//        {
//            "shaderc.exe",
//            "-f",
//            pFilename,
//            "-i",
//            "../../src",
//            "-o",
//            pOutput,
//            "--platform",
//            pType == bgfx::RendererType::Direct3D11 ? "windows" : "linux",
//            "--type",
//            bVertex ? "vertex" : "fragment",
//            "--profile",

//            pType == bgfx::RendererType::Direct3D11
//            ? bVertex
//                ? "vs_5_0"
//                : "ps_5_0"
//            : "120",

//            "-O",
//            "3",
//            "--define",
//            pDefines ? pDefines : ""
//        };
//        int error_code = shaderc::compileShader(BX_COUNTOF(argv), argv);
//        return error_code == 0;
//    }

//    return false;
//}

//bgfx::ProgramHandle loadProgram(const std::string& shaderName);

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
void Program::init(const bgfx::Caps* caps)
{
    m_caps = caps;

    //    const char * buff[256];
    //    shaderc::compileShader(5, buff);

    m_sDiffuse = bgfx::createUniform("s_diffuse", bgfx::UniformType::Sampler);
    m_sOpacity = bgfx::createUniform("s_opacity", bgfx::UniformType::Sampler);
    m_uMaterial = bgfx::createUniform("u_material", bgfx::UniformType::Vec4, s_num_vec4_material);

    m_uDirLights = bgfx::createUniform("u_dirLights", bgfx::UniformType::Vec4, s_numDirLightMax * s_num_vec4_dirLight);
    m_uSpotLights = bgfx::createUniform("u_spotLights", bgfx::UniformType::Vec4, s_numSpotLightMax * s_num_vec4_spotLight);
    m_uPointLights = bgfx::createUniform("u_pointLights", bgfx::UniformType::Vec4, s_numPointLightMax * s_num_vec4_pointLight);

    //    std::string dirLight = "u_dirLights[";
    //    for (int i = 0; i < s_numDirLightMax; ++i) {
    //        //            bgfx::setUniform(m_uDirLights[i][0], temp);
    //        m_uDirLights[i][0] = bgfx::createUniform((dirLight + std::to_string(i) + "].dir").c_str(), bgfx::UniformType::Vec4);
    //        m_uDirLights[i][1] = bgfx::createUniform((dirLight + std::to_string(i) + "].color").c_str(), bgfx::UniformType::Vec4);
    //        //            bgfx::setUniform(m_uDirLights[i][1], temp);
    //        //			bgfx::setUniform(Program::m_uDirLights, temp, 4);
    //    }
    //    m_vrMaterialParameters = bgfx::createUniform("uVrMateralParamaters", bgfx::UniformType::Vec4, 4);
    //    m_diffuse = bgfx::createUniform("diffuse", bgfx::UniformType::Vec4);
    m_uViewPos = bgfx::createUniform("u_viewPos", bgfx::UniformType::Vec4);
    m_uInvModel = bgfx::createUniform("u_invModel", bgfx::UniformType::Mat4);

    m_sShadowMap = bgfx::createUniform("s_shadowMap", bgfx::UniformType::Sampler);
    for (int i = 0; i < s_num_lightMax; ++i) {
        m_sShadowMaps[i] = bgfx::createUniform((std::string("s_shadowMap_light_") + std::to_string(i)).c_str(), bgfx::UniformType::Sampler);
    }

    //    m_sShadowMap_light_0 = bgfx::createUniform("s_shadowMap_light_0", bgfx::UniformType::Sampler);
    //    m_sShadowMap_light_1 = bgfx::createUniform("s_shadowMap_light_1", bgfx::UniformType::Sampler);
    //    m_sShadowMap_light_2 = bgfx::createUniform("s_shadowMap_light_2", bgfx::UniformType::Sampler);

    //    m_sShadowMaps = bgfx::createUniform("s_shadowMaps", bgfx::UniformType::Sampler, s_num_lightMax);
    m_uLightSpaceMatrix = bgfx::createUniform("u_lightSpaceMatrix", bgfx::UniformType::Mat4);

    m_uLightPos = bgfx::createUniform("u_lightPos", bgfx::UniformType::Vec4);
    m_uLightMtx = bgfx::createUniform("u_lightMtx", bgfx::UniformType::Mat4);
    m_uDepthScaleOffset = bgfx::createUniform("u_depthScaleOffset", bgfx::UniformType::Vec4);

    for (int i = 0; i < s_num_lightMax; ++i) {
        bgfx::TextureHandle fbtextures[] = {
            bgfx::createTexture2D(
                m_shadowMapSize,
                m_shadowMapSize,
                false,
                1,
                bgfx::TextureFormat::BGRA8,
                BGFX_TEXTURE_RT),

            bgfx::createTexture2D(
                m_shadowMapSize,
                m_shadowMapSize,
                false,
                1,
                bgfx::TextureFormat::D16,
                BGFX_TEXTURE_RT_WRITE_ONLY),
        };
        m_shadowMapTexture[i] = fbtextures[0];
        assert(BX_COUNTOF(fbtextures) == 2);
        m_shadowMapFB[i] = bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, true);
    }

    //    bgfx::TextureHandle fbtextures[] = {
    ////    m_shadowMapTexture = {
    //        bgfx::createTexture2D(
    //            m_shadowMapSize, m_shadowMapSize,
    //            false, 1, bgfx::TextureFormat::D16,
    //            BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LEQUAL),
    ////    };
    //    };
    //    bgfx::TextureHandle fbtextures[] =
    //    {
    //        bgfx::createTexture2D(
    //              m_shadowMapSize
    //            , m_shadowMapSize
    //            , false
    //            , 1
    //            , bgfx::TextureFormat::BGRA8
    //            , BGFX_TEXTURE_RT
    //            ),
    //        bgfx::createTexture2D(
    //              m_shadowMapSize
    //            , m_shadowMapSize
    //            , false
    //            , 1
    //            , bgfx::TextureFormat::D16
    //            , BGFX_TEXTURE_RT_WRITE_ONLY
    //            ),
    //    };
    //    m_shadowMapTexture = fbtextures[0];
    //    m_shadowMapFB = bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, true);

    //    m_shadowMapTexture = fbtextures[0];
    //    m_shadowMapFB = bgfx::createFrameBuffer(1, &m_shadowMapTexture, false);
    //    m_shadowMapFB = bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, true);

    //    assert(bgfx::isValid(m_shadowMapFB));
    //    assert(bgfx::isValid(m_shadowMapTexture));

    //    m_shadowMapFB.idx = bgfx::kInvalidHandle;
    //    m_shadowMapTexture = fbtextures[0];
    //    m_shadowMapFB = bgfx::createFrameBuffer(1, &m_shadowMapTexture, false);

//    m_hasDiffuseTexture = bgfx::createUniform("hasDiffuseTexture", bgfx::UniformType::Vec4);
//    m_hasSpecularTexture = bgfx::createUniform("hasSpecularTexture", bgfx::UniformType::Vec4);
//    m_diffuseTexture = bgfx::createUniform("diffuseTexture", bgfx::UniformType::Sampler);
//    m_diffuseColor = bgfx::createUniform("diffuseColor", bgfx::UniformType::Vec4);
//    m_specularTexture = bgfx::createUniform("specularTexture", bgfx::UniformType::Sampler);

    m_hasTexture = bgfx::createUniform("hasTexture", bgfx::UniformType::Vec4, s_nTexture);
    for (int i =0; i <s_nTexture; ++i) {
        m_texture[i] = bgfx::createUniform((std::string("texture") + std::to_string(i)).c_str(), bgfx::UniformType::Sampler);
    }
//    m_nAdditionalTexture = bgfx::createUniform("nAdditionalTexture", bgfx::UniformType::Vec4);


    for (int i = 0; i < Shading::Count; ++i) {
        m_programs[i] = BGFX_INVALID_HANDLE;
        //        if (i == Shading::RENDERED)
        //            continue;
        //        if (i == Shading::SHADOW)
        //            continue;
        if (i == Shading::EMISSIVE)
            //            //                if (i != Shading::NORMAL)
            continue;
        //        if (i == Shading::DEBUG_QUAD)
        //            //                if (i != Shading::NORMAL)
        //            continue;
        //        if (i != Shading::NORMAL)
        //            continue;
        //        int i= Shading::NORMAL;
        //    bgfx::ShaderHandle vsh = loadShader("cubes.vert");
        //        const std::string& shadingFileName = "shading/" + shadingFileNames[i];
        const std::string& shadingFileName = shadingFileNames[i];

        m_programs[i] = loadProgram(shadingFileName, "");
        //        continue;
        //        std::cout << "[program] load vertex shader " << i << std::endl;
        //        const bgfx::ShaderHandle vsh = loadShader(shadingFileName, Vertex);
        //        if (!bgfx::isValid(vsh))
        //            continue;

        //        bgfx::setName(vsh, shadingFileName.c_str());

        //        //    bgfx::ShaderHandle fsh = loadShader("cubes.frag");
        //        //        std::cout << "[program] load fragment shader " << i << std::endl;
        //        bgfx::ShaderHandle fsh = loadShader(shadingFileName, Fragment);
        //        bgfx::setName(fsh, shadingFileName.c_str());
        //        // bgfx::ShaderHandle vsh = loadShader("mesh.vert");
        //        // bgfx::ShaderHandle fsh = loadShader("mesh.frag");
        //        //        std::cout << "[program] create program " << i << std::endl;
        //        m_programs[i] = bgfx::createProgram(vsh, fsh, true);
        //        bgfx::frame();
        //        bgfx::setName(m_programs[i], shadingFileName.c_str());
    }

    //    m_progShadow = loadProgram("shadow");
    //    bgfx::frame();

    //        m_uDiffuse = bgfx::createUniform("u_diffuse", bgfx::UniformType::Vec4);

    //    m_uHasDiffuseTexture = bgfx::createUniform("u_hasDiffuseTexture", bgfx::UniformType::Vec4);
    //        m_uTexturesEnable = bgfx::createUniform("u_texturesEnable", bgfx::UniformType::Vec4);

    float depthScaleOffset[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
    if (m_caps->homogeneousDepth) {
        depthScaleOffset[0] = depthScaleOffset[1] = 0.5f;
    }
    bgfx::setUniform(m_uDepthScaleOffset, depthScaleOffset);
    bgfx::touch(0);
}

void Program::clear()
{
    //        int i= Shading::NORMAL;
    for (int i = 0; i < Shading::Count; ++i) {
        if (bgfx::isValid(m_programs[i]))
            bgfx::destroy(m_programs[i]);
    }
    //    bgfx::destroy(m_handle);
    //    bgfx::destroy(m_uDiffuse);
    bgfx::destroy(m_sDiffuse);

    bgfx::destroy(m_sOpacity);
    //    bgfx::destroy(m_uTexturesEnable);

    bgfx::destroy(m_uMaterial);
    //    bgfx::destroy(m_vrMaterialParameters);
    bgfx::destroy(m_uViewPos);
    bgfx::destroy(m_uInvModel);

    //    bgfx::destroy(m_diffuse);
    bgfx::destroy(m_sShadowMap);
    for (int i = 0; i < s_num_lightMax; ++i) {
        bgfx::destroy(m_sShadowMaps[i]);
    }
    //    bgfx::destroy(m_sShadowMap_light_0);
    //    bgfx::destroy(m_sShadowMap_light_1);
    //    bgfx::destroy(m_sShadowMap_light_2);

    //    bgfx::destroy(m_sShadowMaps);
    for (int i = 0; i < s_num_lightMax; ++i) {
        bgfx::destroy(m_shadowMapTexture[i]);
        bgfx::destroy(m_shadowMapFB[i]);
    }
    bgfx::destroy(m_uLightSpaceMatrix);
    bgfx::destroy(m_uLightPos);
    bgfx::destroy(m_uLightMtx);
    bgfx::destroy(m_uDepthScaleOffset);
    bgfx::destroy(m_uDirLights);
    bgfx::destroy(m_uSpotLights);
    bgfx::destroy(m_uPointLights);
    //    for (int i = 0; i < s_numDirLightMax; ++i) {
    //        bgfx::destroy(m_uDirLights[i][0]);
    //        bgfx::destroy(m_uDirLights[i][1]);
    //    }
//    bgfx::destroy(m_hasDiffuseTexture);
//    bgfx::destroy(m_hasSpecularTexture);
//    bgfx::destroy(m_diffuseTexture);
//    bgfx::destroy(m_diffuseColor);
//    bgfx::destroy(m_specularTexture);

    bgfx::destroy(m_hasTexture);
    for (int i =0; i <s_nTexture; ++i) {
        bgfx::destroy(m_texture[i]);
    }
//    bgfx::destroy(m_nAdditionalTexture);
}

void Program::submit(const bgfx::ViewId id, const Shading& shading, const Material& material, const Textures& textures)
{
    //    float vec4[4] = { 0.0, 1.0, 1.0, 1.0 };
    int iTexDiffuse;

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
        //        bgfx::setUniform(m_diffuse, material.m_diffuse);
        //        bgfx::setUniform(handle, buffer2, 4);
        break;

    case Shading::MATERIAL:
    case Shading::RENDERED:
        //    const int iMaterial = m_iMaterial;

        //        bgfx::setUniform(m_uDiffuse, material.diffuse());

        //    		if (group.m_texture != nullptr) {
        //        const int iTexDiffuse = material.iTexDiffuse();
        //        static const int iTexDiffuse = material.m_iTexDiffuse; // question: why not work ?
        iTexDiffuse = material.m_iTexDiffuse;
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
            //            bgfx::setTexture(0, m_sDiffuse, textures.front().textureHandle(), Texture::s_textureSamplerFlags);
            bgfx::setTexture(0, m_sDiffuse, Texture::m_sampleTextures[Texture::CHECKER_BOARD].textureHandle(), Texture::s_textureSamplerFlags);
        }

        //        bgfx::setUniform(m_uTexturesEnable, material.texturesEnable());
        //        const int iTexOpacity = material.iTexOpacity();

        //        const SpotLight& spotLight = spotLights[0];
        //        const std::array<float[4], s_num_vec4_uniforms> buffer = {{

        //        const float buffer[s_num_vec4_material][4] = {
        //            { material.m_diffuse[0], material.m_diffuse[1], material.m_diffuse[2], material.m_diffuse[3] }, // question how do smaller, without glm::vec4
        //            { material.m_specular[0], material.m_specular[1], material.m_specular[2], material.m_specular[3] }, // question how do smaller, without glm::vec4
        //            { material.m_ambient[0], material.m_ambient[1], material.m_ambient[2], material.m_ambient[3] }, // question how do smaller, without glm::vec4
        //            { material.m_shininess, (float)material.m_iTexDiffuse, (float)material.m_iTexOpacity },
        //        };

        //        buffer[0] = material.m_diffuse;
        //        bgfx::setUniform(m_uMaterial, buffer, s_num_vec4_material);
        bgfx::setUniform(m_uMaterial, material.m_data, s_num_vec4_material);

        //        bgfx::setUniform(m_uniformArray, buffer.data(), s_num_vec4_uniforms);

        //        const float temp[s_numDirLightMax][s_num_vec4_dirLight][4] {
        //            {{0.0f, -1.0f, 0.5f, 1.0f},
        //            {1.0f, 1.0f, 1.0f, 0.0f}},
        //            {{0.0f, 0.0f, 0.0f, 0.0f},
        //            {1.0f, 1.0f, 1.0f, 0.0f}}
        //        };
        //        bgfx::setUniform(Program::m_uDirLights, temp, 4);

        //        for (int i = 0; i < s_numDirLightMax; ++i) {
        //            bgfx::setUniform(m_uDirLights[i][0], temp);
        //            bgfx::setUniform(m_uDirLights[i][1], temp);
        //        }
    case Shading::SHADOW:
        const int iTexOpacity = material.m_iTexOpacity;
        if (iTexOpacity >= 0) {
            assert(iTexOpacity < textures.size());
            const Texture& texture = textures[iTexOpacity];

            bgfx::setTexture(1, m_sOpacity, texture.textureHandle(), Texture::s_textureSamplerFlags);
        } else {
            //            bgfx::setTexture(1, m_sOpacity, textures.front().textureHandle(), Texture::s_textureSamplerFlags);
            bgfx::setTexture(1, m_sOpacity, Texture::m_sampleTextures[Texture::CHECKER_BOARD].textureHandle(), Texture::s_textureSamplerFlags);
        }
        break;
    }

    bgfx::submit(id, m_programs[shading], 1.0);
    //    bgfx::submit(id, m_programs[shading], 0, BGFX_DISCARD_NONE | BGFX_CLEAR_DISCARD_COLOR_0);
}

//bgfx::ProgramHandle Program::program() const
//{
//    return m_program;
//}

//const char* s_uniformTypeName[] = {
//    "int",
//    "int",
//    NULL,
//    NULL,
//    "vec4",
//    "float4",
//    "mat3",
//    "float3x3",
//    "mat4",
//    "float4x4",
//};
//BX_STATIC_ASSERT(BX_COUNTOF(s_uniformTypeName) == bgfx::UniformType::Count * 2);

//bx::StringView nextWord(bx::StringView& _parse)
//{
//    bx::StringView word = bx::strWord(bx::strLTrimSpace(_parse));
//    _parse = bx::strLTrimSpace(bx::StringView(word.getTerm(), _parse.getTerm()));
//    return word;
//}

//bgfx::UniformType::Enum nameToUniformTypeEnum(const char* _name)
//{
//    for (uint32_t ii = 0; ii < bgfx::UniformType::Count * 2; ++ii) {
//        if (NULL != s_uniformTypeName[ii]
//            && 0 == bx::strCmp(_name, s_uniformTypeName[ii])) {
//            return bgfx::UniformType::Enum(ii / 2);
//        }
//    }

//    return bgfx::UniformType::Count;
//}

//void strReplace(char* _str, const char* _find, const char* _replace)
//{
//    const int32_t len = bx::strLen(_find);

//    char* replace = (char*)alloca(len + 1);
//    bx::strCopy(replace, len + 1, _replace);
//    for (int32_t ii = bx::strLen(replace); ii < len; ++ii) {
//        replace[ii] = ' ';
//    }
//    replace[len] = '\0';

//    BX_CHECK(len >= bx::strLen(_replace), "");
//    for (bx::StringView ptr = bx::strFind(_str, _find); !ptr.isEmpty(); ptr = bx::strFind(ptr.getPtr() + len, _find)) {
//        bx::memCopy(const_cast<char*>(ptr.getPtr()), replace, len);
//    }
//}

bgfx::ProgramHandle Program::loadProgram(const std::string& shaderName, const char* pShaderDefines)
{
    bgfx::ShaderHandle vsh = loadShader(shaderName, ShaderType::Vertex, pShaderDefines);
    assert(bgfx::isValid(vsh));
    bgfx::setName(vsh, shaderName.c_str());

    bgfx::ShaderHandle fsh = loadShader(shaderName, ShaderType::Fragment, pShaderDefines);
    assert(bgfx::isValid(fsh));
    bgfx::setName(fsh, shaderName.c_str());

    return bgfx::createProgram(vsh, fsh, true);
}

bool callShaderc(const char* pFilename, const char* pOutput, bgfx::RendererType::Enum pRendererType, ShaderType pShaderType, const char* pDefines)
{
#ifdef DEBUG
    std::cout << "\033[35m";
    std::cout << "[program] Compiling '" << pFilename << "' to '" << pOutput << "'" << std::endl;
    std::cout << "\033[0m";
#endif
    //    bool bVertex = !bx::strFind(pFilename, ".vert.sc").isEmpty();
    //    bool bFragment = !bx::strFind(pFilename, ".frag.sc").isEmpty();
    //    if (bVertex || bFragment)
    //    {
    // PATH should be bgfx\examples\runtime
    constexpr int argvSize = 15;
    const char* argv[argvSize] = {
        "shaderc.exe",
        "-f",
        pFilename,
        "-o",
        pOutput,
        "--platform",
        pRendererType == bgfx::RendererType::Direct3D11 ? "windows" : "linux",
        "--type",
        (pShaderType == ShaderType::Vertex) ? "vertex" : "fragment",
        "--profile",
        pRendererType == bgfx::RendererType::Direct3D11
            ? (pShaderType == ShaderType::Vertex)
                ? "vs_5_0"
                : "ps_5_0"
            : "120",
        "-i",
        SHADER_INCLUDE_DIR,
        //        "-O", // DX9, DX11 only
        //        "3", // DX9, DX11 only
        "--define",
        pDefines ? pDefines : ""
    };
    //    const int lenArgv = BX_COUNTOF(argv);

#ifdef USE_SHADERC_EXECUTABLE
    std::string shadercCmd = BGFX_ROOT "/bin/shaderc";
    for (int i = 1; i < argvSize; ++i) {
        if (i == argvSize - 1) {
            shadercCmd += std::string(" \"") + argv[i] + "\"";

        } else {
            shadercCmd += std::string(" ") + argv[i];
        }
    }
#ifdef DEBUG
    std::cout << "EXECUTE: '" << shadercCmd << "'" << std::endl;
#endif
    int error_code = system(shadercCmd.c_str());
#else
        int error_code = shaderc::compileShader(BX_COUNTOF(argv), argv);

#endif
    return error_code == 0;

    //    }

    //    return false;
}

bgfx::ShaderHandle loadShader(const std::string& filename, ShaderType shaderType, const char* pShaderDefines)
//bgfx::ShaderHandle loadShader(const char* filename)
{
    //    const char* shaderPath = "???";
    std::string shaderBinDirPath;
    //    const std::string shaderSrcDirPath = "shaders/src/";

    const auto& rendererType = bgfx::getRendererType();
    //    switch (bgfx::getRendererType()) {
    switch (rendererType) {
    case bgfx::RendererType::Noop:
    case bgfx::RendererType::Direct3D9:
        shaderBinDirPath = "shaders/bin/dx9/";
        //        shaderSrcDirPath = "shaders/src/dx9/";
        break;
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12:
        shaderBinDirPath = "shaders/bin/dx11/";
        //        shaderSrcDirPath = "shaders/src/dx11/";
        break;
    case bgfx::RendererType::Gnm:
        shaderBinDirPath = "shaders/bin/pssl/";
        //        shaderSrcDirPath = "shaders/src/pssl/";
        break;
    case bgfx::RendererType::Metal:
        shaderBinDirPath = "shaders/bin/metal/";
        //        shaderSrcDirPath = "shaders/src/metal/";
        break;
    case bgfx::RendererType::OpenGL:
        shaderBinDirPath = "shaders/bin/glsl/";
        //        shaderSrcDirPath = "shaders/src/glsl/";
        break;
    case bgfx::RendererType::OpenGLES:
        shaderBinDirPath = "shaders/bin/essl/";
        //        shaderSrcDirPath = "shaders/src/essl/";
        break;
    case bgfx::RendererType::Vulkan:
        shaderBinDirPath = "shaders/bin/spirv/";
        //        shaderSrcDirPath = "shaders/src/spirv/";
        break;
    }

    //    std::string filePath = PROJECT_DIR + shaderBinDirPath + filename + ".bin";
    //    const bgfx::Memory* mem;
    //    const std::string srcShaderName = filename
    //        + (shaderType == ShaderType::Vertex ? ".vert" : ".frag");
    //    const std::string srcFilePath = PROJECT_DIR + shaderSrcDirPath + srcShaderName + ".sc";
    const std::string srcFilePath = PROJECT_DIR "shaders/src/"
        + filename + (shaderType == ShaderType::Vertex ? ".vert" : ".frag") + ".sc";
    std::ifstream file;
    file.open(srcFilePath);
    if (!file.is_open()) {
        std::cout << "[program] failed to load '" << srcFilePath << "'" << std::endl;
        exit(1);
    }
    std::stringstream strStream;
    strStream << file.rdbuf();
    file.close();
    //    const uint32_t srcShaderNameHash = bx::hash<bx::HashMurmur2A>(srcShaderName);
    const uint32_t srcShaderHash = bx::hash<bx::HashMurmur2A>(strStream.str().c_str());
    //    std::ifstream file;
    //    std::string typeStr = ShaderType::Vertex ? ".vert" : ".frag";
    const uint32_t shaderDefinesHash = bx::hash<bx::HashMurmur2A>(pShaderDefines);
    const std::string binShaderName = filename
        + "." + std::to_string(srcShaderHash)
        + "." + std::to_string(shaderDefinesHash)
        + (shaderType == ShaderType::Vertex ? ".vert" : ".frag");

    //    std::string typeStr = "";

    bool binFile = false;
    std::string binFilePath = PROJECT_DIR + shaderBinDirPath + binShaderName + ".glsl";
    file = std::ifstream(binFilePath, std::ios::binary);
    if (!file.is_open()) {
        //        file.close();

        binFilePath = PROJECT_DIR + shaderBinDirPath + binShaderName + ".bin";
        file = std::ifstream(binFilePath, std::ios::binary);
        if (!file.is_open()) {
            //            std::cout << "[main] Failed to load '" << filePath << "' '" << shaderName << "'"
            //                      << std::endl;
            bool ret = callShaderc(srcFilePath.c_str(), binFilePath.c_str(), rendererType, shaderType, pShaderDefines);
            assert(ret);

            file = std::ifstream(binFilePath, std::ios::binary);
            if (!file.is_open()) {
                std::cout << "[main] Failed to load '" << binFilePath << "' '" << binShaderName << "'"
                          << std::endl;
                exit(1);
            }
            //            file.close();
            //            exit(1);
            //            return BGFX_INVALID_HANDLE;
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
        constexpr size_t maxBuffSize = 99999;
        assert(maxBuffSize > fileSize);
        char code[maxBuffSize];
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
    bgfx::setName(handle, binShaderName.c_str());
    //    std::cout << "[program] createShader " << filename << std::endl;

    return handle;
    //    return bgfx::createShader(mem);
}

// ------------------------------------ GETTERS AND SETTERS
//bgfx::ProgramHandle Program::handleProgram(const Shading &shading)
//{
//    return m_programs[shading];
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
