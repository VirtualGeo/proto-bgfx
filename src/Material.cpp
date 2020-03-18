#include "Material.h"

#include <cassert>
//Material::Material()
//{

//}
#include "System.h"

Material::Material(const tinyobj::material_t& material, Textures& textures, const std::string& baseDir)
{
    const std::string & diffuseTexName = material.diffuse_texname;

    if (diffuseTexName.length() > 0) {
        // Only load the texture if it is not already loaded
        if (textures.find(diffuseTexName) == textures.end()) {
//            textures.insert(std::make_pair(material->diffuse_texname, texture_id));

            textures.insert(std::make_pair(diffuseTexName, Texture(diffuseTexName, baseDir)));
        }
    }
}
