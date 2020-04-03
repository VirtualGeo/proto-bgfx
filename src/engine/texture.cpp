#include "texture.h"
#include <bx/readerwriter.h>
#include <cassert>
#include <iostream>

#include "fileIO.h"
#include "system.h"
#include <string>

//#include "bgfx_utils.h"
#include <cstring>

//#include <bimg/decode.h>
//#include <bimg/encode.h>
//#include <bimg/bimg.h>
//#include <stb/stb.h>
//#define STB_IMAGE_IMPLEMENTATION // question : bimg failed to load stb_image
#include <stb/stb_image.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image_resize.h>

//u_int64_t Texture::s_textureFlags = 0 | BGFX_TEXTURE_NONE;
//u_int64_t Texture::s_samplerFlags = 0 | BGFX_SAMPLER_NONE;
//u_int64_t Texture::s_textureSamplerFlags = 0 | BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE;
uint64_t Texture::s_textureSamplerFlags = 0 | BGFX_TEXTURE_NONE | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_ANISOTROPIC;

// Texture::Texture(const char *filename) {
Texture::Texture(const std::string& texName, const std::string& baseDir)
{
    //                    GLuint texture_id;
    m_name = texName;
    m_baseDir = baseDir;

    const std::string absoluteTexName = baseDir + texName;

    //    uint texture_id;
    //    int w, h;
    //    int comp;

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

    //    m_textureHandle = loadTexture(absoluteTexName.c_str(), textureFlags | samplerFlags, 0, &m_texInfo, &m_orientation);

    //    unsigned char* image = stbi_load(absoluteTexName.c_str(), &w, &h, &comp, STBI_default);
    uint8_t* data = nullptr;
    //    stbi_set_flip_vertically_on_load(1);
    data = stbi_load(absoluteTexName.c_str(), &m_width, &m_height, &m_nChannels, STBI_default);
    if (!data) {
        std::cerr << "    [Texture] Unable to load texture: " << absoluteTexName
                  << std::endl;
        exit(1);
    }
    //        std::cout << "    [Texture] Loaded texture: '" << absoluteTexName << "', w = " << w
    //                  << ", h = " << h << ", comp = " << comp << std::endl;

    //    const size_t imageSize = w * h * comp;
    //    m_textureSize = imageSize; // bytes
    //    m_textureSize = m_width * m_height * m_nChannels;
    //    m_textureSize = imageSize * 8;
    //    bool hasMips = false;
    //    bgfx::calcTextureSize(m_texInfo, m_width, m_height, 1, false, true, 1, texFormat);

    //            const int numMips = 1 + bx::floor(bx::log2(bx::max(m_width, m_height)));
    m_nMips = bx::floor(bx::log2(bx::max(m_width, m_height)));

    int width = m_width;
    int height = m_height;
    int textureSize = width * height;
    //    const int lenMax = static_cast<int>(bx::pow(2.0f, 2.0f * numMips + 1.0f)) * m_nChannels;
    //    int nMips = 0;
    //    while (width > 1 || height > 1) {
    //        width = bx::max(1, width >> 1);
    //        height = bx::max(1, height >> 1);

    //    }
    for (int i = 0; i < m_nMips; ++i) {
        //    while (width != 1 || height != 1) {
        //        const int a = bx::pow(2, i);
        //        const int mipSize = width * height * m_nChannels;
        //        textureSize += a * a;

        //        memset(&mipMaps, i, mipSize);
        //        memcpy(&mipMaps[offset], data, width * height * m_nChannels * sizeof (unsigned char));
        //        offset += width * height * m_nChannels;

        //        width = bx::ceil(width / 2.0f);
        //        height = bx::ceil(height / 2.0f);
        width = bx::max(1, width >> 1);
        height = bx::max(1, height >> 1);
        textureSize += width * height;
        //        ++nMips;
    }
    //    m_nMips = nMips;
    //    ++textureSize; // avoid max(1, width / 2) with width = 1 -> width / 2 = 0 for 1x1 mipmap
    //    assert(nMips == bx::floor(bx::log2(bx::max(m_width, m_height))));
    //    assert(width == 1 && height == 1);
    m_textureSize = textureSize * m_nChannels;
    //    assert(lenMax > m_textureSize);

    m_image = (uint8_t*)malloc(m_textureSize);
    //        memset(m_image, 0xFF, m_textureSize);
    memcpy(m_image, data, (m_width * m_height * m_nChannels));
    stbi_image_free(data);
    data = nullptr;

    width = m_width;
    height = m_height;
    int offset = 0;
    for (int i = 1; i < m_nMips; ++i) {
        const int previousMipSize = width * height * m_nChannels;
        //        if (i % 2 == 0) {
        const int futureWidth = bx::max(1, width >> 1);
        const int futureHeight = bx::max(1, height >> 1);
        //            memset(&m_image[offset], 0xFF, mipSize);
        //        } else {
        //            memset(&m_image[offset], 0x00, mipSize);
        //        }
        //        bimg::imageRgba8Downsample2x2(&m_image[offset], width, height, 1, )
        //        const int bsup = offset + previousMipSize + futureWidth * futureHeight * m_nChannels;
        assert(offset + previousMipSize + futureWidth * futureHeight * m_nChannels < m_textureSize);
        stbir_resize_uint8(&m_image[offset], width, height, 0, &m_image[offset + previousMipSize], futureWidth, futureHeight, 0, m_nChannels);

        offset += previousMipSize;
        //        width = bx::ceil(width / 2.0f);
        //        height = bx::ceil(height / 2.0f);
        width = futureWidth;
        height = futureHeight;
    }
    //    assert(width == 1 && height == 1);
    //    std::cout << sizeof (uint8_t) << std::endl;
    //    memcpy(m_image, mipMaps, m_textureSize * sizeof(uint8_t));
    //    m_image = std::move(mipMaps);
    //    free(data);
    //    std::cout << "'" << m_name << "', width=" << m_width << ", height=" << m_height << ", nbChannel=" << m_nChannels << ", textureSize=" << m_textureSize << std::endl;
    createTextureHandle();
//    free(m_image); // not possible because scene.save() need image data to save bin model
//    m_image = nullptr;
#ifdef DEBUG
    std::cout << "    [Material] Load texture : '" << m_name << "', width=" << m_width << ", height=" << m_height << ", nbChannel=" << m_nChannels << ", textureSize=" << m_textureSize << ", bitPerPixel=" << m_nChannels << std::endl;
#endif
}

