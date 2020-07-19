#include "material.h"

#include "fileIO.h"
#include "system.h"
#include <algorithm>
#include <cassert>
#include <cstring>

std::vector<Texture> Material::s_textures;
bgfx::UniformHandle Material::s_sDiffuseUH = BGFX_INVALID_HANDLE;
bool Material::s_initialized = false;

bool hasLoaded(const std::string& texName, const Textures& textures, int& iTex)
{

    for (int i = 0; i < textures.size(); ++i) {
        const Texture& texture = textures[i];

        if (texture.name() == texName) {
            iTex = i;
            return true;
        }
    }
    return false;
}

Material::Material(const tinyobj::material_t& material, const std::string& baseDir)
{
    if (! s_initialized) {
        s_sDiffuseUH = bgfx::createUniform("s_diffuse", bgfx::UniformType::Sampler);
//        s_textures.reserve(100);
        s_initialized = true;
    }

    m_name = material.name;
//    std::cout << "        [Material] Loading material: '" << m_name << "' " << std::endl;

    memcpy(m_diffuse, material.diffuse, 3 * sizeof(float));
    memcpy(m_specular, material.specular, 3 * sizeof(float));
    memcpy(m_ambient, material.ambient, 3 * sizeof(float));
    m_shininess = material.shininess;

    std::string diffuseTexName = material.diffuse_texname;
    //    material.diffuse_texopt.;
//        std::cout << "        [Material] Loading texture: '" << diffuseTexName << "' " << std::endl;
    std::replace(diffuseTexName.begin(), diffuseTexName.end(), '\\', '/');

//    assert(diffuseTexName.length() > 0);
    if (diffuseTexName.length() > 0) {
        m_diffuse[3] = 1.0f;
        // Only load the texture if it is not already loaded
        //        for (const Texture& texture : textures) {
        int i;
        if (hasLoaded(diffuseTexName, s_textures, i)) {
            m_iTexDiffuse = i;
#ifdef MODEL_LOADER_INFO
            std::cout << "        [Material] Already loaded texture: '" << diffuseTexName << "' " << std::endl;
#endif
            //            return;
        } else {
            //            std::cout << "        [Material] Load texture: '" << diffuseTexName << "' " << std::endl;
            //    std::cout << "'" << m_name << "', width=" << m_width << ", height=" << m_height << ", nbChannel=" << m_nChannels << ", textureSize=" << m_textureSize << ", bitPerPixel=" << m_nChannels << std::endl;
            m_iTexDiffuse = static_cast<int>(s_textures.size());
            s_textures.emplace_back(diffuseTexName, baseDir, Texture::Type::DIFFUSE);
        }
    }
    //    } else {
    //        m_diffuse[3] = 0.0f;
    //    }

    std::string opacityTexName = material.alpha_texname;
    std::replace(opacityTexName.begin(), opacityTexName.end(), '\\', '/');
//    assert(opacityTexName.length() > 0);
    if (opacityTexName.length() > 0) {
        //        m_texturesEnable[0] = 1.0f;

        int i;
        if (hasLoaded(opacityTexName, s_textures, i)) {
            m_iTexOpacity = i;
#ifdef MODEL_LOADER_INFO
            std::cout << "        [Material] Already loaded texture: '" << diffuseTexName << "' " << std::endl;
#endif
        } else {
            m_iTexOpacity = static_cast<int>(s_textures.size());
            s_textures.emplace_back(opacityTexName, baseDir, Texture::Type::OPACITY);
        }
    }

    updateData();
}

Material::Material(Material&& material)
    : m_name(std::move(material.m_name))
    , m_iTexDiffuse(material.m_iTexDiffuse)
    , m_diffuse { material.m_diffuse[0], material.m_diffuse[1], material.m_diffuse[2], material.m_diffuse[3] }
    , m_specular { material.m_specular[0], material.m_specular[1], material.m_specular[2], material.m_specular[3] }
    , m_ambient { material.m_ambient[0], material.m_ambient[1], material.m_ambient[2], material.m_ambient[3] }
    , m_shininess(material.m_shininess)
    //    , m_texturesEnable{material.m_texturesEnable[0], material.m_texturesEnable[1], material.m_texturesEnable[2], material.m_texturesEnable[3]}
    , m_iTexOpacity(material.m_iTexOpacity)
