#include "Object.h"

#include <map>

//#include <bx/bx.h>
#include <bx/file.h>
#include <cassert>
#include <iostream>
#include <limits>
//#include "Vertex.h"
//#include "Mesh.h"
#include "FileSystem.h"
#include <algorithm>

static void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3])
{
    float v10[3];
    v10[0] = v1[0] - v0[0];
    v10[1] = v1[1] - v0[1];
    v10[2] = v1[2] - v0[2];

    float v20[3];
    v20[0] = v2[0] - v0[0];
    v20[1] = v2[1] - v0[1];
    v20[2] = v2[2] - v0[2];

    N[0] = v20[1] * v10[2] - v20[2] * v10[1];
    N[1] = v20[2] * v10[0] - v20[0] * v10[2];
    N[2] = v20[0] * v10[1] - v20[1] * v10[0];

    float len2 = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
    if (len2 > 0.0f) {
        float len = bx::sqrt(len2);

        N[0] /= len;
        N[1] /= len;
        N[2] /= len;
    }
}

namespace // Local utility functions
{
struct vec3 {
    float v[3];
    vec3()
    {
        v[0] = 0.0f;
        v[1] = 0.0f;
        v[2] = 0.0f;
    }
};

void normalizeVector(vec3& v)
{
    float len2 = v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2];
    if (len2 > 0.0f) {
        float len = bx::sqrt(len2);

        v.v[0] /= len;
        v.v[1] /= len;
        v.v[2] /= len;
    }
}

// Check if `mesh_t` contains smoothing group id.
bool hasSmoothingGroup(const tinyobj::shape_t& shape)
{
    for (size_t i = 0; i < shape.mesh.smoothing_group_ids.size(); i++) {
        if (shape.mesh.smoothing_group_ids[i] > 0) {
            return true;
        }
    }
    return false;
}

void computeSmoothingNormals(const tinyobj::attrib_t& attrib, const tinyobj::shape_t& shape,
    std::map<int, vec3>& smoothVertexNormals)
{
    smoothVertexNormals.clear();
    std::map<int, vec3>::iterator iter;

    for (size_t f = 0; f < shape.mesh.indices.size() / 3; f++) {
        // Get the three indexes of the face (all faces are triangular)
        tinyobj::index_t idx0 = shape.mesh.indices[3 * f + 0];
        tinyobj::index_t idx1 = shape.mesh.indices[3 * f + 1];
        tinyobj::index_t idx2 = shape.mesh.indices[3 * f + 2];

        // Get the three vertex indexes and coordinates
        int vi[3]; // indexes
        float v[3][3]; // coordinates

        for (int k = 0; k < 3; k++) {
            vi[0] = idx0.vertex_index;
            vi[1] = idx1.vertex_index;
            vi[2] = idx2.vertex_index;
            assert(vi[0] >= 0);
            assert(vi[1] >= 0);
            assert(vi[2] >= 0);

            v[0][k] = attrib.vertices[3 * vi[0] + k];
            v[1][k] = attrib.vertices[3 * vi[1] + k];
            v[2][k] = attrib.vertices[3 * vi[2] + k];
        }

        // Compute the normal of the face
        float normal[3];
        CalcNormal(normal, v[0], v[1], v[2]);

        // Add the normal to the three vertexes
        for (size_t i = 0; i < 3; ++i) {
            iter = smoothVertexNormals.find(vi[i]);
            if (iter != smoothVertexNormals.end()) {
                // add
                iter->second.v[0] += normal[0];
                iter->second.v[1] += normal[1];
                iter->second.v[2] += normal[2];
            } else {
                smoothVertexNormals[vi[i]].v[0] = normal[0];
                smoothVertexNormals[vi[i]].v[1] = normal[1];
                smoothVertexNormals[vi[i]].v[2] = normal[2];
            }
        }

    } // f

    // Normalize the normals, that is, make them unit vectors
    for (iter = smoothVertexNormals.begin(); iter != smoothVertexNormals.end();
         iter++) {
        normalizeVector(iter->second);
    }

} // computeSmoothingNormals
} // namespace

