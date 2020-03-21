
//#pragma ounce
#ifndef MESH_H
#define MESH_H

#include <vector>
#include <bgfx/bgfx.h>
#include "Material.h"
#include "Program.h"

class Mesh {
public:
    Mesh(int iMaterial);

    void draw(const bgfx::ViewId id, const Program & program, const float *mtx,
                const uint64_t state, const Materials & materials, const Textures & textures, const bgfx::VertexBufferHandle & vbh) const;

//    std::vector<uint16_t> indices() const;

//private:
    size_t m_iMaterial;
    std::vector<uint16_t> m_indices;
    bgfx::IndexBufferHandle m_ibh;
};


#endif // MESH_H
