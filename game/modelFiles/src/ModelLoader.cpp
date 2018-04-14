#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/detail/func_geometric.inl>

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <array>

#include "ModelLoader.h"

struct VertexData
{
	glm::vec3 position;
	glm::vec2 tex;
	glm::vec3 normal;

	VertexData(glm::vec3 pos, glm::vec2 tex, glm::vec3 normal)
		: position(pos), tex(tex), normal(normal)
	{
	}

	bool operator==(const VertexData& other) const
	{
		return position == other.position && normal == other.normal && tex == other.tex;
	}
};

template<unsigned int splits, unsigned int wordsToSkip, unsigned int wordCount = splits - wordsToSkip>
static std::array<std::string, wordCount> optimizedSplit(const std::string& str, const char splitOn)
{
	std::array<std::string, wordCount> array;
	std::string cache;
	size_t nextIndex = 0;

	for (size_t i = 0; i < str.size(); i++)
	{
		if (nextIndex >= splits)
			break;

		if (str[i] == splitOn)
		{
			if (nextIndex >= wordsToSkip)
				array[nextIndex - wordsToSkip] = cache;

			cache = "";
			nextIndex++;
		}
		else
			cache.push_back(str[i]);
	}
	if (cache.size() > 0 && nextIndex < splits)
		array[nextIndex - wordsToSkip] = std::move(cache);

	return array;
}

template<int chars>
bool startsWith(const std::string& full, const char* prefix)
{
	for (int i = 0; i < chars; i++)
		if (full[i] != prefix[i])
			return false;

	return true;
}

static VertexData createVertexData(const std::array<std::string, 3>& tokens,
	const std::vector<glm::vec3>& vertices,
	const std::vector<glm::vec2>& textures,
	const std::vector<glm::vec3>& normals)
{
	return VertexData(vertices[std::stoi(tokens[0]) - 1],
		textures[std::stoi(tokens[1]) - 1],
		normals[std::stoi(tokens[2]) - 1]);
}

static size_t locateIndex(const VertexData& vertex, std::vector<VertexData>& haystack)
{
	for (size_t i = 0; i < haystack.size(); i++)
		if (haystack[i] == vertex)
			return i;

	//If it doesn't exist, create it
	haystack.push_back(vertex);
	return haystack.size() - 1; //returns last element, the one we just added
}

Mesh Loader::loadObjMeshData(const std::string& filename)
{
	std::ifstream stream(filename);
	if (!stream.is_open())
	{
		std::cout << "Unable to load OBJ file " << filename << std::endl;
		return Mesh();
	}
	std::string line;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textures;
	std::vector<unsigned int> indices;

	bool hasReadAFaceYet = false;
	std::vector<VertexData> orderedData;

	while (getline(stream, line))
	{
		if (line[0] == '#') //Ignore #comments
			continue;

		constexpr int VERTEX = 0, TEXTURE = 1, NORMAL = 2, FACE = 3, SMOOTH_SHADING_SPECIFIER = 4;

		int type = startsWith<2>(line, "v ") ? VERTEX :
			(startsWith<2>(line, "vt") ? TEXTURE :
			(startsWith<2>(line, "vn") ? NORMAL :
				(startsWith<2>(line, "f ") ? FACE :
				(startsWith<2>(line, "s ") ? SMOOTH_SHADING_SPECIFIER : -1))));

		if (type == VERTEX || type == NORMAL)
		{
			std::array<std::string, 3> words = optimizedSplit<4, 1>(line, ' ');

			if (type == VERTEX)
				vertices.emplace_back(
					std::stof(words[0]),
					std::stof(words[1]),
					std::stof(words[2])
				);
			else
				normals.push_back(glm::normalize(glm::vec3(
					std::stof(words[0]),
					std::stof(words[1]),
					std::stof(words[2])
				)));
		}
		else if (type == TEXTURE)
		{
			std::array<std::string, 2> words = optimizedSplit<3, 1>(line, ' ');
			textures.emplace_back(
				std::stof(words[0]),
				std::stof(words[1])
			);
		}
		else if (type == FACE)
		{
			if (!hasReadAFaceYet)
			{
				hasReadAFaceYet = true;
				orderedData.reserve(vertices.size());
			}
			std::array<std::string, 3> words = optimizedSplit<4, 1>(line, ' ');

			VertexData v1 = createVertexData(optimizedSplit<3, 0>(words[0], '/'), vertices, textures, normals);
			VertexData v2 = createVertexData(optimizedSplit<3, 0>(words[1], '/'), vertices, textures, normals);
			VertexData v3 = createVertexData(optimizedSplit<3, 0>(words[2], '/'), vertices, textures, normals);

			indices.reserve(indices.size() + 3);
			indices.push_back(locateIndex(v1, orderedData));
			indices.push_back(locateIndex(v2, orderedData));
			indices.push_back(locateIndex(v3, orderedData));
		}
		else if (type == SMOOTH_SHADING_SPECIFIER)
		{
			if (line == "s off")
				std::cout << "Warning: Smooth shading disabled in model " << filename << std::endl;
		}
	}
	//Reconstruct GL arrays from the vertex data

	Mesh mesh;
	mesh.vCount = orderedData.size();
	mesh.iCount = indices.size();
	mesh.vertices = new float[mesh.vCount * 3];
	mesh.normals = new float[mesh.vCount * 3];
	mesh.textures = new float[mesh.vCount * 2];
	mesh.indices = new unsigned int[mesh.iCount];

	for (size_t i = 0; i < mesh.vCount; i++)
	{
		VertexData data = orderedData[i];
		mesh.vertices[i * 3 + 0] = data.position.x;
		mesh.vertices[i * 3 + 1] = data.position.y;
		mesh.vertices[i * 3 + 2] = data.position.z;

		mesh.normals[i * 3 + 0] = data.normal.x;
		mesh.normals[i * 3 + 1] = data.normal.y;
		mesh.normals[i * 3 + 2] = data.normal.z;

		mesh.textures[i * 2 + 0] = data.tex.x;
		mesh.textures[i * 2 + 1] = data.tex.y;
	}

	for (size_t i = 0; i < mesh.iCount; i++)
		mesh.indices[i] = indices[i];

	return mesh;
}