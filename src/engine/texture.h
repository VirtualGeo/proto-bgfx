//#pragma ounce
#ifndef TEXTURE_H
#define TEXTURE_H

// #include <bgfx/bgfx.h>
#include "bgfx/bgfx.h"
#include <iostream>
#include <map>
//#include <stb/stb_image.h>
#include <vector>

#include <bimg/bimg.h>

// uint64_t;

class Texture {
public:
    enum Sample {
        CHECKER_BOARD = 0,
        RED,
        GREEN,
        BLUE,
        Count
    };

    static uint64_t s_textureSamplerFlags;

public:
    Texture(const std::string& texName, const std::string& baseDir);
    Texture(int width, int height, int nChannel, size_t textureSize, const uint8_t * image, int nMip);
    Texture(Texture&&);
    //    Texture(Texture &&) noexcept = default;
    Texture(const Texture&) = delete; // delete copy constructor
    ~Texture();

    Texture(std::ifstream& file);
    void save(std::ofstream& file) const;

    void createTextureHandle();
    //    void setSamplerAnisotropic();
    //    void setSamplerPoint();
    static void init();
    static void clear();
    static Texture m_sampleTextures[Count];

    friend std::ostream& operator<<(std::ostream& os, const Texture& texture);

private:
    //	bgfx::UniformHandle m_sampler;
    bgfx::TextureHandle m_textureHandle;
    std::string m_name;
    std::string m_baseDir;

    int m_width;
    int m_height;
    int m_nChannels;

    size_t m_textureSize; // bytes
    uint8_t* m_image = nullptr;

    int m_nMips;

    //    bgfx::TextureInfo m_texInfo;
    //    bimg::Orientation::Enum m_orientation;
    bool m_last = true;
    //        static u_int64_t s_textureFlags;
    //        static u_int64_t s_samplerFlags;

public: // ---------------------------------------------- getters
    const bgfx::TextureHandle& textureHandle() const;
    std::string name() const;
    size_t textureSize() const;

    //    static u_int64_t get_textureSamplerFlags();
};

// u_int64_t Texture::s_textureSamplerFlags = 0 | BGFX_TEXTURE_NONE | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_ANISOTROPIC;

using Textures = std::vector<Texture>;
//using Textures = std::map<std::string, Texture>;

#endif // TEXTURE_H
