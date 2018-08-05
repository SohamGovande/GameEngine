#include "Model.h"

Mesh IndexedModel::exportMeshData()
{
	Mesh data;
	
	std::vector<size_t> incrementalSizes;
	size_t sizeCounter = 0;

	incrementalSizes.reserve(vertices.size());
	for (const Vertex& vert : vertices)
	{
		incrementalSizes.push_back(sizeCounter);
		sizeCounter += vert.getDataCount();
	}

	//sizeCounter now holds the total vertex count
	data.vCount = sizeCounter;
	data.positions = new float[data.vCount * 3];
	data.uvs = new float[data.vCount * 2];
	data.normals = new float[data.vCount * 3];
	data.tangents = nullptr;

	size_t counter = 0;
	for (const Vertex& vert : vertices)
	{
		for (size_t i = 0; i < vert.getDataCount(); i++)
		{
			const VertexData& vdata = vert.data(i);
			if (vdata.deleted)
				continue;

			reinterpret_cast<glm::vec3&>(data.positions[counter * 3]) = vert.position;
			reinterpret_cast<glm::vec2&>(data.uvs[counter * 2]) = vdata.uv;
			reinterpret_cast<glm::vec3&>(data.normals[counter * 3]) = vdata.normal;
			counter++;
		}
	}

	std::vector<size_t> incrementalDeletedVCount;
	size_t deletedVCounter = 0;
	for (const Vertex& vert : vertices)
	{
		for (size_t i = 0; i < vert.getDataCount(); i++)
		{
			incrementalDeletedVCount.push_back(deletedVCounter);
			if (vert.data(i).deleted)
				deletedVCounter++;
		}
	}

	data.iCount = triangles.size() * 3;
	data.indices = new unsigned int[data.iCount];

	counter = 0;

	for (const Triangle& tri : triangles)
	{
		for (const Index& idx : tri.v)
		{
			if (idx.deleted)
				continue;

			unsigned int& out = data.indices[counter];
			out = incrementalSizes[idx.pos] + idx.data;
			out -= incrementalDeletedVCount[out];

			counter++;
		}
	}

	return data;
}

glm::vec4 Triangle::getPlane(const IndexedModel& model)
{
	const glm::vec3& v0 = model.vertices[v[0].pos].position;
	const glm::vec3& v1 = model.vertices[v[1].pos].position;
	const glm::vec3& v2 = model.vertices[v[2].pos].position;
	glm::vec3 normal = glm::normalize(glm::cross(v2 - v0, v1 - v0));
	float d = -glm::dot(v0, normal);
	return glm::vec4(normal, d);
}