//    , m_data(std::move(material.m_data))
//    , m_diffuse(std::move(*material.m_diffuse))
// TODO : copy colors
{
    memcpy(m_data, material.m_data, sizeof(m_data));

#ifdef DEBUG
    std::cout << "\033[34m";
    std::cout << "[Material] " << this << " '" << m_name << "' right moving from " << &material << std::endl;
    std::cout << "\033[0m";
#endif
}

Material::~Material()
{
    //#ifdef DEBUG
    //    std::cout << "\033[31m";
    //    std::cout << "[Material] '" << m_name << "' deleted " << this << std::endl;
    //    std::cout << "\033[0m";
    //#endif
}

Material::Material(std::ifstream& file)
{
    if (! s_initialized) {
        s_sDiffuseUH = bgfx::createUniform("s_diffuse", bgfx::UniformType::Sampler);
//        s_textures.reserve(100);
        s_initialized = true;
    }

    FileIO::read(m_name, file);
    FileIO::read(m_iTexDiffuse, file);
    FileIO::read(m_diffuse, 4, file);
    FileIO::read(m_specular, 4, file);
    FileIO::read(m_ambient, 4, file);
    FileIO::read(m_shininess, file);

    //    FileIO::read(m_texturesEnable, 4, file);
    FileIO::read(m_iTexOpacity, file);

    updateData();
}

void Material::save(std::ofstream& file) const
{
    FileIO::write(m_name, file);
    FileIO::write(m_iTexDiffuse, file);
    FileIO::write(m_diffuse, 4, file);
    FileIO::write(m_specular, 4, file);
    FileIO::write(m_ambient, 4, file);
    FileIO::write(m_shininess, file);

    //    FileIO::write(m_texturesEnable, 4, file);
    FileIO::write(m_iTexOpacity, file);
}

void Material::updateData()
{

    const float temp[4][4] = {
        { m_diffuse[0], m_diffuse[1], m_diffuse[2], m_diffuse[3] }, // question how do smaller, without glm::vec4
        { m_specular[0], m_specular[1], m_specular[2], m_specular[3] }, // question how do smaller, without glm::vec4
        { m_ambient[0], m_ambient[1], m_ambient[2], m_ambient[3] }, // question how do smaller, without glm::vec4
        { m_shininess, (float)m_iTexDiffuse, (float)m_iTexOpacity }
    };
    //    m_data = std::move(temp);
    memcpy(m_data, temp, sizeof(m_data));
}

void Material::submit() const
{
//    assert(0 <= m_iTexDiffuse && m_iTexDiffuse < s_textures.size());
    if (m_iTexDiffuse >= 0) {
        assert(m_iTexDiffuse < s_textures.size());
        const Texture& texture = s_textures[m_iTexDiffuse];

        //        float vec4[4] = { 1.0f };
        //        bgfx::setUniform(m_hasDiffuseTexture, vec4);

        assert(bgfx::isValid(texture.textureHandle()));
        bgfx::setTexture(0, s_sDiffuseUH, texture.textureHandle());
//        std::cout << texture << std::endl;

//    } else {
//        //        float vec4[4] = { 0.0f };
//        //        bgfx::setUniform(m_hasDiffuseTexture, vec4);

//        bgfx::setTexture(0, s_sDiffuseUH, Texture::m_sampleTextures[Texture::CHECKER_BOARD].textureHandle(),
//                Texture::s_textureSamplerFlags);
    }

    //    bgfx::setUniform(m_uMaterial, material.m_data, s_num_vec4_material);
}

std::ostream& operator<<(std::ostream& os, const Material& material)
{
    return os << "'" << material.m_name << "'";
}

// -------------------------- GETTERS
//const float* Material::diffuse() const
//{
//    //    float ret[4];
//    //    for (int i =0; i <3; ++i) {
//    //        ret[i] = i;
//    //    }
//    //    ret[3] = (m_iTexDiffuse >= 0) ?(1.0) :(0.0);
//    return m_diffuse;
//}

//const float *Material::texturesEnable() const
//{
//    return m_texturesEnable;
//}

//int Material::iTexDiffuse() const
//{
//    return m_iTexDiffuse;
//}

//int Material::iTexOpacity() const
//{
//    return m_iTexOpacity;
//}

//std::string Material::name() const
//{
//    return m_name;
//}
