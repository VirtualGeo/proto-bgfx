#include "texture.h"
#include <bx/readerwriter.h>
#include <cassert>

#include "fileIO.h"
#include "system.h"
#include <string>

#include <cstring>
#include <utility.h>

//#define STB_IMAGE_IMPLEMENTATION // question : bimg failed to load stb_image
#include <stb/stb_image.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image_resize.h>

//u_int64_t Texture::s_textureFlags = 0 | BGFX_TEXTURE_NONE;
//u_int64_t Texture::s_samplerFlags = 0 | BGFX_SAMPLER_NONE;
uint64_t Texture::s_textureSamplerFlags = 0 | BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE;
//uint64_t Texture::s_textureSamplerFlags = 0 | BGFX_TEXTURE_NONE | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_ANISOTROPIC;
bool Texture::s_initialized = false;
bool Texture::s_shutdowned = false;

static const uint8_t s_checkerBoardImage[12] = {0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF};
static const uint8_t s_whiteImage[3] = {0xFF, 0xFF, 0xFF};
static const uint8_t s_blackImage[3] = {0x00, 0x00, 0x00};
static const uint8_t s_redImage[3] = {0xFF, 0x00, 0x00};
static const uint8_t s_greenImage[3] = {0x00, 0xFF, 0x00};
static const uint8_t s_blueImage[3] = {0x00, 0x00, 0xFF};
static const uint8_t s_cyanImage[3] = {0x00, 0xFF, 0xFF};
static const uint8_t s_magentaImage[3] = {0xFF, 0x00, 0xFF};
static const uint8_t s_yellowImage[3] = {0xFF, 0xFF, 0x00};
static const uint8_t s_azureImage[3] = {0x00, 0x77, 0xFF};
static const uint8_t s_purpleImage[3] = {0x77, 0x00, 0xFF};
static const uint8_t s_pinkImage[3] = {0xFF, 0x00, 0x77};
static const uint8_t s_orangeImage[3] = {0xFF, 0x77, 0x00};
static const uint8_t s_chartreuseImage[3] = {0x77, 0xFF, 0x00};
static const uint8_t s_springGreenImage[3] = {0x00, 0xFF, 0x77};
Texture Texture::m_sampleTextures[Count] {
    Texture{2, 2, 3, sizeof (s_checkerBoardImage), s_checkerBoardImage, 1},
    Texture{1, 1, 3, sizeof (s_whiteImage), s_whiteImage, 1},
    Texture{1, 1, 3, sizeof (s_blackImage), s_blackImage, 1},
    Texture{1, 1, 3, sizeof (s_redImage), s_redImage, 1},
    Texture{1, 1, 3, sizeof (s_greenImage), s_greenImage, 1},
    Texture{1, 1, 3, sizeof (s_blueImage), s_blueImage, 1},
    Texture{1, 1, 3, sizeof (s_cyanImage), s_cyanImage, 1},
    Texture{1, 1, 3, sizeof (s_magentaImage), s_magentaImage, 1},
    Texture{1, 1, 3, sizeof (s_yellowImage), s_yellowImage, 1},
    Texture{1, 1, 3, sizeof (s_azureImage), s_azureImage, 1},
    Texture{1, 1, 3, sizeof (s_purpleImage), s_purpleImage, 1},
    Texture{1, 1, 3, sizeof (s_pinkImage), s_pinkImage, 1},
    Texture{1, 1, 3, sizeof (s_orangeImage), s_orangeImage, 1},
    Texture{1, 1, 3, sizeof (s_chartreuseImage), s_chartreuseImage, 1},
    Texture{1, 1, 3, sizeof (s_springGreenImage), s_springGreenImage, 1},

};
// Texture::Texture(const char *filename) {
Texture::Texture(const std::string& texName, const std::string& baseDir, Type type)
{
    //                    GLuint texture_id;
    m_name = texName;
    m_baseDir = baseDir;
    m_type = type;

    const std::string absoluteTexName = baseDir + texName;
//    std::cout << absoluteTexName << std::endl;

    //    uint texture_id;
    //    int w, h;
    //    int comp;

    assert(!FileExists(texName));

    if (!FileExists(absoluteTexName)) {
        std::cerr << "    [Texture] Unable to find file: " << absoluteTexName
                  << std::endl;
        exit(1);
    }

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


    m_nMips = bx::floor(bx::log2(bx::max(m_width, m_height)));

    int width = m_width;
    int height = m_height;
    int textureSize = width * height;

    //    }
    for (int i = 0; i < m_nMips; ++i) {
        width = bx::max(1, width >> 1);
        height = bx::max(1, height >> 1);
        textureSize += width * height;
    }
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
        const int futureWidth = bx::max(1, width >> 1);
        const int futureHeight = bx::max(1, height >> 1);

        assert(offset + previousMipSize + futureWidth * futureHeight * m_nChannels < m_textureSize);
        stbir_resize_uint8(&m_image[offset], width, height, 0, &m_image[offset + previousMipSize], futureWidth, futureHeight, 0, m_nChannels);

        offset += previousMipSize;
        width = futureWidth;
        height = futureHeight;
    }
    createTextureHandle();
