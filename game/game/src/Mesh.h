#pragma once

struct Mesh
{
	float* vertices, *textures, *normals, *tangents;
	unsigned int* indices;
	unsigned int vCount, iCount;

	Mesh();
	Mesh(const Mesh& other) = delete;
	Mesh(Mesh&& other);
	~Mesh();
	
	Mesh& operator=(const Mesh& other) = delete;
	Mesh& operator=(Mesh&& other);

	void release();

	inline bool hasTangentAttribute() const { return tangents != nullptr; }
};
