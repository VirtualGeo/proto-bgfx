#include "Texture.h"
#include <bx/readerwriter.h>
#include <cassert>
#include <iostream>

#include "System.h"

// Texture::Texture(const char *filename) {
Texture::Texture(const std::string& diffuseTexName, const std::string& baseDir)
{
    //                    GLuint texture_id;
//    uint texture_id;
    int w, h;
    int comp;

    std::string texture_filename = diffuseTexName;
    if (!FileExists(texture_filename)) {
        // Append base dir.
        texture_filename = baseDir + diffuseTexName;
        if (!FileExists(texture_filename)) {
            std::cerr << "    [Texture] Unable to find file: " << diffuseTexName
                      << std::endl;
            exit(1);
        }
    }

    unsigned char* image = stbi_load(texture_filename.c_str(), &w, &h, &comp, STBI_default);
    if (!image) {
        std::cerr << "    [Texture] Unable to load texture: " << texture_filename
                  << std::endl;
        exit(1);
    }
    std::cout << "    [Texture] Loaded texture: '" << texture_filename << "', w = " << w
              << ", h = " << h << ", comp = " << comp << std::endl;

    const size_t imageSize = w * h * comp;
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
    stbi_image_free(m_image);
    m_image = nullptr;
    //	bgfx::destroy(m_sampler);
    bgfx::destroy(m_textureHandle);
}