//    free(m_image); // not possible because scene.save() need image data to save bin model
//    m_image = nullptr;
#ifdef MODEL_LOADER_INFO
    std::cout << "    [Material] Load texture (";
    switch (m_type) {
    case Type::DIFFUSE:
        std::cout << "diffuse";
        break;
    case Type::OPACITY:
        std::cout << "opacity";
        break;
    default:
        assert(false);
    }
    std::cout << "): '" << m_name << "', width=" << m_width << ", height=" << m_height << ", nbChannel=" << m_nChannels << ", textureSize=" << m_textureSize << ", nb channel=" << m_nChannels << std::endl;
//    PRINT_MSG("    [Material] Load texture : '", m_name, "', width=", m_width, ", height=", m_height, ", nbChannel=", m_nChannels, ", textureSize=", m_textureSize, ", bitPerPixel=", m_nChannels);
#endif
}

Texture::Texture(int width, int height, int nChannel, size_t textureSize, const uint8_t *image, int nMip)
    : m_width(width)
    , m_height(height)
    , m_nChannels(nChannel)
    , m_textureSize(textureSize)
    , m_nMips(nMip)
//    , m_image(image)
{
//    m_textureSize = textureSize * m_nChannels;
    //    assert(lenMax > m_textureSize);

    m_image = (uint8_t*)malloc(m_textureSize);
    memcpy(m_image, image, textureSize);
//    m_image = image;

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
    , m_type(texture.m_type)

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

//    bool hasMips = true;
    bool hasMips = m_nMips > 1;

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
    case 2:
        texFormat = bgfx::TextureFormat::RG8;
        break;
    case 4:
        texFormat = bgfx::TextureFormat::RGBA8;
        break;
    default:
        std::cout << "unknown format with " << m_nChannels << " channels" << std::endl;
        exit(1);
    }

    //    const uint64_t samplerFlags = 0 | BGFX_SAMPLER_MAG_ANISOTROPIC | BGFX_SAMPLER_MIN_ANISOTROPIC;
    //    const uint64_t samplerFlags = 0 | BGFX_SAMPLER_MAG_MASK | BGFX_SAMPLER_MIN_MASK;
    //        const uint64_t samplerFlags = 0 | BGFX_SAMPLER_MAG_SHIFT | BGFX_SAMPLER_MIN_SHIFT;
    //        const uint64_t samplerFlags = 0 | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIP_POINT;
    //    const uint64_t samplerFlags = 0 | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIN_POINT;

    m_textureHandle = bgfx::createTexture2D(
        m_width, m_height, hasMips, 1, texFormat,
                s_textureSamplerFlags, bgfx::copy(m_image, m_textureSize));
    assert(bgfx::isValid(m_textureHandle));
    bgfx::frame();
}

void Texture::init()
{
    assert(! s_initialized);
    for (int i =0; i <Sample::Count; ++i) {
        m_sampleTextures[i].createTextureHandle();
    }

    s_initialized = true;
}

void Texture::shutdown()
{
    assert(! s_shutdowned);
    for (int i =0; i <Sample::Count; ++i) {
//        delete m_sampleTextures[i];
        bgfx::destroy(m_sampleTextures[i].textureHandle());
    }

    s_shutdowned = true;
}

bgfx::TextureHandle Texture::getSampleTexture(Texture::Sample sample)
{
    return m_sampleTextures[sample].textureHandle();
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
