#pragma once
#include <string>

class Texture
{
private:
	unsigned int rendererID;

	std::string filepath;
	unsigned char* localBuf;
	int width, height;

public:
	Texture(const std::string& filepath);
	Texture(unsigned int width, unsigned int height, bool useNullTexture);
	
	void cleanUp() const;
	void bind(unsigned int slot = 0U) const;
	void unbind() const;

	inline int getID() const { return rendererID; }
	inline std::string getFilepath() const { return filepath; }
	inline int getWidth() const { return width; }
	inline int getHeight() const { return height; }
	inline unsigned int getTextureID() const { return rendererID; }
};