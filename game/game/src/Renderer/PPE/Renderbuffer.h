#pragma once
class Renderbuffer
{
private:
	unsigned int rendererID;

public:
	Renderbuffer();
	
	void cleanUp() const;
	void bind() const;
	void unbind() const;
	void setStorage(unsigned int storage, unsigned int width, unsigned int height) const;

	inline unsigned int getRenderbufferID() const { return rendererID; }
};
