#include "Material.h"

#include <cassert>
#include "FileSystem.h"
#include "System.h"
//#include <iostream>
//#include <stdio.h>
//#include <string>
//#include <iostream>
//#include <cstdint>
#include <cstring>

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

Material::Material(const tinyobj::material_t& material, Textures& textures, const std::string& baseDir)
{
    //    for (int i =0; i <4; ++i) {
    //        m_activeTextures[i] = 0.0f;
    //    }
    m_name = material.name;

    //        float diffuse[3];
//    for (size_t i = 0; i < 3; i++) {
//        m_diffuse[i] = material.diffuse[i];
//    }
    memcpy(m_diffuse, material.diffuse, 3 * sizeof(float));
//    m_diffuse = material.diffuse;
//    std::memcpy();
    //    std::cout << m_diffuse[0] << m_diffuse[1] << m_diffuse[2] << std::endl;
    //    m_diffuse[0] = 0.0f;
    //    m_diffuse[1] = 1.0f;
    //    m_diffuse[2] = 1.0f;

    const std::string& diffuseTexName = material.diffuse_texname;

    if (diffuseTexName.length() > 0) {
        m_diffuse[3] = 1.0f;
        // Only load the texture if it is not already loaded
        //        for (const Texture& texture : textures) {
        int i;
        if (hasLoaded(diffuseTexName, textures, i)) {
            m_iTexDiffuse = i;
#ifdef DEBUG
            std::cout << "        [Material] Already loaded texture: '" << diffuseTexName << "' " << std::endl;
#endif
            //            return;
        } else {
//            std::cout << "        [Material] Load texture: '" << diffuseTexName << "' " << std::endl;
//    std::cout << "'" << m_name << "', width=" << m_width << ", height=" << m_height << ", nbChannel=" << m_nChannels << ", textureSize=" << m_textureSize << ", bitPerPixel=" << m_nChannels << std::endl;
            m_iTexDiffuse = static_cast<int>(textures.size());
            textures.emplace_back(diffuseTexName, baseDir);
        }
    }
    //    } else {
    //        m_diffuse[3] = 0.0f;
    //    }

    const std::string& opacityTexName = material.alpha_texname;
    if (opacityTexName.length() > 0) {
        m_texturesEnable[0] = 1.0f;

        int i;
        if (hasLoaded(opacityTexName, textures, i)) {
            m_iTexOpacity = i;
#ifdef DEBUG
            std::cout << "        [Material] Already loaded texture: '" << diffuseTexName << "' " << std::endl;
#endif
        }
        else {
            m_iTexOpacity = static_cast<int>(textures.size());
            textures.emplace_back(opacityTexName, baseDir);
        }
    }
}

Material::Material(Material&& material)
    : m_name(std::move(material.m_name))
    , m_iTexDiffuse(material.m_iTexDiffuse)
{
#ifdef DEBUG
    std::cout << "\033[34m";
    std::cout << "[Material] " << this << " '" << m_name << "' right moving from " << &material << std::endl;
    std::cout << "\033[0m";
#endif
}

Material::~Material()
{
#ifdef DEBUG
    std::cout << "\033[31m";
    std::cout << "[Material] '" << m_name << "' deleted " << this << std::endl;
    std::cout << "\033[0m";
#endif
}

Material::Material(std::ifstream& file)
{
    FileSystem::read(m_name, file);
    FileSystem::read(m_iTexDiffuse, file);
    FileSystem::read(m_diffuse, 4, file);

    FileSystem::read(m_texturesEnable, 4, file);
    FileSystem::read(m_iTexOpacity, file);
}

void Material::save(std::ofstream& file) const
{
    FileSystem::write(m_name, file);
    FileSystem::write(m_iTexDiffuse, file);
    FileSystem::write(m_diffuse, 4, file);

    FileSystem::write(m_texturesEnable, 4, file);
    FileSystem::write(m_iTexOpacity, file);
}


std::ostream &operator <<(std::ostream &os, const Material &material)
{
    return os << "'" << material.m_name << "'";
}

// -------------------------- GETTERS
const float* Material::diffuse() const
{
    //    float ret[4];
    //    for (int i =0; i <3; ++i) {
    //        ret[i] = i;
    //    }
    //    ret[3] = (m_iTexDiffuse >= 0) ?(1.0) :(0.0);
    return m_diffuse;
}

const float *Material::texturesEnable() const
{
    return m_texturesEnable;
}

int Material::iTexDiffuse() const
{
    return m_iTexDiffuse;
}

int Material::iTexOpacity() const
{
    return m_iTexOpacity;
}

std::string Material::name() const
{
    return m_name;
}
