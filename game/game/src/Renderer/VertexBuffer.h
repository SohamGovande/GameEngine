#pragma once

class VertexBuffer
{
private:
	unsigned int rendererID;
public:
	VertexBuffer(const void* data, unsigned int size);
	
	void cleanUp() const;
	void bind() const;
	void unbind() const;
};