Object::Object(const tinyobj::shape_t& shape, const tinyobj::attrib_t& attrib,
    const std::vector<tinyobj::material_t>& materials, const size_t iShape,
    const bgfx::VertexLayout& layout)

//Object::Object(const tinyobj::shape_t& shape, const tinyobj::attrib_t & attrib)
{
    //    assert(bgfx::isValid(m_vbh));
    //    Vertex::init();

    m_name = shape.name;

    m_bmin[0] = m_bmin[1] = m_bmin[2] = std::numeric_limits<float>::max();
    m_bmax[0] = m_bmax[1] = m_bmax[2] = -std::numeric_limits<float>::max();

    //    std::vector<float> buffer; // pos(3float), normal(3float), color(3float)
    //    std::vector<Vertex> buffer;

    // Check for smoothing group and compute smoothing normals
    std::map<int, vec3> smoothVertexNormals;
    if (hasSmoothingGroup(shape) > 0) {
        //        std::cout << "[Object] Compute smoothingNormal for shape '" << shape.name << "'" << std::endl;
        computeSmoothingNormals(attrib, shape, smoothVertexNormals);
    }

    // OpenGL viewer does not support texturing with per-face material.
    //    assert(!shape.mesh.material_ids.empty());
    //    m_iMaterial = shape.mesh.material_ids[0];
    //    assert(0 <= m_iMaterial && m_iMaterial < static_cast<int>(materials.size()));

    //    std::cout << "m_iMaterial = " << m_iMaterial << " ";

    //    std::cout << "Object " << shape.name << ", ";
    const tinyobj::mesh_t& mesh = shape.mesh;
    assert(mesh.tags.empty());
    //    std::cout << "nbIndices:" << mesh.indices.size() << ", ";
    //    //    for (int i =0; i <mesh.indices.size(); ++i) {
    //    //    }
    //    std::cout << "nbMaterials:" << mesh.material_ids.size() << ", ";
    //    std::cout << "nbFaces:" << mesh.num_face_vertices.size() << ", ";
    //    std::cout << "nbGroups:" << mesh.smoothing_group_ids.size();
    //    std::cout << std::endl;

    //        std::map<int, int> idx2ind;
    //    const int nbAllVertices = attrib.vertices.size() / 3;
    //    int idx2ind[nbAllVertices];
    //    for (int i = 0; i < nbAllVertices; ++i) {
    //        idx2ind[i] = -1;
    //    }

    //    std::map<int, int> mat2ind;
    //    const int nbAllMaterials = materials.size();
    //    int mat2ind[nbAllMaterials];
    //    for (int i = 0; i < nbAllMaterials; ++i) {
    //        mat2ind[i] = -1;
    //    }

    //    for (size_t i =0; i <mesh.indices.size(); ++i) {
    //        tinyobj::index_t idx = shape.mesh.indices[i];
    //    std::map<int, std::map<int, int>> mat2idx2ind;

    //    }
    assert(attrib.texcoords.size() > 0);
    //    m_meshes.clear();
    m_meshes.reserve(4);

    assert(!mesh.material_ids.empty());
    int previous_material_id = mesh.material_ids[0];
    m_meshes.emplace_back(previous_material_id);
    int iMesh = 0;

    // for each triangles
    for (size_t f = 0; f < mesh.indices.size() / 3; f++) {
        const int current_material_id = mesh.material_ids[f];
        //        if (mat2idx2ind.find(current_material_id) == mat2idx2ind.end()) {
        //            mat
        //        }
        //        std::map<int, int>& idx2ind = mat2idx2ind[current_material_id];

        //        //        assert(idx0.texcoord_index >= 0 || idx1.texcoord_index >= 0 || idx2.texcoord_index >= 0);
        //        //        tinyobj::index_t idxs[3];
        //        for (int i = 0; i < 3; ++i) {
        //            const tinyobj::index_t idx = mesh.indices[3 * f + i];
        //            //            idxs[i] = mesh.indices[3 * f + i];

        //            //            if (idx2ind[idx.vertex_index] < 0) {

        //            if (idx2ind.find(idx.vertex_index) == idx2ind.end()) {
        //                //                if (mat2idx2ind[current_material_id].find(idx.vertex_index) == )
        //                const float x = attrib.vertices[3 * idx.vertex_index];
        //                const float y = attrib.vertices[3 * idx.vertex_index + 1];
        //                const float z = attrib.vertices[3 * idx.vertex_index + 2];
        //                m_bmin[0] = std::min(x, m_bmin[0]);
        //                m_bmin[1] = std::min(y, m_bmin[0]);
        //                m_bmin[2] = std::min(z, m_bmin[0]);
        //                m_bmax[0] = std::max(x, m_bmax[0]);
        //                m_bmax[1] = std::max(y, m_bmax[0]);
        //                m_bmax[2] = std::max(z, m_bmax[0]);

        //                assert(0 <= idx.normal_index && (3 * idx.normal_index + 2) < attrib.normals.size());
        //                const float nx = attrib.normals[3 * idx.normal_index];
        //                const float ny = attrib.normals[3 * idx.normal_index + 1];
        //                const float nz = attrib.normals[3 * idx.normal_index + 2];

        //                assert(0 <= idx.texcoord_index && size_t(2 * idx.texcoord_index + 1) < attrib.texcoords.size());
        //                const float tx = attrib.texcoords[2 * idx.texcoord_index];
        //                //                const float ty = attrib.texcoords[2 * idx.texcoord_index + 1];
        //                const float ty = 1.0f - attrib.texcoords[2 * idx.texcoord_index + 1];

        //                idx2ind[idx.vertex_index] = m_vertices.size();
        //                m_vertices.push_back({ x, y, z, nx, ny, nz, tx, ty });
        //                //            } else {
        //                //                assert(0 <= idx.texcoord_index && size_t(2 * idx.texcoord_index + 1) < attrib.texcoords.size());
        //                //                const float tx = attrib.texcoords[2 * idx.texcoord_index];
        //                //                const float ty = 1.0f - attrib.texcoords[2 * idx.texcoord_index + 1];

        //                //                Vertex& v = m_vertices[idx2ind[idx.vertex_index]];

        //                //                v.tx = (v.tx + tx) /2.0f;
        //                //                v.ty = (v.ty + ty) /2.0f;
        //                ////                float diff = std::abs(tx - v.tx) + std::abs(ty - v.ty);
        //                ////                if (diff > 0.1) {
        //                ////                    std::cout << "diff " << diff << std::endl;
        //                ////                }
        //                //                //                assert(std::abs(tx - v.tx) + std::abs(ty - v.ty) < 0.1f);
        //            }

        //            //            if (mat2ind[current_material_id] < 0) {
        //            if (mat2ind.find(current_material_id) == mat2ind.end()) {
        //                mat2ind[current_material_id] = m_meshes.size();
        //                m_meshes.push_back(current_material_id);
        //            }
        //            //                m_indices.push_back((uint16_t)idx2ind[idx.vertex_index]);
        //            m_meshes[mat2ind[current_material_id]].m_indices.push_back((uint16_t)idx2ind[idx.vertex_index]);
        //        }

        //        assert(idx0.normal_index == idx0.vertex_index);
        //        assert(idx0.vertex_index == idx0.texcoord_index);
        //        shape.mesh.num_face_vertices

        //        int current_material_id = shape.mesh.material_ids[f];

        //        std::cout << current_material_id << " ";
        //        if (current_material_id != m_iMaterial) {
        ////            std::cout << std::endl;
        //            std::cout << "multiple texture for " << m_name << std::endl;
        //        }
        //        assert(current_material_id == m_iMaterial);

        //            assert(0 <= current_material_id && current_material_id < static_cast<int>(materials.size()));
        //        if ((current_material_id < 0) || (current_material_id >= static_cast<int>(materials.size()))) {
        //            // Invaid material ID. Use default material.
        //            current_material_id = materials.size() - 1; // Default material is added to the last item in `materials`.
        //        }
        // if (current_material_id >= materials.size()) {
        //    std::cerr << "Invalid material index: " << current_material_id <<
        //    std::endl;
        //}
        //

        tinyobj::index_t idx0 = shape.mesh.indices[3 * f + 0];
        tinyobj::index_t idx1 = shape.mesh.indices[3 * f + 1];
        tinyobj::index_t idx2 = shape.mesh.indices[3 * f + 2];
        float tc[3][2];
        if (attrib.texcoords.size() > 0) {
            if ((idx0.texcoord_index < 0) || (idx1.texcoord_index < 0) || (idx2.texcoord_index < 0)) {
                // face does not contain valid uv index.
                tc[0][0] = 0.0f;
                tc[0][1] = 0.0f;
                tc[1][0] = 0.0f;
                tc[1][1] = 0.0f;
                tc[2][0] = 0.0f;
                tc[2][1] = 0.0f;
            } else {
                assert(attrib.texcoords.size() > size_t(2 * idx0.texcoord_index + 1));
                assert(attrib.texcoords.size() > size_t(2 * idx1.texcoord_index + 1));
                assert(attrib.texcoords.size() > size_t(2 * idx2.texcoord_index + 1));

                // Flip Y coord.
                tc[0][0] = attrib.texcoords[2 * idx0.texcoord_index];
                tc[0][1] = 1.0f - attrib.texcoords[2 * idx0.texcoord_index + 1];
                tc[1][0] = attrib.texcoords[2 * idx1.texcoord_index];
                tc[1][1] = 1.0f - attrib.texcoords[2 * idx1.texcoord_index + 1];
                tc[2][0] = attrib.texcoords[2 * idx2.texcoord_index];
                tc[2][1] = 1.0f - attrib.texcoords[2 * idx2.texcoord_index + 1];
            }
        } else {
            tc[0][0] = 0.0f;
            tc[0][1] = 0.0f;
            tc[1][0] = 0.0f;
            tc[1][1] = 0.0f;
            tc[2][0] = 0.0f;
            tc[2][1] = 0.0f;
        }

        float v[3][3];
        for (int k = 0; k < 3; k++) {
            int f0 = idx0.vertex_index;
            int f1 = idx1.vertex_index;
            int f2 = idx2.vertex_index;
            assert(f0 >= 0);
            assert(f1 >= 0);
            assert(f2 >= 0);

            v[0][k] = attrib.vertices[3 * f0 + k];
            v[1][k] = attrib.vertices[3 * f1 + k];
            v[2][k] = attrib.vertices[3 * f2 + k];
            m_bmin[k] = std::min(v[0][k], m_bmin[k]);
            m_bmin[k] = std::min(v[1][k], m_bmin[k]);
            m_bmin[k] = std::min(v[2][k], m_bmin[k]);
            m_bmax[k] = std::max(v[0][k], m_bmax[k]);
            m_bmax[k] = std::max(v[1][k], m_bmax[k]);
            m_bmax[k] = std::max(v[2][k], m_bmax[k]);
        }

        float n[3][3];
        {
            bool invalid_normal_index = false;
            if (attrib.normals.size() > 0) {
                int nf0 = idx0.normal_index;
                int nf1 = idx1.normal_index;
                int nf2 = idx2.normal_index;

                if ((nf0 < 0) || (nf1 < 0) || (nf2 < 0)) {
                    // normal index is missing from this face.
                    invalid_normal_index = true;
                } else {
                    for (int k = 0; k < 3; k++) {
                        assert(size_t(3 * nf0 + k) < attrib.normals.size());
                        assert(size_t(3 * nf1 + k) < attrib.normals.size());
                        assert(size_t(3 * nf2 + k) < attrib.normals.size());
                        n[0][k] = attrib.normals[3 * nf0 + k];
                        n[1][k] = attrib.normals[3 * nf1 + k];
                        n[2][k] = attrib.normals[3 * nf2 + k];
                    }
                }
            } else {
                invalid_normal_index = true;
            }

            if (invalid_normal_index && !smoothVertexNormals.empty()) {
                // Use smoothing normals
                int f0 = idx0.vertex_index;
                int f1 = idx1.vertex_index;
                int f2 = idx2.vertex_index;

                if (f0 >= 0 && f1 >= 0 && f2 >= 0) {
                    n[0][0] = smoothVertexNormals[f0].v[0];
                    n[0][1] = smoothVertexNormals[f0].v[1];
                    n[0][2] = smoothVertexNormals[f0].v[2];

                    n[1][0] = smoothVertexNormals[f1].v[0];
                    n[1][1] = smoothVertexNormals[f1].v[1];
                    n[1][2] = smoothVertexNormals[f1].v[2];

                    n[2][0] = smoothVertexNormals[f2].v[0];
                    n[2][1] = smoothVertexNormals[f2].v[1];
                    n[2][2] = smoothVertexNormals[f2].v[2];

                    invalid_normal_index = false;
                }
            }

            if (invalid_normal_index) {
                // compute geometric normal
                CalcNormal(n[0], v[0], v[1], v[2]);
                n[1][0] = n[0][0];
                n[1][1] = n[0][1];
                n[1][2] = n[0][2];
                n[2][0] = n[0][0];
                n[2][1] = n[0][1];
                n[2][2] = n[0][2];
            }
        }

        //                if (mat2ind.find(current_material_id) == mat2ind.end()) {
        //                    mat2ind[current_material_id] = m_meshes.size();
        //                    m_meshes.emplace_back(current_material_id);

        //                }
        if (current_material_id != previous_material_id) {
            ++iMesh;
            m_meshes.emplace_back(current_material_id);
        }
        previous_material_id = current_material_id;
        for (int k = 0; k < 3; k++) {
            //            buffer.push_back(v[k][0]);
            //            buffer.push_back(v[k][1]);
            //            buffer.push_back(v[k][2]);
            //            buffer.push_back(n[k][0]);
            //            buffer.push_back(n[k][1]);
            //            buffer.push_back(n[k][2]);

            //            // Combine normal and diffuse to get color.
            //            float normal_factor = 0.2;
            //            float diffuse_factor = 1 - normal_factor;
            //            float c[3] = { n[k][0] * normal_factor + diffuse[0] * diffuse_factor,
            //                n[k][1] * normal_factor + diffuse[1] * diffuse_factor,
            //                n[k][2] * normal_factor + diffuse[2] * diffuse_factor };
            //            float len2 = c[0] * c[0] + c[1] * c[1] + c[2] * c[2];
            //            if (len2 > 0.0f) {
            //                float len = bx::sqrt(len2);

            //                c[0] /= len;
            //                c[1] /= len;
            //                c[2] /= len;
            //            }
            //            buffer.push_back(c[0] * 0.5 + 0.5);
            //            buffer.push_back(c[1] * 0.5 + 0.5);
            //            buffer.push_back(c[2] * 0.5 + 0.5);

            //            buffer.push_back(tc[k][0]);
            //            buffer.push_back(tc[k][1]);

            //            m_vertices.push_back({v[k][0], v[k][1], v[k][2], n[k][0], n[k][1], n[k][2], tc[k][0], tc[k][1]});
            //                    m_meshes[mat2ind[current_material_id]].m_indices.push_back(m_vertices.size());
            m_meshes[iMesh].m_indices.push_back(m_vertices.size());
            //                    m_meshes[iMesh].m_
            m_vertices.push_back({ v[k][0], v[k][1], v[k][2], n[k][0], n[k][1], n[k][2], tc[k][0], tc[k][1] });
            //            m_vertices.emplace_back(v[k][0], v[k][1], v[k][2], n[k][0], n[k][1], n[k][2], tc[k][0], tc[k][1]);
        }

        //                        m_indices.push_back((uint16_t)idx2ind[idx.vertex_index]);
    }

    //    std::cout << std::endl;
    //    o.vb_id = 0;
    //    o.numTriangles = 0;

    //    if (shape.mesh.material_ids.size() > 0 && shape.mesh.material_ids.size() > iShape) {
    //        m_iMaterial = shape.mesh.material_ids[0];
    //        //        o.material_id = shape.mesh.material_ids[0]; // use the material ID
    //        // of the first face.
    ////        m_iMaterial = materials.size() - 1;
    //    } else {
    //        m_iMaterial = materials.size() - 1;
    //        //        o.material_id = materials.size() - 1; // = ID for default material.
    //    }
    //        printf("[Object] shape[%d] material_id %d\n", int(iShape), int(m_iMaterial));

    if (m_vertices.size() > 0) {
        //        glGenBuffers(1, &o.vb_id);
        //        glBindBuffer(GL_ARRAY_BUFFER, o.vb_id);
        //        glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float),
        //            &buffer.at(0), GL_STATIC_DRAW);
        //        o.numTriangles = buffer.size() / (3 + 3 + 3 + 2) / 3; // 3:vtx, 3:normal, 3:col, 2:texcoord
        //        m_nbTriangles = buffer.size() / (3 + 3 + 3 + 2) / 3; // 3:vtx, 3:normal, 3:col, 2:texcoord
        //        m_nbTriangles = buffer.size() / (3 + 3 + 2) / 3; // 3:vtx, 3:normal, 2:texcoord
        //        m_nbTriangles = m_vertices.size() / 3;
        m_nbTriangles = 0;
        for (int i = 0; i < m_meshes.size(); ++i) {
            Mesh& mmesh = m_meshes[i];
            assert(mmesh.m_indices.size() % 3 == 0);

            //            mmesh.m_ibh = bgfx::createIndexBuffer(bgfx::makeRef(mmesh.m_indices.data(), sizeof(indice_type) * mmesh.m_indices.size()), BGFX_BUFFER_INDEX32);
            mmesh.m_ibh = bgfx::createIndexBuffer(bgfx::makeRef(mmesh.m_indices.data(), sizeof(indice_type) * mmesh.m_indices.size()));
            m_nbTriangles += mmesh.m_indices.size() / 3;
#ifdef DEBUG
            std::cout << "    [Mesh] " << i << " nbIndices=" << mmesh.m_indices.size() << std::endl;
#endif
        }

        //    std::vector<float> buffer; // pos(3float), normal(3float), color(3float)
        //        m_vbh = bgfx::createVertexBuffer(bgfx::copy(buffer.data(), sizeof(float) * buffer.size()), );

        //        bgfx::VertexLayout m_layout;
        //        m_layout.begin()
        //            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        //            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        //            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        //            .end();

        m_vbh = bgfx::createVertexBuffer(
            bgfx::makeRef(m_vertices.data(), sizeof(Vertex) * m_vertices.size()),
            layout);

        //        printf("[Object] shape[%d] # of triangles = %d\n", static_cast<int>(iShape),
        //            m_nbTriangles);
        //        std::cout << "[Object] " << iShape << ", shape '" << m_name << "', nbMeshes=" << m_meshes.size() << ", nbTriangles=" << m_nbTriangles << std::endl;
    }

    //    BX_CHECK(bgfx::isValid(m_vbh), "vbh not valid on object constructor"); // not worked !!!
    //    bgfx::crev
}

