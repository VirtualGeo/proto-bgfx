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

class Texture {

public:
    Texture(const std::string& texName, const std::string& baseDir);
    Texture(Texture&&);
    //    Texture(Texture &&) noexcept = default;
    Texture(const Texture&) = delete; // delete copy constructor
    ~Texture();

    Texture(std::ifstream& file);
    void save(std::ofstream& file) const;

    void createTextureHandle();

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

public: // ---------------------------------------------- getters
    const bgfx::TextureHandle& textureHandle() const;
    std::string name() const;
    size_t textureSize() const;
};

using Textures = std::vector<Texture>;
//using Textures = std::map<std::string, Texture>;

#endif // TEXTURE_H
