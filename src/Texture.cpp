#include "Texture.h"
#include <bx/readerwriter.h>
#include <cassert>
#include <iostream>

#include "System.h"
#include "FileSystem.h"
#include <string>

// Texture::Texture(const char *filename) {
Texture::Texture(const std::string& texName, const std::string& baseDir)
{
    //                    GLuint texture_id;
    m_name = texName;
    m_baseDir = baseDir;

    const std::string absoluteTexName = baseDir + texName;

    //    uint texture_id;
    int w, h;
    int comp;

    assert(!FileExists(texName));

    //    std::string texture_filename = diffuseTexName;
    //    if (!FileExists(texture_filename)) {
    // Append base dir.
    //        texture_filename = baseDir + diffuseTexName;
    if (!FileExists(absoluteTexName)) {
        std::cerr << "    [Texture] Unable to find file: " << absoluteTexName
                  << std::endl;
        exit(1);
    }
    //    }

//    unsigned char* image = stbi_load(absoluteTexName.c_str(), &w, &h, &comp, STBI_default);
    m_image = stbi_load(absoluteTexName.c_str(), &w, &h, &comp, STBI_default);
    if (!m_image) {
        std::cerr << "    [Texture] Unable to load texture: " << absoluteTexName
                  << std::endl;
        exit(1);
    }
    std::cout << "    [Texture] Loaded texture: '" << absoluteTexName << "', w = " << w
              << ", h = " << h << ", comp = " << comp << std::endl;

    const size_t imageSize = w * h * comp;
    m_textureSize = imageSize; // bytes
//    m_textureSize = imageSize * 8;
    const uint64_t textureFlags = 0 | BGFX_TEXTURE_NONE;
    const uint64_t samplerFlags = 0 | BGFX_SAMPLER_NONE;
    bool hasMips = false;
    //                    glGenTextures(1, &texture_id);
    //                    glBindTexture(GL_TEXTURE_2D, texture_id);
    //                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (comp == 3) {
        //                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB,
        //                            GL_UNSIGNED_BYTE, image);
        m_textureHandle = bgfx::createTexture2D(
            w, h, hasMips, 1, bgfx::TextureFormat::RGB8,
            // BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIN_POINT, stippleTex);
            textureFlags | samplerFlags, bgfx::makeRef(m_image, imageSize));
        // textureFlags | samplerFlags, mem);
    } else if (comp == 4) {
        m_textureHandle = bgfx::createTexture2D(
            w, h, hasMips, 1, bgfx::TextureFormat::RGBA8,
            textureFlags | samplerFlags, bgfx::makeRef(m_image, imageSize));
        //                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
        //                            GL_UNSIGNED_BYTE, image);
    } else {
        std::cout << "    [Texture] unable to load texture with " << comp << " channels" << std::endl;
        assert(0); // TODO
    }
    //                    glBindTexture(GL_TEXTURE_2D, 0);

    //    stbi_image_free(image);

    //	const uint64_t textureFlags = 0 | BGFX_TEXTURE_NONE;
    //	const uint64_t samplerFlags = 0 | BGFX_SAMPLER_NONE;
    //	if (nbChannel == 3) {

    //	} else if (nbChannel == 1) {
    //		m_texture = bgfx::createTexture2D(
    //			width, height, hasMips, 1, bgfx::TextureFormat::R8,
    //			textureFlags | samplerFlags, bgfx::makeRef(m_image, imageSize));
    //	} else if (nbChannel == 4) {
    //	}
    //    else {
    //        std::cout << "unable to load texture with " << nbChannel << " channels" << std::endl;
    //        exit(1);
    //    }
    // bgfx::TextureHandle loadTexture(const char *filename) {

    //	int width;
    //	int height;
    //	int nbChannel;
    //	m_image =
    //		stbi_load(texName.c_str(), &width, &height, &nbChannel, STBI_rgb);
    //	// stbi_load(filename, &width, &height, &comp, STBI_rgb);
    //	size_t imageSize = width * height * nbChannel;

    //	if (m_image == nullptr) {
    //		// throw(std::string("Failed to load texture : ") +
    //		// std::string(filename)); throw(std::string("Failed to load texture
    //		// :
    //		// "));
    //		std::cout << "Failed to load texture '" << texName.c_str() << "'"
    //				  << std::endl;
    //		exit(1);
    //		// throw std::runtime_error("Failed to load texture");
    // assert(nbChannel == 3);
    // stbi_image_free(image);
    // image = nullptr;
    // return bgfx::createTexture2D();
    // bool hasMips = true; imageSize = 65535;
    // const bgfx::Memory *texPtr = bgfx::alloc(imageSize);
    // stippleTex = bgfx::alloc(imageSize);
    // bx::memSet(stippleTex->data, 0, stippleTex->size);
    // texPtr->data = (uint8_t*)std::move(image);
    // const uint64_t textureFlags = 0 | BGFX_CAPS_FORMAT_TEXTURE_MIP_AUTOGEN;
    // BGFX_CAPS_FORMAT_TEXTURE_MIP_AUTOGEN;
    // const uint64_t textureFlags = 0 | BGFX_TEXTURE_NONE;
    // BGFX_CAPS_FORMAT_TEXTURE_MIP_AUTOGEN
    // const uint64_t samplerFlags =
    // 0 | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIN_POINT;

    // bgfx::TextureInfo info;
    // bgfx::calcTextureSize(info, width, height, 0, false, true, 0,
    // bgfx::TextureFormat::Enum::RGB8);
    // // std::cout << "texture info : " << info.format << info.storageSize
    // << info.depth << info.numMips << info.bitsPerPixel << std::endl;
    // assert(imageSize == info.storageSize);
    // imageSize = info.storageSize;
    //	m_sampler = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
    // const bgfx::Memory * mem = bgfx::copy(m_image, imageSize * 8);
    // const bgfx::Memory * mem = bgfx::alloc(imageSize);
    // bx::memCopy(mem, image, imageSize);
    // bgfx::copy(m_image, )
    // bgfx::blit()

    // bgfx::alloc()
    // stbi_image_free(m_image);
    // m_image = nullptr;

    // stbi_image_free(image);
    // image = nullptr;
    // }
}

