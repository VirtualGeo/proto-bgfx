#include "Material.h"

#include <cassert>
//Material::Material()
//{

#include "FileSystem.h"
//}
#include "System.h"

Material::Material(const tinyobj::material_t& material, Textures& textures, const std::string& baseDir)
{
    m_name = material.name;

    //        float diffuse[3];
    for (size_t i = 0; i < 3; i++) {
        m_diffuse[i] = material.diffuse[i];
    }
    //    std::cout << m_diffuse[0] << m_diffuse[1] << m_diffuse[2] << std::endl;
    //    m_diffuse[0] = 0.0f;
    //    m_diffuse[1] = 1.0f;
    //    m_diffuse[2] = 1.0f;

    const std::string& diffuseTexName = material.diffuse_texname;

    if (diffuseTexName.length() > 0) {
        m_diffuse[3] = 1.0f;
        // Only load the texture if it is not already loaded
        //        for (const Texture& texture : textures) {
        for (int i = 0; i < textures.size(); ++i) {
            const Texture& texture = textures[i];

            if (texture.name() == diffuseTexName) {
                m_iTexDiffuse = i;
                std::cout << "        [Texture] Already loaded texture: '" << diffuseTexName << "' " << std::endl;
                return;
            }
        }
        //        if (textures.find(diffuseTexName) == textures.end()) {
        //            textures.insert(std::make_pair(material->diffuse_texname, texture_id));

        //            textures.insert(std::make_pair(diffuseTexName, Texture(diffuseTexName, baseDir)));
        //            textures.insert(std::make_pair(diffuseTexName, {diffuseTexName, baseDir}));
        m_iTexDiffuse = static_cast<int>(textures.size());
        //        textures.push_back(Texture(diffuseTexName, baseDir));
        //        textures.emplace_back(Texture(diffuseTexName, baseDir));
        textures.emplace_back(diffuseTexName, baseDir);
        //        }
    }
    else {
        m_diffuse[3] = 0.0f;
    }
}

Material::~Material()
{
    std::cout << "\033[31m";
    std::cout << "[Material] '" << m_name << "' deleted " << this << std::endl;
    std::cout << "\033[0m";
}

Material::Material(std::ifstream &file)
{
    FileSystem::read(m_name, file);
    FileSystem::read(m_iTexDiffuse, file);
    FileSystem::read(m_diffuse, 4, file);
}

void Material::save(std::ofstream &file)
{
    FileSystem::write(m_name, file);
    FileSystem::write(m_iTexDiffuse, file);
    FileSystem::write(m_diffuse, 4, file);
}

Material::Material(Material&& material)
    : m_name(std::move(material.m_name))
    , m_iTexDiffuse(material.m_iTexDiffuse)
{
    std::cout << "\033[34m";
    std::cout << "[Material] " << this << " '" << m_name << "' right moving from " << &material << std::endl;
    std::cout << "\033[0m";
}

const float* Material::diffuse() const
{
//    float ret[4];
//    for (int i =0; i <3; ++i) {
//        ret[i] = i;
//    }
//    ret[3] = (m_iTexDiffuse >= 0) ?(1.0) :(0.0);
    return m_diffuse;
}

int Material::iTexDiffuse() const
{
    return m_iTexDiffuse;
}