Object::Object(Object&& object)
    : m_vbh(std::move(object.m_vbh))
    , m_nbTriangles(object.m_nbTriangles)
    //    , m_iMaterial(object.m_iMaterial)
    , m_meshes(std::move(object.m_meshes))
    ,
    //      m_bmin(object.m_bmin),
    //      m_bmax(object.m_bmax),
    m_name(std::move(object.m_name))
{
    for (int i = 0; i < 3; ++i) {
        m_bmin[i] = object.m_bmin[i];
        m_bmax[i] = object.m_bmax[i];
    }
    object.m_name = "";

#ifdef DEBUG
    std::cout << "\033[34m";
    std::cout << "[Object] " << this << " '" << m_name << "' right moving from " << &object << std::endl;
    std::cout << "\033[0m";
#endif
}

Object::~Object()
{
    //    assert(m_name.empty());
    if (!m_name.empty()) {
        //        assert(bgfx::isValid(m_vbh));
        bgfx::destroy(m_vbh);
    }

#ifdef DEBUG
    std::cout << "\033[31m";
    std::cout << "[Object] '" << m_name << "' deleted " << this << std::endl;
    std::cout << "\033[0m";
#endif
}

Object::Object(std::ifstream& file, const size_t iShape, const bgfx::VertexLayout& layout)
{
    FileSystem::read(m_vertices, file);
    FileSystem::read(m_nbTriangles, file);
    FileSystem::read(m_bmin, 3, file);
    FileSystem::read(m_bmax, 3, file);
    FileSystem::read(m_name, file);

    size_t size;
    FileSystem::read(size, file);
    m_meshes.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        //        m_meshes.push_back(file);
        m_meshes.emplace_back(file);
        //        const Mesh & mesh = m_meshes.back();
        //        std::cout << "    [Object] Load mesh[" << i << "/" << size << "] nbIndices=" << mesh.m_indices.size() << ", iMaterial=" << mesh.m_iMaterial  << std::endl;
    }
    m_vbh = bgfx::createVertexBuffer(
        bgfx::makeRef(m_vertices.data(), sizeof(Vertex) * m_vertices.size()),
        layout);

    //        printf("[Object] shape[%d] # of triangles = %d\n", static_cast<int>(iShape),
    //            m_nbTriangles);
    //    std::cout << "[Object] " << iShape << ", shape '" << m_name << "', nbMeshes=" << m_meshes.size() << ", nbTriangles=" << m_nbTriangles << std::endl;
}

