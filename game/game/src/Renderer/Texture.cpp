#include <iostream>
#include <SFML/Graphics/Image.hpp>

#include "GlMacros.h"
#include "Texture.h"

Texture::Texture(std::string filepath, bool clamp)
	: rendererID(0), filepath("res/textures/" + filepath + ".png"), localBuf(nullptr),
	width(0), height(0), bitsPerPixel(0)
{
	sf::Image imgData;
	if (!imgData.loadFromFile(this->filepath))
	{
		std::cout << "Unable to load texture from file. File doesn't exist: " << filepath << std::endl;
		return;
	}
	imgData.flipVertically();

	sf::Vector2u size = imgData.getSize();
	width = size.x;
	height = size.y;

	GlCall(glGenTextures(1, &rendererID));
	GlCall(glBindTexture(GL_TEXTURE_2D, rendererID));	

	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	int wrapFunc = clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT;
	
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapFunc));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapFunc));

	GlCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1.0f));

	GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData.getPixelsPtr()));

	GlCall(glGenerateMipmap(GL_TEXTURE_2D));

	GlCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::cleanUp() const
{
	GlCall(glDeleteTextures(1, &rendererID));
}

void Texture::bind(unsigned int slot) const
{
	GlCall(glActiveTexture(GL_TEXTURE0 + slot));
	GlCall(glBindTexture(GL_TEXTURE_2D, rendererID));
}

void Texture::unbind() const
{
	GlCall(glBindTexture(GL_TEXTURE_2D, 0));
}
