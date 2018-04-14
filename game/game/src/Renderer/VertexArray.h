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

	void cleanUp() const;
	void bind() const;
	void unbind() const;
	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

};

