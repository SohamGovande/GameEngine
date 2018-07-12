#pragma once

class VertexBuffer
{
private:
	unsigned int rendererID;

public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(const VertexBuffer& other) = delete;
	VertexBuffer(VertexBuffer&& other);
	~VertexBuffer();

	VertexBuffer& operator=(const VertexBuffer& other) = delete;
	VertexBuffer& operator=(VertexBuffer&& other);

	void bind() const;
	void unbind() const;

private:
	void release();
};