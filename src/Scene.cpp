#include "Scene.h"

#include "TimerUtil.h"
#include <cassert>
#include <iostream>
#include <stb/stb_image.h>

#include "System.h"

Scene::Scene()
{
    m_layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        // .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .end();
}

void Scene::addModel(const char* filename)
{
    std::string absoluteFilename(std::string(PROJECT_DIR) + filename);

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    //    std::map<std::string, uint> textures;

    timerutil tm;
    tm.start();

    std::string base_dir = GetBaseDir(absoluteFilename);
    if (base_dir.empty()) {
        base_dir = ".";
    }
#ifdef _WIN32
    base_dir += "\\";
#else
    base_dir += "/";
#endif

    std::string warn;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, absoluteFilename.c_str(),
        base_dir.c_str());
    if (!warn.empty()) {
        std::cout << "[Scene] WARN: " << warn << std::endl;
    }
    if (!err.empty()) {
        std::cerr << "[Scene] " << err << std::endl;
    }

    tm.end();

    if (!ret) {
        std::cerr << "[Scene] Failed to load " << absoluteFilename << std::endl;
        //        throw std::runtime_error("");
        exit(1);
        //        return;
    }

    printf("[Scene] Parsing time: %d [ms]\n", (int)tm.msec());

    printf("[Scene] # of vertices  = %d\n", (int)(attrib.vertices.size()) / 3);
    printf("[Scene] # of normals   = %d\n", (int)(attrib.normals.size()) / 3);
    printf("[Scene] # of texcoords = %d\n", (int)(attrib.texcoords.size()) / 2);
    printf("[Scene] # of materials = %d\n", (int)materials.size());
    printf("[Scene] # of shapes    = %d\n", (int)shapes.size());

    // Append `default` material
//    materials.push_back(tinyobj::material_t());

//    for (size_t i = 0; i < materials.size(); i++) {
//        printf("material[%d].diffuse_texname = %s\n", int(i),
//            materials[i].diffuse_texname.c_str());
//    }

    // Load diffuse textures
    //    {
//    for (const tinyobj::material_t& material : materials) {

    for (size_t i = 0; i < materials.size(); i++) {
        const tinyobj::material_t & material = materials[i];

        std::cout << "[Scene] Load material[" << i << "] " << material.name << std::endl;
//        printf("material[%d].difname = %s\n", int(i),
//            material.name.c_str());
        m_materials.push_back(Material(material, m_textures, base_dir));
    }
    assert(materials.size() == m_materials.size());

    for (const tinyobj::shape_t & shape : shapes) {
        m_objects.push_back(Object(shape));
    }
    //        for (size_t m = 0; m < materials.size(); m++) {
    //            tinyobj::material_t* mp = &materials[m];
    //    }
    //    }
}

void Scene::submit(bgfx::ViewId id, bgfx::ProgramHandle program, const float* mtx, uint64_t state)
{
    //    for (const Object & object : m_objects) {

    //		bgfx::setTransform(mtx);
    //		bgfx::setState(state);

    //		bgfx::setUniform(group.m_uDiffuse, group.m_diffuse);

    //		if (group.m_texture != nullptr) {
    //			// uint32_t flags = UINT32_MAX;
    //			// uint32_t flags = BGFX_TEXTURE_USE_DEFAULT;

    //			// const uint64_t textureFlags =
    //				// 0 | BGFX_CAPS_FORMAT_TEXTURE_MIP_AUTOGEN;
    //			// const uint64_t textureFlags = 0 | BGFX_TEXTURE_NONE;
    //			// const uint64_t samplerFlags = 0 | BGFX_SAMPLER_NONE;
    //			bgfx::setTexture(0, group.m_texture->m_sampler,
    //							 group.m_texture->m_texture);
    //							//  textureFlags | samplerFlags);

    //			// bgfx::setUniform(group.m_uHasDiffuseTexture, (void*)true);
    //		}
    //		// else {
    //		// bgfx::setTexture(0, group.m_texture->m_sampler, bgfx::Tex)
    //		// }

    //		// }
    //		// else {
    //		//     continue;
    //		// }

    //		bgfx::setIndexBuffer(group.m_ibh);
    //		bgfx::setVertexBuffer(0, group.m_vbh);
    //		// bgfx::submit(id, program, 0, it != itEnd - 1);
    //		bgfx::submit(id, program);
    //    }
}
