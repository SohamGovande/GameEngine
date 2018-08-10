#pragma once
#include <vector>
#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
private:
	unsigned int rendererID;
	unsigned int lastAttribIndex;

public:
	VertexArray();
	VertexArray(const VertexArray& other) = delete;
	VertexArray(VertexArray&& other);
	~VertexArray();

	VertexArray& operator=(const VertexArray& other) = delete;
	VertexArray& operator=(VertexArray&& other);

	void bind() const;
	void unbind() const;
	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	inline unsigned int getVaoID() const { return rendererID; }
private:
	void release();
};

