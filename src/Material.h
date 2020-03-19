#ifndef MATERIAL_H
#define MATERIAL_H

#include <tiny_obj_loader.h>
#include "Texture.h"
#include <vector>

class Material
{
public:
    Material(const tinyobj::material_t & tinyobjMaterial, Textures & textures, const std::string & baseDir);
    ~Material();

    Material(const Material &) = delete;
//    Material(Material &&) noexcept = default;
    Material(Material &&);

    const float * diffuse() const;

private:
    std::string m_name;
//    std::vector<uint> m_iTextures[Texture::size];
    int m_iTexDiffuse = -1;

    float m_diffuse[3];

};

using Materials = std::vector<Material>;

#endif // MATERIAL_H
