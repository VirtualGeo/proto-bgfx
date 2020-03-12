
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
	float tx;
	float ty;
};

struct Group {

	std::vector<Vertex> m_vertices;
	std::vector<uint16_t> m_indices;

	bgfx::VertexBufferHandle m_vbh;
	bgfx::IndexBufferHandle m_ibh;

};
typedef std::vector<Group> GroupArray;

struct Mesh {

	// Mesh(const char *filePath);
	Mesh(const char *filePath);
	void submit(bgfx::ViewId id, bgfx::ProgramHandle program, const float *mtx,
				uint64_t state);
	~Mesh();
	// bgfx::
	// uint8_t _align[4];
	// uint16_t m_numVertices;
	// uint8_t *m_vertices;
	// uint32_t m_numIndices;
	// uint16_t *m_indices;

	// std::vector<Vertex> m_vertices;
	// std::vector<uint16_t> m_indices;

	bgfx::VertexLayout m_layout;
	GroupArray m_groups;
	// bgfx::VertexBufferHandle m_vbh;
	// bgfx::IndexBufferHandle m_ibh;
};
