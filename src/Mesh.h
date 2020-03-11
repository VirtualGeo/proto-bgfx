
#pragma ounce
#include "bgfx/bgfx.h"
#include <vector>
// #include <tinyobj/>
#include "tiny_obj_loader.h"
#include <cassert>
#include <iostream>

// struct Vertex {
// 	glm::vec3 position;
// 	glm::vec3 normal;
// 	// glm::vec2 uvTexture;
// };

// struct PosColorVertex {
// 	float x;
// 	float y;
// 	float z;
// 	// uint32_t abgr;
// 	float nx;
// 	float ny;
// 	float nz;
// };

// static PosColorVertex cubeVertices[] = {
// 	{-1.0f, 1.0f, 1.0f, 0xff000000},   {1.0f, 1.0f, 1.0f, 0xff0000ff},
// 	{-1.0f, -1.0f, 1.0f, 0xff00ff00},  {1.0f, -1.0f, 1.0f, 0xff00ffff},
// 	{-1.0f, 1.0f, -1.0f, 0xffff0000},  {1.0f, 1.0f, -1.0f, 0xffff00ff},
// 	{-1.0f, -1.0f, -1.0f, 0xffffff00}, {1.0f, -1.0f, -1.0f, 0xffffffff},
// };

// static PosColorVertex cubeVertices[] = {
// 	{-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
// 	{1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
// 	{-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f},
// 	{1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f},
// 	{-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f},
// 	{1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f},
// 	{-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f},
// 	{1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f},
// };

// static const uint16_t cubeTriList[] = {
// 	0, 1, 2, 1, 3, 2, 4, 6, 5, 5, 6, 7, 0, 2, 4, 4, 2, 6,
// 	1, 5, 3, 5, 7, 3, 0, 4, 1, 4, 5, 1, 2, 3, 6, 6, 3, 7,
// };
struct Vertex {
	float x;
	float y;
	float z;
	float nx;
	float ny;
	float nz;
	// float tx;
	// float ty;
};

struct Mesh {

	Mesh(const char *filePath) {
		// ------------- INIT GEOMETRY
		// bgfx::VertexDecl pcvDecl; // question : out of date ?
		// bgfx::VertexLayout ms_layout;
		m_layout.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
			// .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			// .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();

		// bgfx::VertexBufferHandle vbh;
		// std::string inputFile = "D:/proto-bgfx/Assets/Teapot/teapot.obj";
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string warn;
		std::string err;

		const bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn,
										  &err, filePath);

		if (!warn.empty()) {
			std::cout << warn << std::endl;
		}

		if (!err.empty()) {
			std::cerr << err << std::endl;
		}

		if (!ret) {
			throw std::runtime_error("");
			// exit(1);
		}

		if (attrib.vertices.empty() || attrib.normals.empty() ||
			attrib.texcoords.empty()) {
			throw std::runtime_error("bad vertices");
		}
		// std::vector<float[8]> vertices;
		// std::vector<Vertex> vertices;
		// vertices.clear();
		// std::vector<uint16_t> triangle_list;

		// std::vector<Vertex> vertices;
		// std::vector<uint16_t> triangle_list;
        m_vertices.clear();
        m_indices.clear();
        m_vertices.reserve(1000);
        m_indices.reserve(1000);

