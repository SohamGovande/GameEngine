#pragma once
class Renderbuffer
{
private:
	unsigned int rendererID;

public:
	Renderbuffer();
	Renderbuffer(const Renderbuffer& other) = delete;
	Renderbuffer(Renderbuffer&& other);
	~Renderbuffer();

	Renderbuffer& operator=(const Renderbuffer& other) = delete;
	Renderbuffer& operator=(Renderbuffer&& other);

	void release() const;
	void bind() const;
	void unbind() const;
	void setStorage(unsigned int storage, unsigned int width, unsigned int height) const;

	inline unsigned int getRenderbufferID() const { return rendererID; }
};
