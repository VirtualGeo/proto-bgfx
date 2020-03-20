#ifndef SCENE_H
#define SCENE_H

#include <bgfx/bgfx.h>
#include "Object.h"
#include <vector>
#include <tiny_obj_loader.h>
#include "Material.h"
#include "Texture.h"

class Scene
{
public:
    Scene();
    void addModel(const char * filename);
//    void draw()
    void submit(bgfx::ViewId id, bgfx::ProgramHandle program, const float *mtx,
                uint64_t state);
    void clear(); // clear scene (bgfx::destroy(handle)) before bgfx::shutdown() from main shutdown()
    size_t nbVertices() const;
    size_t nbTriangles() const;
    size_t nbObjects() const;
    size_t texturesSize() const;

    int parsingTime() const;
    void setParsingTime(int parsingTime);

    int loadingMaterialsTime() const;

    int loadingObjectsTime() const;

private:
    bgfx::VertexLayout m_layout;
    std::vector<Object> m_objects;

//    std::vector<Material> m_materials;
    Materials m_materials;
//    std::vector<Texture> m_textures;
    Textures m_textures;

    bgfx::UniformHandle m_uDiffuse;
    bgfx::UniformHandle m_samplerDiffuse;
    bgfx::UniformHandle m_uHasDiffuseTexture;



private:
    int m_parsingTime;
    int m_loadingMaterialsTime;
    int m_loadingObjectsTime;

//    uint m_nbVertices;
//    uint m_nbFaces;
//    uint m_nbTriangles;

};

#endif // SCENE_H
