// #pragma ounce

#ifndef VERTEX_H
#define VERTEX_H

#include <bgfx/bgfx.h>
//		BX_CHECK(NULL != _mem, "_mem can't be NULL");
//		BX_CHECK(isValid(_layout), "Invalid VertexLayout.");

//namespace { // question : why without build failed ?

struct Vertex {

    float x;
    float y;
    float z;
    float nx;
    float ny;
    float nz;
    float tx;
    float ty;

    //public:
    //    static void init()
    //    {

    //        m_layout.begin()
    //            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
    //            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
    //            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
    //            // .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
    //            .end();
    //    }

    //public:

    //public:
    //    static const bgfx::VertexLayout & layout() {
    //        return m_layout;
    //    }

    //private:
    //    static bgfx::VertexLayout m_layout;

    //public:
    //    static bgfx::VertexLayout layout();
};

//    bgfx::VertexLayout m_layout;
//} // close namespace

#endif // VERTEX_H
