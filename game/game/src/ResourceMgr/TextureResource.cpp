#include <iostream>
#include "TextureResource.h"

TextureResource::TextureResource(const std::string& filepath)
	: filepath(filepath)
{
}

void TextureResource::generate()
{
	value = new Texture(filepath);
}
