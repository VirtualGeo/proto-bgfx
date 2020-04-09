#ifndef OBJECT_H
#define OBJECT_H

//#include <vector>
#include <bgfx/bgfx.h>
#include <tiny_obj_loader.h>
//#include <memory>
#include "material.h"
#include "mesh.h"
#include "program.h"
#include "vertex.h"
#include "dirLight.h"
//#include <engine/camera.h>
#include "camera.h"

class Object {
public:
    Object(const tinyobj::shape_t& shape, const tinyobj::attrib_t& attrib, const std::vector<tinyobj::material_t>& materials, const size_t iShape, const bgfx::VertexLayout& layout);
    Object(const Object&) = delete; // copy constructor
    Object(Object&& object);
    ~Object();

    Object(std::ifstream& file, const size_t iShape, const bgfx::VertexLayout& layout);
    void save(std::ofstream& file) const;

    //    void draw() const;
    //    void draw(bgfx::ViewId id, bgfx::ProgramHandle program, const float *mtx,
    //                uint64_t state) const;
    void draw(const bgfx::ViewId id, const Program& program, const float* mtx,
        const uint64_t state, const Materials& materials, const Textures& textures, const DirLight & dirLight, const Camera & camera) const;

    friend std::ostream& operator<<(std::ostream& os, const Object& object);

private:
    bgfx::VertexBufferHandle m_vbh;
    //    std::unique_ptr<bgfx::VertexBufferHandle> m_vbh;
    std::vector<Vertex> m_vertices;
    std::vector<Mesh> m_meshes;

    size_t m_nbTriangles;

    float m_bmin[3];
    float m_bmax[3];
    std::string m_name;

public: // --------------------------------------- getters
    const bgfx::VertexBufferHandle& vbh() const;
    //    bgfx::IndexBufferHandle ibh() const;

    //    size_t iMaterial() const;
    size_t nbVertices() const;

    size_t nbTriangles() const;
    size_t nbMeshes() const;
    std::string name() const;
};

//using Objects = std::vector<Object>;

#endif // OBJECT_H
