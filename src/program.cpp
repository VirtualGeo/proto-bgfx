#include "Program.h"
#include <iostream>
#include <fstream>

#include <string>

//bgfx::ShaderHandle loadShader(const char* filename);
bgfx::ShaderHandle loadShader(const std::string & filename);

Program::Program()
{

}

Program::~Program()
{
//    bgfx::destroy(m_handle);
//    bgfx::destroy(m_uDiffuse);
//    bgfx::destroy(m_sDiffuse);

}

void Program::init(const char *shaderName)
{

//    bgfx::ShaderHandle vsh = loadShader("cubes.vert");
    bgfx::ShaderHandle vsh = loadShader(std::string(shaderName) + ".vert");
//    bgfx::ShaderHandle fsh = loadShader("cubes.frag");
    bgfx::ShaderHandle fsh = loadShader(std::string(shaderName) + ".frag");
    // bgfx::ShaderHandle vsh = loadShader("mesh.vert");
    // bgfx::ShaderHandle fsh = loadShader("mesh.frag");
    m_handle = bgfx::createProgram(vsh, fsh, true);

    m_uDiffuse = bgfx::createUniform("u_diffuse", bgfx::UniformType::Vec4);
    m_sDiffuse = bgfx::createUniform("s_diffuse", bgfx::UniformType::Sampler);

    m_sOpacity = bgfx::createUniform("s_opacity", bgfx::UniformType::Sampler);
    //    m_uHasDiffuseTexture = bgfx::createUniform("u_hasDiffuseTexture", bgfx::UniformType::Vec4);
    m_uTexturesEnable = bgfx::createUniform("u_texturesEnable", bgfx::UniformType::Vec4);
}

void Program::clear()
{
    bgfx::destroy(m_handle);
    bgfx::destroy(m_uDiffuse);
    bgfx::destroy(m_sDiffuse);

    bgfx::destroy(m_sOpacity);
    bgfx::destroy(m_uTexturesEnable);
}

//bgfx::ProgramHandle Program::program() const
//{
//    return m_program;
//}


bgfx::ShaderHandle loadShader(const std::string& filename)
//bgfx::ShaderHandle loadShader(const char* filename)
{
    //    const char* shaderPath = "???";
    std::string shaderPath;

    switch (bgfx::getRendererType()) {
    case bgfx::RendererType::Noop:
    case bgfx::RendererType::Direct3D9:
        shaderPath = "shaders/bin/dx9/";
        break;
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12:
        shaderPath = "shaders/bin/dx11/";
        break;
    case bgfx::RendererType::Gnm:
        shaderPath = "shaders/bin/pssl/";
        break;
    case bgfx::RendererType::Metal:
        shaderPath = "shaders/bin/metal/";
        break;
    case bgfx::RendererType::OpenGL:
        shaderPath = "shaders/bin/glsl/";
        break;
    case bgfx::RendererType::OpenGLES:
        shaderPath = "shaders/bin/essl/";
        break;
    case bgfx::RendererType::Vulkan:
        shaderPath = "shaders/bin/spirv/";
        break;
    }
    // shaderPath = "shaders/bin/";
    // shaderPath = "/home/gauthier/tmp2/proto-bgfx/shaders/bin/";

    //    size_t shaderLen = strlen(shaderPath);
    //    size_t fileLen = strlen(FILENAME);
    //    char* filePath = (char*)malloc(shaderLen + fileLen + 5);
    std::string filePath = PROJECT_DIR + shaderPath + filename + ".bin";
    //    memcpy(filePath, shaderPath, shaderLen);
    //    memcpy(&filePath[shaderLen], FILENAME, fileLen);
    //    memcpy(&filePath[shaderLen + fileLen], ".bin\0", 5);

    // std::cout << "current path : " << std::filesystem::current_path() <<
    // std::endl; char buff[80]; GetCurrentDirectory(buff, 80);

    // FILE *file = fopen(FILENAME, "rb");
    // FILE *file = fopen(filePath, "rb");
    // bx::FileReaderI * fileReader = nullptr;
    // bx::AllocatorI * allocator = nullptr;
    // allocator = & bx::DefaultAllocator;
    // fileReader = BX_NEW(allocator, FileReader);

    // FILE *file = fopen("shaders/bin/cubes.vert.bin", "rb");
    //    FILE* file = fopen(filePath, "rb");
    //    std::fstream file;
    std::ifstream file(filePath, std::ios::binary);
    //    file.open(filePath, std::ios::in | std::ios::binary);

    //    if (file == NULL) {
    if (!file.is_open()) {
        // perror(std::string("Failed to load ")+  std::string(FILENAME));
        std::cout << "[main] Failed to load '" << filePath << "' '" << filename << "'"
                  << std::endl;

        // throw std::runtime_error("Failed to load " + std::string(FILENAME));

        exit(-1);
    }
    //    fseek(file, 0, SEEK_END);
    //    file.seekg(0, std::ios::seekdir::_S_end);
    //    long fileSize = ftell(file);
    //    long fileSize = file.tellg();
    long begin = file.tellg();
    file.seekg(0, std::ios::end);
    long end = file.tellg();
    long fileSize = end - begin;
    //    fseek(file, 0, SEEK_SET);
    //    file.seekg(0)

    file.seekg(0, std::ios::beg);

    const bgfx::Memory* mem = bgfx::alloc(fileSize + 1);
    //    fread(mem->data, 1, fileSize, file);
    file.read((char*)mem->data, fileSize);
    mem->data[mem->size - 1] = '\0';
    //    fclose(file);
    file.close();

    //    free(filePath);

    return bgfx::createShader(mem);
}
