#pragma once
#include <string>

class Texture
{
private:
	unsigned int rendererID;

	std::string filepath;
	int width, height;

public:
	Texture(const std::string& filepath);
	Texture(unsigned int width, unsigned int height, bool useNullTexture);
	Texture(const Texture& other) = delete;
	Texture(Texture&& other);
	~Texture();

	Texture& operator=(const Texture& right) = delete;
	Texture& operator=(Texture&& other);

	void sendToGL(unsigned int internalFormat, unsigned int format, const void* pixels) const;

	void setMagFilter(int filter) const;
	void setMinFilter(int filter) const;
	void setWrapping(int type) const;
	void setParami(unsigned int pname, int pval) const;
	void setParamf(unsigned int pname, float pval) const;

	void createMipmaps() const;

	void bind(unsigned int slot) const;
	void bind() const;
	void unbind() const;

	void release() const;

	inline int getID() const { return rendererID; }
	inline std::string getFilepath() const { return filepath; }
	inline int getWidth() const { return width; }
	inline int getHeight() const { return height; }
	inline unsigned int getTextureID() const { return rendererID; }
};