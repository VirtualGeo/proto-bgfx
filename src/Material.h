#ifndef MATERIAL_H
#define MATERIAL_H

#include <tiny_obj_loader.h>
#include "Texture.h"

class Material
{
public:
    Material(const tinyobj::material_t & tinyobjMaterial, Textures & textures, const std::string & baseDir);
};

using Materials = std::vector<Material>;

#endif // MATERIAL_H
