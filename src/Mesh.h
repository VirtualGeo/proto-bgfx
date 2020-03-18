
//#pragma ounce
#ifndef __MESH_H
#define __MESH_H
// #include "bgfx/bgfx.h"

#include <bgfx/bgfx.h>
// #include "bgfx/platform.h"
#include <vector>
// #include <tinyobj/>
//#define TINYOBJLOADER_IMPLEMENTATION // question : why not possible to include tiny_obj_loader here ?
#include "tiny_obj_loader.h"

#include <cassert>
#include <iostream>
#include "Texture.h"
// #include <bx/bx.h>
#include <bx/file.h>
#include <bx/macros.h>
#include <bx/uint32_t.h>

// bgfx::UniformType::Vec4 f;
// bx::Vec4;

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

// struct Texture {
//     // uint32_t m_flags;
//     bgfx::UniformHandle m_sampler;
//     bgfx::TextureHandle m_texture;
//     // uint8_t m_stage;
// };

struct Group {

	std::vector<Vertex> m_vertices;
	std::vector<uint16_t> m_indices;


    Texture * m_texture = nullptr;
	// loadTexture("D:/proto-bgfx/Assets/Teapot/default.png");

	bgfx::UniformHandle m_uDiffuse;
	float m_diffuse[4];
	// float m_diffuseColor[3];
	// bool m_hasDiffuseTexture = false;
	// bgfx::UniformHandle m_uHasDiffuseTexture;
	// bx::Vec3 m_diffuse;

	bgfx::VertexBufferHandle m_vbh;
	bgfx::IndexBufferHandle m_ibh;

};
typedef std::vector<Group> GroupArray;

class Mesh {

public:

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

private:
	bgfx::VertexLayout m_layout;
	GroupArray m_groups;
    // std::string m_path;
	// bgfx::VertexBufferHandle m_vbh;
	// bgfx::IndexBufferHandle m_ibh;
};

#endif // __MESH_H
