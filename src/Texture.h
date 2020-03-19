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
    ~Texture();

    const bgfx::TextureHandle & textureHandle() const;

    std::string name() const;

    Texture(const Texture &) = delete; // delete copy constructor
//    Texture(Texture &&) noexcept = default;
    Texture(Texture &&);

private:
    unsigned char* m_image = nullptr;
    //	bgfx::UniformHandle m_sampler;
    bgfx::TextureHandle m_textureHandle;
    std::string m_name;
    std::string m_baseDir;
};

using Textures = std::vector<Texture>;
//using Textures = std::map<std::string, Texture>;

#endif // TEXTURE_H
