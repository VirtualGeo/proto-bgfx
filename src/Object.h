#ifndef OBJECT_H
#define OBJECT_H

//#include <vector>
#include <bgfx/bgfx.h>
#include <tiny_obj_loader.h>
//#include <memory>
#include "Vertex.h"
#include "Mesh.h"
#include "Material.h"
#include "Program.h"

//namespace  { // question : why without build failed ?

//struct Vertex {
//    float x;
//    float y;
//    float z;
//    float nx;
//    float ny;
//    float nz;
//    float tx;
//    float ty;

//    static void init()
//    {

//        m_layout.begin()
//            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
//            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
//            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
//            // .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
//            .end();
//    }

//    static bgfx::VertexLayout m_layout;
//};
////}

//bgfx::VertexLayout Vertex::m_layout;
//bgfx::VertexLayout Vertex::m_layout = bgfx::VertexLayout();

class Object
{
public:
    Object(const tinyobj::shape_t & shape, const tinyobj::attrib_t & attrib, const std::vector<tinyobj::material_t> & materials, const size_t iShape, const bgfx::VertexLayout & layout);
//    Object(std::ifstream & file);
    Object(std::ifstream &file, const size_t iShape, const bgfx::VertexLayout &layout);
    void save(std::ofstream & file);

    Object(const Object &) = delete; // copy constructor
    Object(Object && object);
    ~Object();

//    void draw() const;
//    void draw(bgfx::ViewId id, bgfx::ProgramHandle program, const float *mtx,
//                uint64_t state) const;
    void draw(const bgfx::ViewId id, const Program & program, const float *mtx,
                const uint64_t state, const Materials & materials, const Textures & textures) const;

    const bgfx::VertexBufferHandle & vbh() const;
//    bgfx::IndexBufferHandle ibh() const;

//    size_t iMaterial() const;
    size_t nbVertices() const;

    size_t nbTriangles() const;
    size_t nbMeshes() const;
    std::string name() const;
    friend std::ostream & operator <<(std::ostream & os, const Object & object);

private:
    bgfx::VertexBufferHandle m_vbh;
//    std::unique_ptr<bgfx::VertexBufferHandle> m_vbh;
    std::vector<Vertex> m_vertices;
    std::vector<Mesh> m_meshes;

    size_t m_nbTriangles;

    float m_bmin[3];
    float m_bmax[3];
    std::string m_name;
};

//using Objects = std::vector<Object>;


#endif // OBJECT_H
