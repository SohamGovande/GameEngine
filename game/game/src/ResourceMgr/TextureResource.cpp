#include <iostream>
#include "TextureResource.h"

TextureResource::TextureResource(const std::string& filepath)
	: filepath(filepath)
{
}

TextureResource::~TextureResource()
{
}

void TextureResource::generate()
{
	value = new Texture(filepath);
}
