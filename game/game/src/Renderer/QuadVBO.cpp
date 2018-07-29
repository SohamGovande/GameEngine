#include "QuadVBO.h"

QuadVBO::QuadVBO()
	: quadVertices({
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	}),
	vbo(&quadVertices[0], sizeof(float) * quadVertices.size()),
	vao(),
	layout()
{
	layout.push<float>(2); //positions
	layout.push<float>(2); //textures
	vao.addBuffer(vbo, layout);
}

QuadVBO::~QuadVBO()
{
}
