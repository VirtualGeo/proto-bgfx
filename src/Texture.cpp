#include "Texture.h"
#include <cassert>
#include <iostream>
#include <bx/readerwriter.h>

// Texture::Texture(const char *filename) {
Texture::Texture(const std::string &filename) {

	// bgfx::TextureHandle loadTexture(const char *filename) {

	int width;
	int height;
	int nbChannel;
	m_image =
		stbi_load(filename.c_str(), &width, &height, &nbChannel, STBI_rgb);
	// stbi_load(filename, &width, &height, &comp, STBI_rgb);
	size_t imageSize = width * height * nbChannel;

	if (m_image == nullptr) {
		// throw(std::string("Failed to load texture : ") +
		// std::string(filename)); throw(std::string("Failed to load texture
		// :
		// "));
		std::cout << "Failed to load texture '" << filename.c_str() << "'"
				  << std::endl;
		exit(1);
		// throw std::runtime_error("Failed to load texture");
	}
	// assert(nbChannel == 3);
	// stbi_image_free(image);
	// image = nullptr;
	// return bgfx::createTexture2D();
	bool hasMips = false;
	// bool hasMips = true; imageSize = 65535;
	// const bgfx::Memory *texPtr = bgfx::alloc(imageSize);
	// stippleTex = bgfx::alloc(imageSize);
	// bx::memSet(stippleTex->data, 0, stippleTex->size);
	// texPtr->data = (uint8_t*)std::move(image);
	// const uint64_t textureFlags = 0 | BGFX_CAPS_FORMAT_TEXTURE_MIP_AUTOGEN;
	const uint64_t textureFlags = 0 | BGFX_TEXTURE_NONE;
	// BGFX_CAPS_FORMAT_TEXTURE_MIP_AUTOGEN;
	// const uint64_t textureFlags = 0 | BGFX_TEXTURE_NONE;
	const uint64_t samplerFlags = 0 | BGFX_SAMPLER_NONE;
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
	m_sampler = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
	// const bgfx::Memory * mem = bgfx::copy(m_image, imageSize * 8);
	// const bgfx::Memory * mem = bgfx::alloc(imageSize);
	// bx::memCopy(mem, image, imageSize);
	// bgfx::copy(m_image, )
	// bgfx::blit()

	// bgfx::alloc()
	// stbi_image_free(m_image);
	// m_image = nullptr;

	if (nbChannel == 3) {
		m_texture = bgfx::createTexture2D(
			width, height, hasMips, 1, bgfx::TextureFormat::RGB8,
			// BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIN_POINT, stippleTex);
			textureFlags | samplerFlags, bgfx::makeRef(m_image, imageSize));
			// textureFlags | samplerFlags, mem);

	} else if (nbChannel == 1) {
		m_texture = bgfx::createTexture2D(
			width, height, hasMips, 1, bgfx::TextureFormat::R8,
			textureFlags | samplerFlags, bgfx::makeRef(m_image, imageSize));
	} else if (nbChannel == 4) {
		m_texture = bgfx::createTexture2D(
			width, height, hasMips, 1, bgfx::TextureFormat::RGBA8,
			textureFlags | samplerFlags, bgfx::makeRef(m_image, imageSize));
	}
    else {
        std::cout << "unable to load texture with " << nbChannel << " channels" << std::endl;
        exit(1);
    }

	// stbi_image_free(image);
	// image = nullptr;
	// }
}

Texture::~Texture() {
	stbi_image_free(m_image);
	m_image = nullptr;
	bgfx::destroy(m_sampler);
	bgfx::destroy(m_texture);
}