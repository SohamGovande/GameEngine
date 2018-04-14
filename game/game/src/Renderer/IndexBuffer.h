#pragma once

class IndexBuffer
{
private:
	unsigned int rendererID;
	unsigned int indicesCount;

public:
	IndexBuffer(const unsigned int* data, unsigned int count);

	void cleanUp() const;
	void bind() const;
	void unbind() const;

	inline unsigned int getCount() const { return indicesCount; }
};