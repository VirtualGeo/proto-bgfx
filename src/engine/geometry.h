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

    static bgfx::VertexLayout m_layout;

    static Vertex s_hplaneVertices[];
    static const uint16_t s_planeIndices[];


    static bgfx::VertexBufferHandle m_vbh;
    static bgfx::IndexBufferHandle m_ibh;

};

#endif // GEOMETRY_H
