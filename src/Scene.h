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

private:
    bgfx::VertexLayout m_layout;
    std::vector<Object> m_objects;

//    std::vector<Material> m_materials;
    Materials m_materials;
//    std::vector<Texture> m_textures;
    Textures m_textures;

    bgfx::UniformHandle m_uDiffuse;


};

#endif // SCENE_H
