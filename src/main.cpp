

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
// #include "bx/file.h"
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"
#include "logo.h"
#include <bx/file.h>
#include <bx/macros.h>
#include <bx/uint32_t.h>
// #include "bgfx/vertexdecl.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <chrono>
#include <direct.h>
#include <iostream>
#include <stdio.h>
#include <thread>
// #include <filesystem>
#include <glm/glm.hpp>
// #include <assert.h>
#include <cassert>

#define WIN_WIDTH 640
#define WIN_HEIGHT 480

void init();
void shutdown();
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void update();
bgfx::ShaderHandle loadShader(const char *FILENAME);

// ----------------- GLOBAL VARS
GLFWwindow *window = nullptr;
bgfx::VertexBufferHandle vbh;
bgfx::IndexBufferHandle ibh;
// bx::FileReaderI * fileReader = nullptr;
bgfx::ProgramHandle program;
unsigned int counter = 0;

struct PosColorVertex {
	float x;
	float y;
	float z;
	// uint32_t abgr;
	float nx;
	float ny;
	float nz;
};

// static PosColorVertex cubeVertices[] = {
// 	{-1.0f, 1.0f, 1.0f, 0xff000000},   {1.0f, 1.0f, 1.0f, 0xff0000ff},
// 	{-1.0f, -1.0f, 1.0f, 0xff00ff00},  {1.0f, -1.0f, 1.0f, 0xff00ffff},
// 	{-1.0f, 1.0f, -1.0f, 0xffff0000},  {1.0f, 1.0f, -1.0f, 0xffff00ff},
// 	{-1.0f, -1.0f, -1.0f, 0xffffff00}, {1.0f, -1.0f, -1.0f, 0xffffffff},
// };

static PosColorVertex cubeVertices[] = {
	{-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f},   {1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
	{-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f},  {1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f},
	{-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f},  {1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f},
	{-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f}, {1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f},
};

static const uint16_t cubeTriList[] = {
	0, 1, 2, 1, 3, 2, 4, 6, 5, 5, 6, 7, 0, 2, 4, 4, 2, 6,
	1, 5, 3, 5, 7, 3, 0, 4, 1, 4, 5, 1, 2, 3, 6, 6, 3, 7,
};

static const char *s_ptNames[]{
	"Triangle List",
	// "Triangle Strip",
	// "Lines",
	// "Line Strip",
	// "Points",
};

static const uint64_t s_ptState[]{
	UINT64_C(0), BGFX_STATE_PT_TRISTRIP,
	// BGFX_STATE_PT_LINES,
	// BGFX_STATE_PT_LINESTRIP,
	// BGFX_STATE_PT_POINTS,
};
// BX_STATIC_ASSERT(BX_COUNTOF(s_ptState) == BX_COUNTOF(s_ptNames));

// struct Vertex {
// 	glm::vec3 position;
// 	glm::vec3 normal;
// 	// glm::vec2 uvTexture;
// };

struct Vertex {
	float x;
	float y;
	float z;
	float nx;
	float ny;
	float nz;
};
// #define WIN32_LEAN_AND_MEAN
// #include <windows.h>
// #include <Shlwapi.h>
// #pragma comment(lib, "shlwapi.lib")

// #include <iostream>
// #include <string>
// #include <cstring>
// // using namespace std;

// inline std::string GetExeDir()
// {
//     char path[MAX_PATH] = "";
//     GetModuleFileNameA(NULL, path, MAX_PATH);
//     PathRemoveFileSpecA(path);
//     PathAddBackslashA(path);
//     return path;
// }

// std::string GetWorkingDir()
// {
//     char path[MAX_PATH] = "";
//     GetCurrentDirectoryA(MAX_PATH, path);
//     PathAddBackslashA(path);
//     return path;
// }

// int main() {
// 	std::string exeDir     = GetExeDir();
// 	std::string workingDir = GetWorkingDir();
// 	std::cout << "Exe Dir     = " << exeDir     << "\n";
// 	std::cout << "Working Dir = " << workingDir << "\n";
// 	return 0;
// }

int main(int argc, char const *argv[]) {
	std::cout << "hello bgfx !!!" << std::endl;

	init();

	// while (true)
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		update();
		// bgfx::frame();

		counter++;
		// std::this_thread::sleep_for(std::chrono::milliseconds(16));

		// glfwSwapBuffers(window);
		glfwPollEvents();
		std::cout << "update " << counter << std::endl;
	}

	shutdown();
	// bgfx::shutdown(); // question : after or before glfwTerminate ?
	// glfwTerminate();

	return 0;
}

