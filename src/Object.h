#ifndef OBJECT_H
#define OBJECT_H

//#include <vector>
#include <bgfx/bgfx.h>
#include <tiny_obj_loader.h>

class Object
{
public:
    Object(const tinyobj::shape_t & shape);

private:
    bgfx::VertexBufferHandle m_vbh;
    uint m_nbTriangles;
    size_t m_iMaterial;

    float m_bmin[3];
    float m_bmax[3];
};

//using Objects = std::vector<Object>;


#endif // OBJECT_H