		// attrib.vertices;
		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces(polygon)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size();
				 f++) {
				int fv = shapes[s].mesh.num_face_vertices[f];

				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx =
						shapes[s].mesh.indices[index_offset + v];
					tinyobj::real_t vx =
						attrib.vertices[3 * idx.vertex_index + 0];
					tinyobj::real_t vy =
						attrib.vertices[3 * idx.vertex_index + 1];
					tinyobj::real_t vz =
						attrib.vertices[3 * idx.vertex_index + 2];
					tinyobj::real_t nx =
						attrib.normals[3 * idx.normal_index + 0];
					tinyobj::real_t ny =
						attrib.normals[3 * idx.normal_index + 1];
					tinyobj::real_t nz =
						attrib.normals[3 * idx.normal_index + 2];
					tinyobj::real_t tx =
						attrib.texcoords[2 * idx.texcoord_index + 0];
					tinyobj::real_t ty =
						attrib.texcoords[2 * idx.texcoord_index + 1];

					// vertices.push_back({{vx, vy, vz}, {nx, ny, nz}}); //
					// question : emplace_back ?
					// vertices.push_back({vx, vy, vz, nx, ny, nz, tx,
										// ty}); // question : emplace_back ?
					m_vertices.push_back(
						{vx, vy, vz, nx, ny, nz}); // question : emplace_back ?
					// Optional: vertex colors
					// tinyobj::real_t red =
					// attrib.colors[3*idx.vertex_index+0]; tinyobj::real_t
					// green = attrib.colors[3*idx.vertex_index+1];
					// tinyobj::real_t blue =
					// attrib.colors[3*idx.vertex_index+2];
					m_indices.push_back(uint16_t(m_indices.size()));
				}
				index_offset += fv;

				// per-face material
				shapes[s].mesh.material_ids[f];
			}
		}

		// vbh = bgfx::createVertexBuffer(
		// 	bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), ms_layout);
		// ibh = bgfx::createIndexBuffer(
		// 	bgfx::makeRef(cubeTriList, sizeof(cubeTriList)));
		const uint16_t stride = m_layout.getStride();

		// const bgfx::Memory * mem = bgfx::alloc(vertices.size * stride);
		// bx::memCopy(vertices, )
		// vbh = bgfx::createVertexBuffer(mem, ms_layout);
		assert(stride == sizeof(Vertex));

		// m_numVertices = vertices.size();
		// m_vertices = (uint8_t *)malloc(sizeof(Vertex) * m_numVertices);
		// *m_vertices = (uint8_t)vertices.data();
		// m_vbh = bgfx::createVertexBuffer(
		// 	bgfx::makeRef(m_vertices, sizeof(Vertex) * m_numVertices),
		// 	m_layout);

		// m_numIndices = triangle_list.size();
		// // m_indices = std::move(triangle_list.data());
		// m_indices = (uint16_t *)malloc(sizeof(uint16_t) * m_numIndices);
		// *m_indices = (uint16_t)triangle_list.data();
		// m_ibh = bgfx::createIndexBuffer(
		// 	bgfx::makeRef(m_indices, sizeof(uint16_t) * m_numIndices));

		m_vbh = bgfx::createVertexBuffer(
			bgfx::makeRef(m_vertices.data(),
						  sizeof(Vertex) * m_vertices.size()),
			m_layout);
		m_ibh = bgfx::createIndexBuffer(bgfx::makeRef(
			m_indices.data(), sizeof(uint16_t) * m_indices.size()));
	}

	void submit(bgfx::ViewId id, bgfx::ProgramHandle program, const float *mtx,
				uint64_t state) {
		bgfx::setTransform(mtx);
		bgfx::setState(state);

		bgfx::setIndexBuffer(m_ibh);
		bgfx::setVertexBuffer(0, m_vbh);
		bgfx::submit(id, program);
	}

	~Mesh() {
		// delete m_vertices;
		// m_vertices = nullptr;
		// delete m_indices;
		// m_indices = nullptr;
		bgfx::destroy(m_vbh);
		bgfx::destroy(m_ibh);
        // bgfx::destroy(m_layout);
	}
	// bgfx::
    // uint8_t _align[4];
	// uint16_t m_numVertices;
	// uint8_t *m_vertices;
	// uint32_t m_numIndices;
	// uint16_t *m_indices;
	std::vector<Vertex> m_vertices;
	std::vector<uint16_t> m_indices;

	bgfx::VertexLayout m_layout;
	bgfx::VertexBufferHandle m_vbh;
	bgfx::IndexBufferHandle m_ibh;
};
