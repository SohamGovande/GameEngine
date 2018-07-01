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

	inline void free()
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
