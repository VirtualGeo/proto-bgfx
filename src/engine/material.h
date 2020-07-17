#ifndef MATERIAL_H
#define MATERIAL_H

#include <tiny_obj_loader.h>
#include "texture.h"
#include <vector>
#include <bx/file.h>

class Material
{
public:
    static std::vector<Texture> s_textures;

public:
    Material(const tinyobj::material_t & tinyobjMaterial, const std::string & baseDir);
    Material(const Material & file) = delete;
//    Material(Material &&) noexcept = default;
    Material(Material && material);
    ~Material();

    Material(std::ifstream & file);
    void save(std::ofstream & file) const;

    void updateData();
    void submit() const;


    friend std::ostream & operator <<(std::ostream & os, const Material & material);

//private:
    std::string m_name;
//    std::vector<uint> m_iTextures[Texture::size];
    int m_iTexDiffuse = -1;

    float m_diffuse[4] = {0.0f};
    float m_specular[4] = {0.0f};
    float m_ambient[4] = {0.0f};
//    bx::Vec3 m_colorDiffuse;
    float m_shininess;

//    float m_texturesEnable[4] = {0.0f};
    int m_iTexOpacity = -1;

    float m_data[4][4];

    static bgfx::UniformHandle s_sDiffuseUH;
    static bool s_initialized;

//public: // ------------------------- getters
//    const float * diffuse() const;
//    const float * texturesEnable() const;
//    int iTexDiffuse() const;
//    int iTexOpacity() const;
//    std::string name() const;


};

using Materials = std::vector<Material>;

#endif // MATERIAL_H
