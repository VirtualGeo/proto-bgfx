#ifndef SCENE_H
#define SCENE_H

#include <bgfx/bgfx.h>
#include "object.h"
#include <vector>
#include <tiny_obj_loader.h>
#include "material.h"
#include "texture.h"
#include "program.h"

#include "dirLight.h"
//#include <engine/camera.h>
#include "camera.h"
#include "spotlight.h"

class Scene
{
public:
    Scene();
//    void addModel(const char * filename);
    void addModel(const std::string &filename);
//    void draw()
    void draw(const bgfx::ViewId id, const Shading &shading, const float *mtx,
                const uint64_t state, const Camera & camera, float ratio) const;
    void clear(); // clear scene (bgfx::destroy(handle)) before bgfx::shutdown() from main shutdown()

    void load(std::ifstream & file);
    void save(std::ofstream & file) const;

    void printStats(int &line);
//    void addLight(Light && light);
    void addLight(SpotLight && spotLight);

private:
    bgfx::VertexLayout m_layout;
    std::vector<Object> m_objects;

//    std::vector<Material> m_materials;
    Materials m_materials;
//    std::vector<Texture> m_textures;
    Textures m_textures;

//    DirLight m_dirLight;
//    Lights m_lights;
    SpotLights m_spotLights;

//    bgfx::UniformHandle m_uDiffuse;
//    bgfx::UniformHandle m_samplerDiffuse;
//    bgfx::UniformHandle m_uHasDiffuseTexture;

private:
//    int m_parsingTime;
//    int m_loadingMaterialsTime;
//    int m_loadingObjectsTime;

    void updateStats();
    size_t m_nbVertices;
    size_t m_nbTriangles;
    size_t m_nbObjects;
    size_t m_nbMeshes;
    float m_texturesSize;
    size_t m_nbTextures;
    int m_parsingTime;
    int m_loadingMaterialsTime;
    int m_loadingObjectsTime;
    int m_totalLoadingTime;
    size_t m_nbVertexBuffer;
    size_t m_nbIndexBuffer;

//    uint m_nbVertices;
//    uint m_nbFaces;
//    uint m_nbTriangles;

public: // ----------------------- getters
//    size_t nbVertices() const;
//    size_t nbTriangles() const;
//    size_t nbObjects() const;
//    size_t texturesSize() const;
//    size_t nbVertexBuffer() const;
//    size_t nbIndexBuffer() const;
//    size_t nbTextures() const;

//    int parsingTime() const;
//    int loadingMaterialsTime() const;
//    int loadingObjectsTime() const;
};

#endif // SCENE_H
