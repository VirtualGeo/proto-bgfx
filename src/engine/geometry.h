#ifndef GEOMETRY_H
#define GEOMETRY_H

//#include "program.h"
#include "vertex.h"

class Geometry
{
public:
    Geometry() = delete;
    ~Geometry() = delete;

    static void init();
    static void clear();
    static void drawQuad();
    static void drawCube();
    static void drawCube(const float mtx[16]);
    static void drawUVSphere();
    static void drawUVSphere(const float mtx[16]);

    static bgfx::VertexLayout m_layout;

    static Vertex s_planeVertices[];
    static const uint16_t s_planeIndices[];
    static bgfx::VertexBufferHandle m_planeVbh;
    static bgfx::IndexBufferHandle m_planeIbh;

    static Vertex s_cubeVertices[];
    static const uint16_t s_cubeIndices[];
    static bgfx::VertexBufferHandle m_cubeVbh;
    static bgfx::IndexBufferHandle m_cubeIbh;

//    static Vertex s_uvSphereVertices[];
//    static const uint16_t s_uvSphereIndices[];
    static bgfx::VertexBufferHandle m_uvSphereVbh;
    static bgfx::IndexBufferHandle m_uvSphereIbh;

private:
    static void initUVSphere();

};

#endif // GEOMETRY_H
