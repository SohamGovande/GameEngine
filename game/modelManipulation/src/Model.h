#pragma once
#include <vector>
#include <array>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

struct Index
{
	size_t pos;
	size_t data;
	bool deleted;

	Index(size_t pos, size_t data)
		: pos(pos), data(data), deleted(false)
	{}

	bool operator==(const Index& other) const { return pos == other.pos && data == other.data; }
};

struct IndexedModel;

struct Triangle
{
	std::array<Index, 3> v;

	Triangle(const Index& v0, const Index& v1, const Index& v2)
		: v({ v0, v1, v2 }) {}

	glm::vec4 getPlane(const IndexedModel& model);
};

struct VertexData
{
	glm::vec2 uv;
	glm::vec3 normal;
	bool deleted;

	VertexData(const glm::vec2& uv, const glm::vec3& normal)
		: uv(uv), normal(normal), deleted(false)
	{}

	bool operator==(const VertexData& right) const { return normal == right.normal && uv == right.uv; }
	bool operator!=(const VertexData& r) const { return !operator==(r); }
};

class Vertex
{
public:
	glm::vec3 position;
	glm::mat4 quadric;

private:
	VertexData firstData;
	std::vector<VertexData> otherData;
	size_t dataCount;

public:
	Vertex(const glm::vec3& pos, const VertexData& data)
		: position(pos), firstData(data), dataCount(1), quadric(0.0f)
	{
	}
	
	Vertex(const glm::vec3& pos)
		: position(pos), firstData(glm::vec2(0), glm::vec3(0)), dataCount(0), quadric(0.0f)
	{}

	VertexData& data(size_t index)
	{
		return index == 0 ? firstData : otherData[index - 1];
	}

	const VertexData& data(size_t index) const
	{
		return index == 0 ? firstData : otherData[index - 1];
	}

	size_t addData(const VertexData& data)
	{
		if (dataCount == 0)
		{
			firstData = data;
			dataCount++;
			return 0;
		}
		else
		{
			if (firstData == data)
				return 0;

			for (size_t i = 0; i < otherData.size(); i++)
				if (otherData[i] == data)
					return i + 1;
			
			dataCount++;
			otherData.push_back(data);
			return otherData.size();
		}
	}
	
	size_t getDataCount() const 
	{
		return dataCount;
	}
};

struct Mesh
{
	float* positions, *uvs, *normals, *tangents;
	unsigned int* indices;
	unsigned int vCount, iCount;

	Mesh()
		: positions(nullptr), uvs(nullptr), normals(nullptr), indices(nullptr), tangents(nullptr),
		vCount(0), iCount(0) {}

	void free() //Don't use RAII because that causes some problems returning a Mesh from a method
	{
		delete[] positions;
		delete[] uvs;
		delete[] normals;
		delete[] indices;
		delete[] tangents;
	}

	bool hasTangentAttribute() const
	{
		return tangents != nullptr;
	}
};

struct IndexedModel
{
	std::vector<Triangle> triangles;
	std::vector<Vertex> vertices;

	Mesh exportMeshData();
};
