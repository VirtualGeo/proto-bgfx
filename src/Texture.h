#pragma ounce

// #include <bgfx/bgfx.h>
#include "bgfx/bgfx.h"
#include <stb/stb_image.h>
#include <iostream>

struct Texture {

//   public:
	Texture(const std::string & filename);
    ~Texture();

//   private:
	unsigned char *m_image = nullptr;
	bgfx::UniformHandle m_sampler;
	bgfx::TextureHandle m_texture;
};