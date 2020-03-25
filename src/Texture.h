//#pragma ounce
#ifndef TEXTURE_H
#define TEXTURE_H

// #include <bgfx/bgfx.h>
#include "bgfx/bgfx.h"
#include <iostream>
#include <map>
#include <stb/stb_image.h>
#include <vector>

class Texture {

public:
    Texture(const std::string& texName, const std::string& baseDir);
    Texture(Texture &&);
//    Texture(Texture &&) noexcept = default;
    Texture(const Texture &) = delete; // delete copy constructor
    ~Texture();

//    Texture(std::ifstream & file);
    void save(std::ofstream & file);

    // -------------------- getters
    const bgfx::TextureHandle & textureHandle() const;
    std::string name() const;
    size_t textureSize() const;

    friend std::ostream & operator<<(std::ostream & os, const Texture & texture);

private:
    unsigned char* m_image = nullptr;
    //	bgfx::UniformHandle m_sampler;
    bgfx::TextureHandle m_textureHandle;
    std::string m_name;
    std::string m_baseDir;

    int m_width;
    int m_height;
    int m_nbChannel;
    size_t m_textureSize; // bytes
};

using Textures = std::vector<Texture>;
//using Textures = std::map<std::string, Texture>;

#endif // TEXTURE_H
