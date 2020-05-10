#include "geometry.h"

//Geometry::Geometry()
//{

//}
//#include "vertex.h"
#include <cassert>
#include <bx/file.h>
bool g_isInit = false;

bgfx::VertexLayout Geometry::m_layout;
bgfx::VertexBufferHandle Geometry::m_vbh;
bgfx::IndexBufferHandle Geometry::m_ibh;
    Vertex Geometry::s_hplaneVertices[] =
    {
//        { -1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f},
//        {  1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f},
//        { -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f},
//        {  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,  1.0f, 1.0f},
//        {-1.0f, 1.0f, 0.0f,  -1.0f, 1.0f, 1.0f,  0.0f, 1.0f},
//        { 1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f},
//        { 1.0f,-1.0f, 0.0f,  1.0f, -1.0f, 1.0f,  1.0f, 0.0f},
//        {-1.0f,-1.0f, 0.0f,  -1.0f, -1.0f, 1.0f,  0.0f, 0.0f},
        {-1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f},
        { 1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f},
        { 1.0f,-1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f},
        {-1.0f,-1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f},
    };

    const uint16_t Geometry::s_planeIndices[] =
    {
        0, 1, 2,
        0, 2, 3,
//        1, 2, 3
//        1, 3, 2,
    };


void Geometry::init()
{
    m_layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        // .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .end();

//    bgfx::frame();


    m_vbh = bgfx::createVertexBuffer(
          bgfx::makeRef(s_hplaneVertices, sizeof(s_hplaneVertices) )
        , m_layout
        );

    m_ibh = bgfx::createIndexBuffer(
          bgfx::makeRef(s_planeIndices, sizeof(s_planeIndices) )
        );

//    m_vbh = bgfx::createVertexBuffer(
//        bgfx::makeRef(m_vertices.data(), sizeof(Vertex) * m_vertices.size()),
//        Program::m_layout);

    g_isInit = true;

}

void Geometry::clear()
{
//    bgfx::destroy(m_layout);

    bgfx::destroy(m_vbh);
    bgfx::destroy(m_ibh);

}

void Geometry::drawQuad()
{
    assert(g_isInit);
    bgfx::setIndexBuffer(m_ibh);
    bgfx::setVertexBuffer(0, m_vbh);
    float mtx[16];
    bx::mtxIdentity(mtx);
    const uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS
        | BGFX_STATE_CULL_CCW | BGFX_STATE_BLEND_NORMAL | BGFX_STATE_MSAA;
    bgfx::setTransform(mtx);
    bgfx::setState(state);
}
