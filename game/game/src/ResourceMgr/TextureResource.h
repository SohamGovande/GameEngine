#pragma once
#include <string>
#include "Renderer/Texture.h"
#include "Resource.h"
#include "Resource.t.h"

class TextureResource
	: public Resource<Texture>
{
private:
	std::string filepath;
	
protected:
	void generate() override;

public:
	TextureResource(const std::string& filepath);
	~TextureResource();

	inline const std::string& getFilepath() const { return filepath; }
};