Texture::Texture(Texture&& texture)
    : m_textureHandle(std::move(texture.m_textureHandle))
    //        : m_textureHandle(texture.m_textureHandle)
    , m_name(std::move(texture.m_name))
    , m_baseDir(std::move(texture.m_baseDir))

    , m_width(texture.m_width)
    , m_height(texture.m_height)
    , m_nChannels(texture.m_nChannels)
    , m_textureSize(texture.m_textureSize)
    , m_image(texture.m_image)
    , m_nMips(texture.m_nMips)

//    , m_texInfo(std::move(texture.m_texInfo))
//    , m_orientation(std::move(texture.m_orientation))
{
    texture.m_last = false;
    texture.m_image = nullptr;

//    texture.m_textureHandle.
#ifdef DEBUG
    std::cout << "\033[34m";
    std::cout << "[Texture] " << this << " '" << m_name << "' right moving from " << &texture << std::endl;
    std::cout << "\033[0m";
#endif
}

Texture::~Texture()
{
    //    std::cout << "\033[31m";
    //    std::cout << "[Texture] " << this << " delete texture '" << m_name << "' " << m_image << ", textureHandle=" << m_textureHandle.idx << std::endl;
    //    std::cout << "\033[0m";

    //    assert(m_image);
    //    if (m_image != nullptr) {
    if (m_last) {
        //                stbi_image_free(m_image);
        if (m_image != nullptr) {
            free(m_image);
            m_image = nullptr;
        }

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

    //#ifdef DEBUG
    //    std::cout << "\033[31m";
    //    std::cout << "[Texture] " << this << " '" << m_name << "' deleted" << std::endl;
    //    std::cout << "\033[0m";
    //#endif
}

Texture::Texture(std::ifstream& file)
{
    FileIO::read(m_name, file);
    FileIO::read(m_baseDir, file);
    //    Texture::Texture(m_name, m_baseDir);
    //    const bool hasMips = true;
    FileIO::read(m_width, file);
    FileIO::read(m_height, file);
    FileIO::read(m_nChannels, file);

    FileIO::read(m_textureSize, file);
    m_image = (uint8_t*)malloc(m_textureSize);
    FileIO::read(m_image, m_textureSize, file);
    FileIO::read(m_nMips, file);

    createTextureHandle();
    free(m_image);
    m_image = nullptr;
}

void Texture::save(std::ofstream& file) const
{
    FileIO::write(m_name, file);
    FileIO::write(m_baseDir, file);

    FileIO::write(m_width, file);
    FileIO::write(m_height, file);
    FileIO::write(m_nChannels, file);

    FileIO::write(m_textureSize, file);
    assert(m_image != nullptr);
    FileIO::write(m_image, m_textureSize, file);
    FileIO::write(m_nMips, file);
    //    FileIO::write()
}

void Texture::createTextureHandle()
{
    assert(m_image != nullptr);

    bool hasMips = true;
    //    int width = m_width;
    //    int height = m_height;
    bgfx::TextureFormat::Enum texFormat;
    switch (m_nChannels) {
    case 1:
        texFormat = bgfx::TextureFormat::R8;
        break;
    case 3:
        texFormat = bgfx::TextureFormat::RGB8;
        break;
    case 4:
        texFormat = bgfx::TextureFormat::RGBA8;
        break;
    default:
        std::cout << "unknown format" << std::endl;
        exit(1);
    }
    //    m_textureSize = m_texInfo.storageSize * m_texInfo.bitsPerPixel;
    //    const bgfx::Memory * mem = bgfx::makeRef(m_image, imageSize);
    //                    glGenTextures(1, &texture_id);
    //                    glBindTexture(GL_TEXTURE_2D, texture_id);
    //                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //    const uint64_t textureFlags = 0 | BGFX_TEXTURE_NONE;
//    const uint64_t textureFlags = 0 | BGFX_TEXTURE_NONE;
    //            const uint64_t textureFlags = 0 | BGFX_TEXTURE_RT_MSAA_X8;
    //                 const uint64_t textureFlags = 0 | BGFX_CAPS_FORMAT_TEXTURE_MIP_AUTOGEN;
    //                 const uint64_t textureFlags = 0 | BGFX_RESOLVE_AUTO_GEN_MIPS;
    //        const uint64_t textureFlags = 0 | BGFX_RESOLVE_AUTO_GEN_MIPS;
    // BGFX_CAPS_FORMAT_TEXTURE_MIP_AUTOGEN;
    //         const uint64_t textureFlags = 0 | BGFX_TEXTURE_NONE;
    // BGFX_CAPS_FORMAT_TEXTURE_MIP_AUTOGEN
    //    const uint64_t samplerFlags = 0 | BGFX_SAMPLER_NONE;
//    const uint64_t samplerFlags = 0 | BGFX_SAMPLER_NONE;
    //                const uint64_t samplerFlags = 0 | BGFX_SAMPLER_MIN_ANISOTROPIC | BGFX_SAMPLER_MAG_ANISOTROPIC;
    //                const uint64_t samplerFlags = 0 | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT;
    //    / m_fun->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //    m_fun->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //    m_fun->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //    const uint64_t samplerFlags = 0 | BGFX_SAMPLER_MAG_ANISOTROPIC | BGFX_SAMPLER_MIN_ANISOTROPIC;
    //    const uint64_t samplerFlags = 0 | BGFX_SAMPLER_MAG_MASK | BGFX_SAMPLER_MIN_MASK;
    //        const uint64_t samplerFlags = 0 | BGFX_SAMPLER_MAG_SHIFT | BGFX_SAMPLER_MIN_SHIFT;
    //        const uint64_t samplerFlags = 0 | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIP_POINT;
    //    const uint64_t samplerFlags = 0 | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIN_POINT;

    m_textureHandle = bgfx::createTexture2D(
        m_width, m_height, hasMips, 1, texFormat,
        s_textureSamplerFlags, bgfx::copy(m_image, m_textureSize));
}

//u_int64_t Texture::get_textureSamplerFlags()
//{
//    return s_textureFlags | s_samplerFlags;
//}

std::ostream& operator<<(std::ostream& os, const Texture& texture)
{
    //    return os << "'" << texture.m_name << "', width=" << texture.m_texInfo.width << ", height=" << texture.m_texInfo.height << ", nbChannel=" << texture.m_texInfo.bitsPerPixel;
    return os << "'" << texture.m_name << "', width=" << texture.m_width << ", height=" << texture.m_height << ", nbChannel=" << texture.m_nChannels << ", nMips=" << texture.m_nMips;
}

// ----------------------------------------- GETTERS
const bgfx::TextureHandle& Texture::textureHandle() const
{
    return m_textureHandle;
}

std::string Texture::name() const
{
    return m_name;
}

size_t Texture::textureSize() const
{
    //    return m_texInfo.storageSize;
    return m_textureSize;
}