void update() {
	bgfx::setViewRect(0, 0, 0, uint16_t(WIN_WIDTH), uint16_t(WIN_HEIGHT));
	bgfx::touch(0);

	// bgfx::dbgTextClear();
	// bgfx::dbgTextImage(bx::max<uint16_t>(uint16_t(WIN_WIDTH / 2 / 8), 20) -
	// 20, 				   bx::max<uint16_t>(uint16_t(WIN_HEIGHT / 2 / 16), 6) -
	// 6, 40, 12, s_logo, 160);

	// bgfx::dbgTextPrintf(0, 1, 0x0f,
	// 					"Color can be changed with ANSI "
	// 					"\x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b["
	// 					"14;me\x1b[0m code too.");

	// bgfx::dbgTextPrintf(
	// 	80, 1, 0x0f,
	// 	"\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; "
	// 	"5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
	// bgfx::dbgTextPrintf(
	// 	80, 2, 0x0f,
	// 	"\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    "
	// 	"\x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");

	// const bgfx::Stats *stats = bgfx::getStats();
	// bgfx::dbgTextPrintf(
	// 	0, 2, 0x0f,
	// 	"Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.",
	// 	stats->width, stats->height, stats->textWidth, stats->textHeight);

	// ------------------------- RENDER SCENE
	const bx::Vec3 at = {0.0f, 0.0f, 0.0f};
	const bx::Vec3 eye = {0.0f, 0.0f, -2.0f};
	// {
	float view[16];
	bx::mtxLookAt(view, eye, at);

	float proj[16];
	bx::mtxProj(proj, 60.0f, float(WIN_WIDTH) / float(WIN_HEIGHT), 0.1f, 100.0f,
				bgfx::getCaps()->homogeneousDepth);
	// bgfx::setViewTransform(0, view, proj);
	bgfx::setViewTransform(0, view, proj);

	// }
	// bgfx::setViewRect(0, 0, 0, uint16_t(WIN_WIDTH), uint16_t(WIN_HEIGHT));
	// bgfx::touch(0);

	float mtx[16];
	bx::mtxRotateXY(mtx, counter * 0.01f, counter * 0.01f);
	bgfx::setTransform(mtx);

	// bgfx::submit(0, program);
	bgfx::setVertexBuffer(0, vbh);
	bgfx::setIndexBuffer(ibh);

	uint64_t state = 0 | BGFX_STATE_WRITE_R | BGFX_STATE_WRITE_G |
					 BGFX_STATE_WRITE_B | BGFX_STATE_WRITE_A |
					 BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS |
					 BGFX_STATE_CULL_CW | BGFX_STATE_MSAA | s_ptState[0];

	bgfx::setState(state);

	bgfx::submit(0, program);

	bgfx::frame();
}
std::vector<Vertex> vertices;
std::vector<uint16_t> triangle_list;

void init() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Hello, bgfx from glfw!",
							  NULL, NULL);
	if (window == NULL) {
		// std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
		// exit(-1);
	}
	// glfwMakeContextCurrent(window); // question : what does this fonction ?
	// glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// bgfx::RenderFrame();
	bgfx::renderFrame();

	// bgfx::PlatformData pd;
	// pd.nwh = glfwGetWin32Window(window);
	// bgfx::setPlatformData(pd);

	bgfx::Init bgfxInit = {};
	// bgfxInit.type = bgfx::RendererType::OpenGL;
	bgfxInit.platformData.nwh = glfwGetWin32Window(window);

	bgfxInit.type =
		bgfx::RendererType::Count; // Automatically choose a renderer
	bgfxInit.resolution.width = WIN_WIDTH;
	bgfxInit.resolution.height = WIN_HEIGHT;
	bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
	// bgfxInit.resolution.reset = BGFX_RESET_NONE; // question
	// bgfx::init(bgfxInit);
	if (!bgfx::init(bgfxInit)) {
		throw std::runtime_error("Failed to initialize bgfx");
		// exit(1);
		// return 1;
	}

	// bgfx::setDebug(BGFX_DEBUG_TEXT);
	// bgfx::setDebug(BGFX_DEBUG_WIREFRAME);

	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFF0000FF, 1.0f,
					   0);
	bgfx::setViewRect(0, 0, 0, WIN_WIDTH, WIN_HEIGHT);
	bgfx::touch(0);

	// glfwMakeContextCurrent(nullptr); // question : why we can do it ?

	// ------------- INIT GEOMETRY
	// bgfx::VertexDecl pcvDecl; // question : out of date ?
	bgfx::VertexLayout ms_layout;
	ms_layout.begin()
		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
		// .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
		// .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
		.end();

	// bgfx::VertexBufferHandle vbh;

	// std::string inputFile = "D:/proto-bgfx/Assets/Sponza/sponza.obj";
	std::string inputFile = "D:/proto-bgfx/Assets/Teapot/teapot.obj";
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	const bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
									  inputFile.c_str());

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

	if (attrib.vertices.empty() || attrib.normals.empty() || attrib.texcoords.empty()) {
		throw std::runtime_error("bad vertices");
	}
	// std::vector<float[8]> vertices;
	// std::vector<Vertex> vertices;
	vertices.clear();
	// std::vector<uint16_t> triangle_list;

	// attrib.vertices;
	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
				tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
				tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
				tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
				// tinyobj::real_t tx =
				// 	attrib.texcoords[2 * idx.texcoord_index + 0];
				// tinyobj::real_t ty =
				// 	attrib.texcoords[2 * idx.texcoord_index + 1];
				
				// vertices.push_back({{vx, vy, vz}, {nx, ny, nz}}); // question : emplace_back ?
				vertices.push_back({vx, vy, vz, nx, ny, nz}); // question : emplace_back ?
				// Optional: vertex colors
				// tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
				// tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
				// tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];
				triangle_list.push_back(uint16_t(triangle_list.size()));
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
	const uint16_t stride = ms_layout.getStride();

	// const bgfx::Memory * mem = bgfx::alloc(vertices.size * stride);
	// bx::memCopy(vertices, )
	// vbh = bgfx::createVertexBuffer(mem, ms_layout);
	assert(stride == sizeof(Vertex));

	vbh = bgfx::createVertexBuffer(
		bgfx::makeRef(vertices.data(), sizeof(Vertex) * vertices.size()), ms_layout);
	ibh = bgfx::createIndexBuffer(
		bgfx::makeRef(triangle_list.data(), sizeof(uint16_t) * triangle_list.size()));

	// ----------------- INIT SHADER
	// fileReader = BX_NEW(allocator, fileReader);
	bgfx::ShaderHandle vsh = loadShader("cubes.vert");
	bgfx::ShaderHandle fsh = loadShader("cubes.frag");
	// bgfx::ShaderHandle vsh = loadShader("mesh.vert");
	// bgfx::ShaderHandle fsh = loadShader("mesh.frag");
	program = bgfx::createProgram(vsh, fsh, true);

	// bgfx::ProgramHandle program;
	// program = loadProgram("cubes.vert", "cubes.frag");
}