Texture::~Texture()
{
//    std::cout << "\033[31m";
//    std::cout << "[Texture] " << this << " delete texture '" << m_name << "' " << m_image << ", textureHandle=" << m_textureHandle.idx << std::endl;
//    std::cout << "\033[0m";

//    assert(m_image);
    if (m_image != nullptr) {
        stbi_image_free(m_image);
        m_image = nullptr;

        bgfx::destroy(m_textureHandle);
//        std::cout << "bgfx destroy " << std::endl;
    }
//    else {
//        std::cout << "bgfx bad destroy " << std::endl;
//    }
    //	bgfx::destroy(m_sampler);
    //    bgfx::isValid(m_textureHandle)
//    assert(bgfx::isValid(m_textureHandle));
//    bgfx::destroy(m_textureHandle);
    //    std::cout << "[Texture] " << m_name << " deleted" << std::endl;

    std::cout << "\033[31m";
    std::cout << "[Texture] " << this << " '" << m_name << "' deleted" << std::endl;
    std::cout << "\033[0m";
}

//Texture::Texture(std::ifstream &file)
//{
////    FileSystem::read(m_name, file);
////    FileSystem::read(m_baseDir, file);
////    Texture::Texture(m_name, m_baseDir);

//}

void Texture::save(std::ofstream &file)
{
    FileSystem::write(m_name, file);
    FileSystem::write(m_baseDir, file);

}

const bgfx::TextureHandle& Texture::textureHandle() const
{
    return m_textureHandle;
}

std::string Texture::name() const
{
    return m_name;
}

Texture::Texture(Texture&& texture)
    : m_image(texture.m_image)
    , m_textureHandle(std::move(texture.m_textureHandle))
    , m_name(std::move(texture.m_name))
    , m_baseDir(std::move(texture.m_baseDir))
{
    texture.m_image = nullptr;
//    texture.m_textureHandle.
    std::cout << "\033[34m";
    std::cout << "[Texture] " << this << " '" << m_name << "' right moving from " << &texture << std::endl;
    std::cout << "\033[0m";
}

size_t Texture::textureSize() const
{
    return m_textureSize;
}
