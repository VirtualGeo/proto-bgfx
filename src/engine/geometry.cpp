#include "geometry.h"

//Geometry::Geometry()
//{

//}
//#include "vertex.h"
#include <bx/file.h>
#include <cassert>
//#include <vector>
//#include <math.h> // not work on win
#define _USE_MATH_DEFINES
#include <cmath>
bool g_isInit = false;

bgfx::VertexLayout Geometry::m_layout;
bgfx::VertexBufferHandle Geometry::m_planeVbh;
bgfx::IndexBufferHandle Geometry::m_planeIbh;
bgfx::VertexBufferHandle Geometry::m_cubeVbh;
bgfx::IndexBufferHandle Geometry::m_cubeIbh;
bgfx::VertexBufferHandle Geometry::m_uvSphereVbh;
bgfx::IndexBufferHandle Geometry::m_uvSphereIbh;

Vertex Geometry::s_planeVertices[] = {
    { -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
    { 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
    { 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
    { -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },
};
const uint16_t Geometry::s_planeIndices[] = {
    0, 1, 2,
    0, 2, 3,
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

    initUVSphere();

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

    bgfx::destroy(m_uvSphereVbh);
    bgfx::destroy(m_uvSphereIbh);
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

void Geometry::drawCube()
{
    assert(g_isInit);
    float mtx[16];
    bx::mtxIdentity(mtx);
    drawCube(mtx);
}


void Geometry::drawCube(const float mtx[])
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

void Geometry::drawUVSphere()
{
    assert(g_isInit);
    float mtx[16];
    bx::mtxIdentity(mtx);
    drawUVSphere(mtx);
}


void Geometry::drawUVSphere(const float mtx[])
{
    assert(g_isInit);
    bgfx::setTransform(mtx);

    bgfx::setVertexBuffer(0, m_uvSphereVbh);
    bgfx::setIndexBuffer(m_uvSphereIbh);
//    float mtx[16];
//    bx::mtxIdentity(mtx);
    const uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS
        | BGFX_STATE_CULL_CW | BGFX_STATE_BLEND_NORMAL | BGFX_STATE_MSAA;
    bgfx::setState(state);

}


void Geometry::initUVSphere()
{
    constexpr int m_nbRings = 100;
    constexpr int m_nbSlices = 100;
    constexpr float m_radius = 1.0f;
  // -----------------------------------------------------------------

  constexpr int nbRect = m_nbSlices * (m_nbRings - 1);
  constexpr int nbTriangle = nbRect * 2 + m_nbSlices * 2; // 20000 triangles
  constexpr int nbVertices = nbRect + m_nbSlices + 2;


  Vertex uvSphereVertices[nbVertices];
  uint16_t uvSphereIndices[nbTriangle * 3];

  int iNorthPole = nbVertices - 2;
  uvSphereVertices[iNorthPole] = {0, 0, m_radius, 0, 0, 1, 0, 0};

  int iSouthPole = nbVertices - 1;
  uvSphereVertices[iSouthPole] = {0, 0, -m_radius, 0, 0, -1, 0, 0};

  float deltaRing = static_cast<float>(M_PI) / (m_nbRings + 1.0f);
  float deltaSlice = 2.0f * static_cast<float>(M_PI) / m_nbSlices;

  int i = 0;

  for (int iRing = 0, iVertice = 0; iRing < m_nbRings; ++iRing) {
      float teta = (iRing + 1) * deltaRing;

      for (int iSlice = 0; iSlice < m_nbSlices; ++iSlice, ++iVertice) {
          float alpha = iSlice * deltaSlice;

          float y = sin(teta) * cos(alpha);
          float x = sin(alpha) * sin(teta);
          float z = cos(teta);

          float module2 = x * x + y * y + z * z;
          assert(abs(module2 - 1.0) < 1e-6);

          uvSphereVertices[iVertice] = {x * m_radius, y * m_radius, z * m_radius, x, y, z, 0.0f, 0.0f};

          int topLeft = iRing * m_nbSlices + iSlice;
          int topRight = (iSlice == m_nbSlices - 1) ? (iRing * m_nbSlices) : (topLeft + 1);

          if (iRing == 0) {
              uvSphereIndices[i++] = topRight;
              uvSphereIndices[i++] = topLeft;
              uvSphereIndices[i++] = nbVertices - 2;
          }

          if (iRing == m_nbRings - 1) {
              uvSphereIndices[i++] = topLeft;
              uvSphereIndices[i++] = topRight;
              uvSphereIndices[i++] = nbVertices - 1;
          }

          if (iRing != m_nbRings - 1) {
              int bottomLeft = topLeft + m_nbSlices;

              uvSphereIndices[i++] = topLeft;
              uvSphereIndices[i++] = topRight;
              uvSphereIndices[i++] = bottomLeft;

              uvSphereIndices[i++] = bottomLeft;
              uvSphereIndices[i++] = topRight;
              uvSphereIndices[i++] = topRight + m_nbSlices;
          }
      }
  }

  assert(i == nbTriangle * 3);

    m_uvSphereVbh = bgfx::createVertexBuffer(
        bgfx::copy(uvSphereVertices, sizeof(uvSphereVertices)), m_layout);
    m_uvSphereIbh = bgfx::createIndexBuffer(
        bgfx::copy(uvSphereIndices, sizeof(uvSphereIndices)));
}

