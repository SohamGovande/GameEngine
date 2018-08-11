#include <iostream>
#include <SFML/Graphics/Image.hpp>

#include "GlMacros.h"
#include "Texture.h"

Texture::Texture(const std::string& filepath)
	: rendererID(0), filepath("res/textures/" + filepath + ".png"),
	width(0), height(0)
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

	bind();

	setMagFilter(GL_LINEAR);
	setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
	setWrapping(GL_REPEAT);
	setParamf(GL_TEXTURE_LOD_BIAS, -1.0f);

	GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData.getPixelsPtr()));

	createMipmaps();

	unbind();
}

Texture::Texture(unsigned int width, unsigned int height, bool useNullTexture)
{
	GlCall(glGenTextures(1, &rendererID));
	bind();

	if (useNullTexture)
	{
		GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
	}

	setMinFilter(GL_LINEAR);
	setMagFilter(GL_LINEAR);
}

Texture::Texture(Texture&& other)
	: rendererID(other.rendererID),
	filepath(std::move(other.filepath)),
	width(other.width), height(other.height)
{
	other.rendererID = 0;
	other.width = 0;
	other.height = 0;
}

Texture::~Texture()
{
	release();
}

Texture& Texture::operator=(Texture&& other)
{
	if (this != &other)
	{
		release();
		rendererID = other.rendererID;
		filepath = std::move(other.filepath);
		width = other.width;
		height = other.height;

		other.rendererID = 0;
		other.width = 0;
		other.height = 0;
	}
	return *this;
}

void Texture::sendToGL(unsigned int internalFormat, unsigned int format, const void* pixels) const
{
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
}

void Texture::setMagFilter(int filter) const
{
	setParami(GL_TEXTURE_MIN_FILTER, filter);
}

void Texture::setMinFilter(int filter) const
{
	setParami(GL_TEXTURE_MIN_FILTER, filter);
}

void Texture::setWrapping(int type) const
{
	setParami(GL_TEXTURE_WRAP_S, type);
	setParami(GL_TEXTURE_WRAP_T, type);
}

void Texture::setParami(unsigned int pname, int pval) const
{
	GlCall(glTexParameteri(GL_TEXTURE_2D, pname, pval));
}

void Texture::setParamf(unsigned int pname, float pval) const
{
	GlCall(glTexParameterf(GL_TEXTURE_2D, pname, pval));
}

void Texture::createMipmaps() const
{
	GlCall(glGenerateMipmap(GL_TEXTURE_2D));
}

void Texture::release() const
{
	GlCall(glDeleteTextures(1, &rendererID));
}

void Texture::bind(unsigned int slot) const
{
	GlCall(glActiveTexture(GL_TEXTURE0 + slot));
	GlCall(glBindTexture(GL_TEXTURE_2D, rendererID));
}

void Texture::bind() const
{
	GlCall(glBindTexture(GL_TEXTURE_2D, rendererID));
}

void Texture::unbind() const
{
	GlCall(glBindTexture(GL_TEXTURE_2D, 0));
}
