#pragma once

struct Mesh
{
	float* vertices, *textures, *normals, *tangents;
	unsigned int* indices;
	unsigned int vCount, iCount;

	inline Mesh()
		: vertices(nullptr), textures(nullptr), normals(nullptr), indices(nullptr), tangents(nullptr),
		vCount(0), iCount(0)
	{

	}

	inline void free() //Don't use RAII because that causes some problems returning a Mesh from a method
	{
		delete[] vertices;
		delete[] textures;
		delete[] normals;
		delete[] indices;
		delete[] tangents;
	}

	inline bool hasTangentAttribute() const
	{
		return tangents != nullptr;
	}
};