void Object::save(std::ofstream& file) const
{
    FileSystem::write(m_vertices, file);
    FileSystem::write(m_nbTriangles, file);
    FileSystem::write(m_bmin, 3, file);
    FileSystem::write(m_bmax, 3, file);
    FileSystem::write(m_name, file);

    size_t size = m_meshes.size();
    FileSystem::write(size, file);
    //    m_meshes.reserve(size);
    for (int i = 0; i < size; ++i) {
        //        m_meshes.push_back(file);
        m_meshes[i].save(file);
    }
}

void Object::draw(const bgfx::ViewId id, const Program& program, const float* mtx, const uint64_t state, const Materials& materials, const Textures& textures) const
{
    for (const Mesh& mesh : m_meshes) {
        bgfx::setVertexBuffer(0, m_vbh);

        mesh.draw(id, program, mtx, state, materials, textures, m_vbh);

        //        bgfx::submit(id, program.m_handle);
    }
}


// --------------------------------- GETTERS
const bgfx::VertexBufferHandle& Object::vbh() const
{
    return m_vbh;
}

//size_t Object::iMaterial() const
//{
//    return m_iMaterial;
//}

size_t Object::nbVertices() const
{
    return m_vertices.size();
}

size_t Object::nbTriangles() const
{
    return m_nbTriangles;
}

size_t Object::nbMeshes() const
{
    return m_meshes.size();
}

std::string Object::name() const
{
    return m_name;
}

std::ostream& operator<<(std::ostream& os, const Object& object)
{
    os << "'" << object.m_name << "', nbMeshes=" << object.m_meshes.size() << ", nbTriangles=" << object.m_nbTriangles << std::endl;
    //    for (const Mesh& mesh : object.m_meshes) {
    int size = object.m_meshes.size();
    for (int i = 0; i < size; ++i) {
        const Mesh& mesh = object.m_meshes[i];
        os << "    [Object] Load mesh[" << i << "/" << size << "] : " << mesh;
//        os << mesh;
        if (i != size - 1) {
            os << std::endl;
        }
    }
    return os;
}

//bgfx::IndexBufferHandle Object::ibh() const
//{
//    return m_ibh;
//}

//std::vector<uint16_t> Mesh::indices() const
//{
//    return m_indices;
//}
