#include "GlMacros.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
	: lastAttribIndex(0)
{
	GlCall(glGenVertexArrays(1, &rendererID));
	GlCall(glBindVertexArray(rendererID));
}

void VertexArray::cleanUp() const
{
	GlCall(glDeleteVertexArrays(1, &rendererID));
}

void VertexArray::bind() const
{
	GlCall(glBindVertexArray(rendererID));
}

void VertexArray::unbind() const
{
	GlCall(glBindVertexArray(0));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	bind();
	vb.bind();
	const std::vector<VertexLayoutElement>& elements = layout.getElements();

	unsigned int offset = 0;
	
	const int prevLastAttribIndex = lastAttribIndex; //Hold this because lastAttribIndex gets modified within the loop

	for (unsigned int i = prevLastAttribIndex; i < (elements.size() + prevLastAttribIndex); i++)
	{
		const VertexLayoutElement& element = elements[i - prevLastAttribIndex];
		GlCall(glEnableVertexAttribArray(i));
		GlCall(glVertexAttribPointer(i, element.count, element.type, element.normalized,
			layout.getStride(), reinterpret_cast<const void*>(offset)));

		offset += element.count * VertexLayoutElement::getSizeOfType(element.type);
		lastAttribIndex = i + 1;
	}
}
