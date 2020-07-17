#include "mesh.h"

#include "fileIO.h"
#include <cassert>

Mesh::Mesh(int iMaterial)
    : m_iMaterial(iMaterial)
{
}

Mesh::Mesh(Mesh&& mesh)
    : m_iMaterial(mesh.m_iMaterial)
    , m_indices(std::move(m_indices))
    , m_ibh(std::move(m_ibh))
{
    //    m_last = true;
    mesh.m_last = false;

#ifdef DEBUG
    std::cout << "\033[34m";
    std::cout << "[Mesh] " << this << " right moving from " << &mesh << std::endl;
    std::cout << "\033[0m";
#endif
}

Mesh::~Mesh()
{
    if (m_last) {
        bgfx::destroy(m_ibh);
    }

    //#ifdef DEBUG
    //    std::cout << "\033[31m";
    //    std::cout << "[Mesh] " << this << " deleted" << std::endl;
    //    std::cout << "\033[0m";
    //#endif
}

Mesh::Mesh(std::ifstream& file)
{
    FileIO::read(m_iMaterial, file);
    FileIO::read(m_indices, file);

    //    m_ibh = bgfx::createIndexBuffer(bgfx::makeRef(m_indices.data(), sizeof(uint16_t) * m_indices.size()));
    m_ibh = bgfx::createIndexBuffer(bgfx::makeRef(m_indices.data(), sizeof(indice_type) * m_indices.size()));
    //    m_ibh = bgfx::createIndexBuffer(bgfx::makeRef(m_indices.data(), sizeof(indice_type) * m_indices.size()), BGFX_BUFFER_INDEX32);
}

void Mesh::save(std::ofstream& file) const
{
    FileIO::write(m_iMaterial, file);
    FileIO::write(m_indices, file);
}

//Mesh &Mesh::operator=(Mesh &&mesh)
//{

//}

void Mesh::draw(const bgfx::ViewId id, const Shading &shading, const float* mtx, const uint64_t state, const Materials& materials) const
{
    bgfx::setTransform(mtx);
    bgfx::setState(state);

    assert(0 <= m_iMaterial && m_iMaterial < materials.size());
    const Material& material = materials[m_iMaterial];

    assert(bgfx::isValid(m_ibh));
    bgfx::setIndexBuffer(m_ibh);

    Program::submit(id, shading, material);
    //    bgfx::setVertexBuffer(0, vbh);
//    bgfx::submit(id, Program::handleProgram(shading));
}

std::ostream& operator<<(std::ostream& os, const Mesh& mesh)
{
    //        const Mesh & mesh = m_meshes.back();
    //        std::cout << "    [Object] Load mesh[" << i << "/" << size << "] nbIndices=" << mesh.m_indices.size() << ", iMaterial=" << mesh.m_iMaterial  << std::endl;
    return os << "nbIndices=" << mesh.m_indices.size() << ", iMaterial=" << mesh.m_iMaterial;
}
