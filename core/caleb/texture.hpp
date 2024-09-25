#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "../ew/external/glad.h"
#include "../ew/external/stb_image.h"
#include <string>

class Texture {
public:
	unsigned int ID;

	Texture() : ID(0) {};

	void TextureJPG(const std::string& path, int filterMode, int wrapMode);
	void TexturePNG(const std::string& path, int filterMode, int wrapMode);
	void Bind(unsigned int slot = 0) const;
};

#endif