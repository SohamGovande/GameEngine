#include "Mesh.h"

Mesh::Mesh()
	: vertices(nullptr), textures(nullptr), normals(nullptr), tangents(nullptr), indices(nullptr),
	vCount(0), iCount(0)
{
}

Mesh::Mesh(Mesh&& other)
	: vertices(other.vertices), textures(other.textures), normals(other.normals),
	tangents(other.tangents), indices(other.indices),
	vCount(other.vCount), iCount(other.iCount)
{
	other.vertices = other.textures = other.normals = other.tangents = nullptr;
	other.indices = nullptr;
}

Mesh::~Mesh()
{
	release();
}

Mesh& Mesh::operator=(Mesh&& other)
{''
	if (this != &other)
	{
		release();

		vertices = other.vertices;
		textures = other.textures;
		normals = other.normals;
		tangents = other.tangents;
		indices = other.indices;
		vCount = other.vCount;
		iCount = other.iCount;
		
		other.vertices = other.textures = other.normals = other.tangents = nullptr;
		other.indices = nullptr;
	}
	return *this;
}

void Mesh::release()
{
	delete[] vertices;
	delete[] textures;
	delete[] normals;
	delete[] tangents;
	delete[] indices;
	vertices = textures = normals = tangents = nullptr;
	indices = nullptr;
}
