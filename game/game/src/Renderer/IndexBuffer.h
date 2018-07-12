#pragma once

class IndexBuffer
{
private:
	unsigned int rendererID;
	unsigned int indicesCount;

public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	IndexBuffer(const IndexBuffer& other) = delete;
	IndexBuffer(IndexBuffer&& other);
	~IndexBuffer();

	IndexBuffer& operator=(const IndexBuffer& other) = delete;
	IndexBuffer& operator=(IndexBuffer&& other);

	void bind() const;
	void unbind() const;

	inline unsigned int getCount() const { return indicesCount; }

private:
	void release();
};