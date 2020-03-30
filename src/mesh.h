
//#pragma ounce
#ifndef MESH_H
#define MESH_H

#include <bgfx/bgfx.h>

#include <vector>

#include "material.h"
#include "program.h"
#include "dirLight.h"

// using indice_type = uint16_t;
using indice_type = unsigned short;

class Mesh {
public:
    Mesh(int iMaterial);
    Mesh(const Mesh& mesh) = delete;
    //    Mesh(Mesh && mesh) noexcept = default;
    Mesh(Mesh&& mesh);
    //    Mesh & operator =(Mesh && mesh) = delete ;
    //    Mesh & operator=(Mesh & mesh) = delete ;
    //    Mesh & operator=(Mesh && mesh);
    ~Mesh();
    Mesh(std::ifstream& file);
    void save(std::ofstream& file) const;

    void draw(const bgfx::ViewId id, const Program& program, const float* mtx,
        const uint64_t state, const Materials& materials,
        const Textures& textures, const DirLight & dirLight) const;

    //    std::vector<uint16_t> indices() const;
    friend std::ostream& operator<<(std::ostream& os, const Mesh& mesh);

    // private:
    size_t m_iMaterial;
    std::vector<indice_type> m_indices;
    bgfx::IndexBufferHandle m_ibh;

private:
    bool m_last = true;
};

#endif // MESH_H
