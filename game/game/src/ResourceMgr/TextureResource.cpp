#include <iostream>
#include "TextureResource.h"

TextureResource::TextureResource(const std::string& filepath, bool clamp)
	: filepath(filepath), clamp(clamp)
{
}

TextureResource::~TextureResource()
{
}

void TextureResource::generate()
{
	value = new Texture(filepath, clamp);
}
