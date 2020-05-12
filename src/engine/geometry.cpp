#include "geometry.h"

//Geometry::Geometry()
//{

//}
//#include "vertex.h"
#include <bx/file.h>
#include <cassert>
bool g_isInit = false;

bgfx::VertexLayout Geometry::m_layout;
bgfx::VertexBufferHandle Geometry::m_planeVbh;
bgfx::IndexBufferHandle Geometry::m_planeIbh;
bgfx::VertexBufferHandle Geometry::m_cubeVbh;
bgfx::IndexBufferHandle Geometry::m_cubeIbh;

Vertex Geometry::s_planeVertices[] = {
    //        { -1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f},
    //        {  1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f},
    //        { -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f},
    //        {  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,  1.0f, 1.0f},
    //        {-1.0f, 1.0f, 0.0f,  -1.0f, 1.0f, 1.0f,  0.0f, 1.0f},
    //        { 1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f},
    //        { 1.0f,-1.0f, 0.0f,  1.0f, -1.0f, 1.0f,  1.0f, 0.0f},
    //        {-1.0f,-1.0f, 0.0f,  -1.0f, -1.0f, 1.0f,  0.0f, 0.0f},
    { -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
    { 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
    { 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
    { -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },
};
const uint16_t Geometry::s_planeIndices[] = {
    0, 1, 2,
    0, 2, 3,
    //        1, 2, 3
    //        1, 3, 2,
};

Vertex Geometry::s_cubeVertices[] {
    {-1.0f,  1.0f,  1.0f,  -1.0, 1.0f, 1.0f,  0.0f, 0.0f },
    { 1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f },
    {-1.0f, -1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  0.0f, 0.0f },
    { 1.0f, -1.0f,  1.0f,  1.0f, -1.0f, 1.0f,  0.0f, 0.0f },
    {-1.0f,  1.0f, -1.0f,  -1.0f, 1.0f, -1.0f,  0.0f, 0.0f },
    { 1.0f,  1.0f, -1.0f,  1.0f, 1.0f, -1.0f,  0.0f, 0.0f },
    {-1.0f, -1.0f, -1.0f,  -1.0f, -1.0f, -1.0f,  0.0f, 0.0f },
    { 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,  0.0f, 0.0f }
};
const uint16_t Geometry::s_cubeIndices[] {
    0, 1, 2, // 0
    1, 3, 2,
    4, 6, 5, // 2
    5, 6, 7,
    0, 2, 4, // 4
    4, 2, 6,
    1, 5, 3, // 6
    5, 7, 3,
    0, 4, 1, // 8
    4, 5, 1,
    2, 3, 6, // 10
    6, 3, 7,
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

    m_planeVbh = bgfx::createVertexBuffer(
        bgfx::makeRef(s_planeVertices, sizeof(s_planeVertices)), m_layout);
    m_planeIbh = bgfx::createIndexBuffer(
        bgfx::makeRef(s_planeIndices, sizeof(s_planeIndices)));


    m_cubeVbh = bgfx::createVertexBuffer(
        bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), m_layout);
    m_cubeIbh = bgfx::createIndexBuffer(
        bgfx::makeRef(s_cubeIndices, sizeof(s_cubeIndices)));


    //    m_planeVbh = bgfx::createVertexBuffer(
    //        bgfx::makeRef(m_vertices.data(), sizeof(Vertex) * m_vertices.size()),
    //        Program::m_layout);

    g_isInit = true;
}

void Geometry::clear()
{
    //    bgfx::destroy(m_layout);

    bgfx::destroy(m_planeVbh);
    bgfx::destroy(m_planeIbh);

    bgfx::destroy(m_cubeVbh);
    bgfx::destroy(m_cubeIbh);
}

void Geometry::drawQuad()
{
    assert(g_isInit);
    bgfx::setIndexBuffer(m_planeIbh);
    bgfx::setVertexBuffer(0, m_planeVbh);
    float mtx[16];
    bx::mtxIdentity(mtx);
    const uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS
        | BGFX_STATE_CULL_CCW | BGFX_STATE_BLEND_NORMAL | BGFX_STATE_MSAA;
    bgfx::setTransform(mtx);
    bgfx::setState(state);
}

void Geometry::drawCube(float mtx[])
{
    assert(g_isInit);
    bgfx::setTransform(mtx);

    bgfx::setVertexBuffer(0, m_cubeVbh);
    bgfx::setIndexBuffer(m_cubeIbh);
//    float mtx[16];
//    bx::mtxIdentity(mtx);
    const uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS
        | BGFX_STATE_CULL_CW | BGFX_STATE_BLEND_NORMAL | BGFX_STATE_MSAA;
    bgfx::setState(state);

}

