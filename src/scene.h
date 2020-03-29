#ifndef SCENE_H
#define SCENE_H

#include <bgfx/bgfx.h>
#include "object.h"
#include <vector>
#include <tiny_obj_loader.h>
#include "material.h"
#include "texture.h"
#include "program.h"

class Scene
{
public:
    Scene();
//    void addModel(const char * filename);
    void addModel(const std::string &filename);
//    void draw()
    void draw(const bgfx::ViewId id, const Program & program, const float *mtx,
                const uint64_t state) const;
    void clear(); // clear scene (bgfx::destroy(handle)) before bgfx::shutdown() from main shutdown()

    void load(std::ifstream & file);
    void save(std::ofstream & file) const;

private:
    bgfx::VertexLayout m_layout;
    std::vector<Object> m_objects;

//    std::vector<Material> m_materials;
    Materials m_materials;
//    std::vector<Texture> m_textures;
    Textures m_textures;

//    bgfx::UniformHandle m_uDiffuse;
//    bgfx::UniformHandle m_samplerDiffuse;
//    bgfx::UniformHandle m_uHasDiffuseTexture;

private:
    int m_parsingTime;
    int m_loadingMaterialsTime;
    int m_loadingObjectsTime;

//    uint m_nbVertices;
//    uint m_nbFaces;
//    uint m_nbTriangles;

public: // ----------------------- getters
    size_t nbVertices() const;
    size_t nbTriangles() const;
    size_t nbObjects() const;
    size_t texturesSize() const;
    size_t nbVertexBuffer() const;
    size_t nbIndexBuffer() const;
    size_t nbTextures() const;

    int parsingTime() const;
    int loadingMaterialsTime() const;
    int loadingObjectsTime() const;
};

#endif // SCENE_H