#pragma once
#include <vector>
#include "GlMacros.h"

struct VertexLayoutElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int getSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:
				return sizeof(float);
			case GL_UNSIGNED_INT:
				return sizeof(unsigned int);
			case GL_UNSIGNED_BYTE:
				return sizeof(unsigned char);
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexLayoutElement> elements;
	unsigned int stride;

public:
	inline VertexBufferLayout()
		: stride(0) {}

	//Simple creator that creates a vertex buffer layout with only 1 element
	template<typename T>
	static VertexBufferLayout simple(unsigned int count)
	{
		VertexBufferLayout layout;
		layout.push<T>(count);
		return layout;
	}

	template<typename T>
	void push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void push<float>(unsigned int count)
	{
		elements.push_back({ GL_FLOAT, count, GL_FALSE });
		stride += VertexLayoutElement::getSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void push<unsigned int>(unsigned int count)
	{
		elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		stride += VertexLayoutElement::getSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void push<unsigned char>(unsigned int count)
	{
		elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		stride += VertexLayoutElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	inline const std::vector<VertexLayoutElement> getElements() const { return elements; }
	inline unsigned int getStride() const { return stride; }
};
