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
    Texture(const std::string& diffuseTexName, const std::string& baseDir);
    ~Texture();

private:
    unsigned char* m_image = nullptr;
    //	bgfx::UniformHandle m_sampler;
    bgfx::TextureHandle m_textureHandle;
};

//using Textures = std::vector<Texture>;
using Textures = std::map<std::string, Texture>;

#endif // TEXTURE_H