void shutdown() {
	glfwMakeContextCurrent(window);
	bgfx::destroy(ibh);
	bgfx::destroy(vbh);
	bgfx::destroy(program);
	bgfx::shutdown();

	glfwMakeContextCurrent(nullptr);
	glfwDestroyWindow(window);
	// window = nullptr; // question : necessary ?
	glfwTerminate();
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	// glViewport(0, 0, width, height);
}

bgfx::ShaderHandle loadShader(const char *FILENAME) {
	const char *shaderPath = "???";

	// switch (bgfx::getRendererType()) {
	// case bgfx::RendererType::Noop:
	// case bgfx::RendererType::Direct3D9:
	// 	shaderPath = "shaders/bin/dx9/";
	// 	break;
	// case bgfx::RendererType::Direct3D11:
	// case bgfx::RendererType::Direct3D12:
	// 	shaderPath = "shaders/bin/dx11/";
	// 	break;
	// case bgfx::RendererType::Gnm:
	// 	shaderPath = "shaders/bin/pssl/";
	// 	break;
	// case bgfx::RendererType::Metal:
	// 	shaderPath = "shaders/bin/metal/";
	// 	break;
	// case bgfx::RendererType::OpenGL:
	// 	shaderPath = "shaders/bin/glsl/";
	// 	break;
	// case bgfx::RendererType::OpenGLES:
	// 	shaderPath = "shaders/bin/essl/";
	// 	break;
	// case bgfx::RendererType::Vulkan:
	// 	shaderPath = "shaders/bin/spirv/";
	// 	break;
	// }
	shaderPath = "shaders/bin/";

	size_t shaderLen = strlen(shaderPath);
	size_t fileLen = strlen(FILENAME);
	char *filePath = (char *)malloc(shaderLen + fileLen + 5);
	memcpy(filePath, shaderPath, shaderLen);
	memcpy(&filePath[shaderLen], FILENAME, fileLen);
	memcpy(&filePath[shaderLen + fileLen], ".bin\0", 5);

	// std::cout << "current path : " << std::filesystem::current_path() <<
	// std::endl; char buff[80]; GetCurrentDirectory(buff, 80);

	// FILE *file = fopen(FILENAME, "rb");
	// FILE *file = fopen(filePath, "rb");
	// bx::FileReaderI * fileReader = nullptr;
	// bx::AllocatorI * allocator = nullptr;
	// allocator = & bx::DefaultAllocator;
	// fileReader = BX_NEW(allocator, FileReader);

	// FILE *file = fopen("shaders/bin/cubes.vert.bin", "rb");
	FILE *file = fopen(filePath, "rb");
	if (file == NULL) {
		// perror(std::string("Failed to load ")+  std::string(FILENAME));
		std::cout << "Failed to load '" << filePath << "' '" << FILENAME << "'"
				  << std::endl;

		// throw std::runtime_error("Failed to load " + std::string(FILENAME));

		exit(-1);
	}
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	const bgfx::Memory *mem = bgfx::alloc(fileSize + 1);
	fread(mem->data, 1, fileSize, file);
	mem->data[mem->size - 1] = '\0';
	fclose(file);

	free(filePath);

	return bgfx::createShader(mem);
}
