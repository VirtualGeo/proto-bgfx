

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
// #include "tiny_obj_loader.h"
// #include "stb_image.h"
// #include <stb_image.h>
// #include "stb/stb_image.h"
// #define STB_IMAGE_IMPLEMENTATION
// #include <stb/stb_image.h>

#include <chrono>
#include <direct.h>
#include <iostream>
#include <stdio.h>
#include <thread>
// #include <filesystem>
// #include <glm/glm.hpp>
// #include <assert.h>
#include "Mesh.h"
#include <cassert>

#define WIN_WIDTH 640
#define WIN_HEIGHT 480

void init();
void shutdown();
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void update();
bgfx::ShaderHandle loadShader(const char *FILENAME);
// bgfx::TextureHandle loadTexture(const char *FILENAME);

// ----------------- GLOBAL VARS
GLFWwindow *g_window = nullptr;
// bgfx::VertexBufferHandle vbh;
// bgfx::IndexBufferHandle ibh;
// bx::FileReaderI * fileReader = nullptr;
bgfx::ProgramHandle g_program;
unsigned int counter = 0;
// bgfx::UniformHandle g_texColor;
// bgfx::TextureHandle g_texture;
Mesh *g_mesh = nullptr;

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
	std::cout << "init done." << std::endl;

	// while (true)
	while (!glfwWindowShouldClose(g_window)) {
		processInput(g_window);

		update();
		// bgfx::frame();

		counter++;
		// std::this_thread::sleep_for(std::chrono::milliseconds(16));

		// glfwSwapBuffers(g_window);
		glfwPollEvents();
		// std::cout << "update " << counter << std::endl;
	}

	shutdown();
	// bgfx::shutdown(); // question : after or before glfwTerminate ?
	// glfwTerminate();

	return 0;
}

void init() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	g_window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Hello, bgfx from glfw!",
								NULL, NULL);
	if (g_window == NULL) {
		// std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		std::cout << "Failed to create GLFW window" << std::endl;
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
	bgfxInit.platformData.nwh = glfwGetWin32Window(g_window);

	bgfxInit.type =
		bgfx::RendererType::Count; // Automatically choose a renderer
	bgfxInit.resolution.width = WIN_WIDTH;
	bgfxInit.resolution.height = WIN_HEIGHT;
	bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
	// bgfxInit.resolution.reset = BGFX_RESET_NONE; // question
	// bgfx::init(bgfxInit);
	if (!bgfx::init(bgfxInit)) {
		std::cout << "Failed to initialize bgfx" << std::endl;
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

	// g_mesh = new Mesh("D:/proto-bgfx/Assets/Sponza/sponza.obj");
	// g_mesh = new Mesh("D:/proto-bgfx/Assets/McGuire/sponza/sponza.obj");
	g_mesh = new Mesh("Assets/McGuire/sponza/sponza.obj");
	// g_mesh = new Mesh("D:/proto-bgfx/Assets/Teapot/teapot.obj");
	// g_mesh = new Mesh("D:/proto-bgfx/Assets/McGuire/teapot/teapot.obj");

	// ----------------- INIT SHADER
	// fileReader = BX_NEW(allocator, fileReader);
	bgfx::ShaderHandle vsh = loadShader("cubes.vert");
	bgfx::ShaderHandle fsh = loadShader("cubes.frag");
	// bgfx::ShaderHandle vsh = loadShader("mesh.vert");
	// bgfx::ShaderHandle fsh = loadShader("mesh.frag");
	g_program = bgfx::createProgram(vsh, fsh, true);

	// g_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);

	// g_texture = loadTexture("textures/checkerboard.png");
	// g_texture = loadTexture("Assets/Sponza/textures/background_ddn.tga");
	// g_texture = loadTexture("Assets/McGuire/teapot/default.png");
	// g_texture = loadTexture("D:/proto-bgfx/Assets/Teapot/default.png");

	// loadTexture("D:/proto-bgfx/Assets/Teapot/default.png");

	// bgfx::ProgramHandle program;
	// program = loadProgram("cubes.vert", "cubes.frag");
}

void update() {
	// Set view 0 default viewport.
	bgfx::setViewRect(0, 0, 0, uint16_t(WIN_WIDTH), uint16_t(WIN_HEIGHT));

	// This dummy draw call is here to make sure that view 0 is cleared
	// if no other draw calls are submitted to view 0.
	bgfx::touch(0);


	// ------------------------- RENDER SCENE
	const bx::Vec3 at = {0.0f, 2.0f, 0.0f};
	const bx::Vec3 eye = {0.0f, 2.0f, -2.0f};
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
	// bx::mtxRotateXY(mtx, counter * 0.01f, counter * 0.01f);
	bx::mtxRotateY(mtx, counter * 0.01f);
	// bx::mtxScale(mtx, 0.1f);
	// bgfx::setTransform(mtx);

	const uint64_t state = 0 | BGFX_STATE_WRITE_R | BGFX_STATE_WRITE_G |
						   BGFX_STATE_WRITE_B | BGFX_STATE_WRITE_A |
						   BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS |
						   BGFX_STATE_CULL_CCW | BGFX_STATE_MSAA | s_ptState[0];
	// UINT64_C(0), BGFX_STATE_PT_TRISTRIP,

	// const uint64_t stateTransparent =
	// 	0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
	// 	BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CCW | BGFX_STATE_MSAA |
	// 	BGFX_STATE_BLEND_ALPHA;

	// const uint64_t stateOpaque = BGFX_STATE_DEFAULT;

	// state = BGFX_STATE_DEFAULT;
	// bgfx::submit(0, program);
	// bgfx::setVertexBuffer(0, vbh);
	// bgfx::setIndexBuffer(ibh);
	// bgfx::setTransform(mtx);
	// bgfx::setState(state);
	// bgfx::setTexture(0, g_texColor, g_texture);
	g_mesh->submit(0, g_program, mtx, state);
	// g_mesh->submit(0, g_program, mtx, stateTransparent);
	// g_mesh->submit(0, g_program, mtx, stateOpaque);

	// bgfx::submit(0, g_program);

	bgfx::frame();
}

void shutdown() {
	// bgfx::destroy(ibh);
	// bgfx::destroy(vbh);

	// stbi_image_free(image);

	delete g_mesh;
	g_mesh = nullptr;
	bgfx::destroy(g_program);
	// bgfx::destroy(g_texture);
	// bgfx::destroy(g_texColor);
	bgfx::shutdown();

	// glfwMakeContextCurrent(nullptr);
	glfwDestroyWindow(g_window);
	// window = nullptr; // question : necessary ?
	glfwTerminate();
	// glfwMakeContextCurrent(g_window);
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

// unsigned char *image = nullptr;

// const bgfx::Memory *stippleTex;
