#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/detail/func_geometric.inl>

#include <unordered_map>
#include <functional>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <array>

#include "ModelLoader.h"

struct VertexOtherInfo
{
	glm::vec2 tex;
	glm::vec3 normal;
	glm::vec3 tangent;

	VertexOtherInfo(const glm::vec2& tex, const glm::vec3& normal)
		: tex(tex), normal(normal), tangent(0)
	{}

	inline bool operator==(const VertexOtherInfo& other) const
	{
		return tex == other.tex && normal == other.normal;
	}
};

struct PositionedVData
{
	glm::vec3 pos;

	VertexOtherInfo first;
	std::vector<VertexOtherInfo> additional;

	unsigned int amount;

	PositionedVData(const glm::vec3& pos)
		: pos(pos), first(glm::vec2(0), glm::vec3(0)), amount(0)
	{}

	PositionedVData(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal)
		: pos(pos), first(tex, normal), amount(1)
	{}

	inline const VertexOtherInfo& getVertexOtherInfo(unsigned int index) const
	{
		return index == 0 ? first : additional[index - 1];
	}

	inline VertexOtherInfo& getVertexOtherInfo(unsigned int index)
	{
		return index == 0 ? first : additional[index - 1];
	}

	unsigned int add(const glm::vec2& tex, const glm::vec3& normal)
	{
		for (unsigned int i = 0; i < amount; i++)
		{
			const VertexOtherInfo& ei = getVertexOtherInfo(i);
			if (ei.tex == tex && ei.normal == normal)
				return i;
		}

		if (amount == 0)
			first = VertexOtherInfo(tex, normal);
		else
			additional.emplace_back(tex, normal);
		amount++;
		return amount - 1;
	}

	unsigned int add(const VertexOtherInfo& info)
	{
		for (unsigned int i = 0; i < amount; i++)
			if (getVertexOtherInfo(i) == info)
				return i;

		if (amount == 0)
			first = info;
		else
			additional.push_back(info);
		amount++;
		return amount - 1;
	}
};

struct Index
{
	unsigned int posIndex, extraIndex;

	Index(unsigned int posIndex, unsigned int extraIndex)
		: posIndex(posIndex), extraIndex(extraIndex)
	{}
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
static bool startsWith(const std::string& full, const char* prefix)
{
	for (int i = 0; i < chars; i++)
		if (full[i] != prefix[i])
			return false;

	return true;
}

static Index createVertexData(const std::array<std::string, 3>& tokens,
	const std::vector<glm::vec2>& textures,
	const std::vector<glm::vec3>& normals,
	std::vector<PositionedVData>& outPairs,
	std::vector<Index>& indices)
{
	unsigned int vIndex = std::stoi(tokens[0]) - 1;
	
	const glm::vec2& tex = textures[std::stoi(tokens[1]) - 1];
	const glm::vec3& norm = normals[std::stoi(tokens[2]) - 1];

	indices.emplace_back(
		vIndex,
		outPairs[vIndex].add(tex, norm)
	);
	return indices.back();
}

static Mesh packDataToMesh(const std::vector<PositionedVData>& vdata, const std::vector<Index>& indices, bool includeTangents)
{
	std::vector<unsigned int> perVertexSizeCounter;
	perVertexSizeCounter.reserve(vdata.size());
	unsigned int totalVertexCount = 0;

	for (unsigned int i = 0; i < vdata.size(); i++)
	{
		perVertexSizeCounter.push_back(totalVertexCount);
		totalVertexCount += vdata[i].amount;
	}

	Mesh mesh;
	mesh.vCount = totalVertexCount;
	mesh.vertices = new float[mesh.vCount * 3];
	mesh.normals = new float[mesh.vCount * 3];
	mesh.textures = new float[mesh.vCount * 2];
	if (includeTangents)
		mesh.tangents = new float[mesh.vCount * 3];
	else
		mesh.tangents = nullptr;

	unsigned int vi = 0; //Temp variable we use to add things into the arrays

	for (unsigned int i = 0; i < vdata.size(); i++)
	{
		const PositionedVData& data = vdata[i];
		for (unsigned int j = 0; j < data.amount; j++)
		{
			const VertexOtherInfo& info = data.getVertexOtherInfo(j);

			reinterpret_cast<glm::vec3&>(mesh.vertices[vi * 3]) = data.pos;
			reinterpret_cast<glm::vec2&>(mesh.textures[vi * 2]) = info.tex;
			reinterpret_cast<glm::vec3&>(mesh.normals[vi * 3]) = info.normal;
			if (includeTangents)
				reinterpret_cast<glm::vec3&>(mesh.tangents[vi * 3]) = glm::normalize(info.tangent);
			
			vi++;
		}
	}

	mesh.iCount = indices.size();
	mesh.indices = new unsigned int[mesh.iCount];

	for (unsigned int i = 0; i < indices.size(); i++)
		mesh.indices[i] = perVertexSizeCounter[indices[i].posIndex] + indices[i].extraIndex;
	return mesh;
}

static void calculateTangentVectors(std::vector<PositionedVData>& vdata, const Index& i0, const Index& i1, const Index& i2)
{
	std::array<std::reference_wrapper<PositionedVData>, 3> tri {
		vdata[i0.posIndex],
		vdata[i1.posIndex],
		vdata[i2.posIndex]
	};	
	VertexOtherInfo& o0 = tri[0].get().getVertexOtherInfo(i0.extraIndex);
	VertexOtherInfo& o1 = tri[1].get().getVertexOtherInfo(i1.extraIndex);
	VertexOtherInfo& o2 = tri[2].get().getVertexOtherInfo(i2.extraIndex);

	glm::vec3 deltaPos1 = tri[1].get().pos - tri[0].get().pos;
	glm::vec3 deltaPos2 = tri[2].get().pos - tri[0].get().pos;
	glm::vec2 deltaUv1 = o1.tex - o0.tex;
	glm::vec2 deltaUv2 = o2.tex - o0.tex;

	float r = 1.0f / (deltaUv1.x * deltaUv2.y - deltaUv1.y * deltaUv2.x);
	deltaPos1 *= deltaUv2.y;
	deltaPos2 *= deltaUv1.y;

	glm::vec3 tangent = r * (deltaPos1 - deltaPos2);
	o0.tangent += tangent;
	o1.tangent += tangent;
	o2.tangent += tangent;
}

Mesh Loader::loadObjMeshData(const std::string& filename, bool calculateTangents)
{
	std::ifstream stream("res/models/obj/" + filename + ".obj");
	if (!stream.is_open())
	{
		std::cout << "Unable to load OBJ file " << filename << std::endl;
		return Mesh();
	}
	std::string line;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textures;

	bool hasReadAFaceYet = false;

	std::vector<PositionedVData> vdata;
	std::vector<Index> indices;

	std::unordered_map<unsigned int, unsigned int> vertexRerouting;

	while (getline(stream, line))
	{
		if (line[0] == '#') //Ignore #comments
			continue;

		constexpr int VERTEX = 0, TEXTURE = 1, NORMAL = 2, FACE = 3, SMOOTH_SHADING_SPECIFIER = 4;

		int type = startsWith<2>(line, "v ") ? VERTEX :
			(startsWith<2>(line, "vt") ? TEXTURE :
			(startsWith<2>(line, "vn") ? NORMAL :
				(startsWith<1>(line, "f") ? FACE :
				(startsWith<1>(line, "s") ? SMOOTH_SHADING_SPECIFIER : -1))));

		if (type == VERTEX)
		{
			std::array<std::string, 3> words = optimizedSplit<4, 1>(line, ' ');

			vertices.emplace_back(
				std::stof(words[0]),
				std::stof(words[1]),
				std::stof(words[2])
			);
		}
		else if (type == NORMAL)
		{
			std::array<std::string, 3> words = optimizedSplit<4, 1>(line, ' ');

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
				vdata.reserve(vertices.size());

				for (unsigned int i = 0; i < vertices.size(); i++)
					vdata.emplace_back(vertices[i]);
			}

			std::array<std::string, 3> words = optimizedSplit<4, 1>(line, ' ');

			Index i0 = createVertexData(optimizedSplit<3, 0>(words[0], '/'), textures, normals, vdata, indices);
			Index i1 = createVertexData(optimizedSplit<3, 0>(words[1], '/'), textures, normals, vdata, indices);
			Index i2 = createVertexData(optimizedSplit<3, 0>(words[2], '/'), textures, normals, vdata, indices);
	
			if (calculateTangents)
				calculateTangentVectors(vdata, i0, i1, i2);
		}
		else if (type == SMOOTH_SHADING_SPECIFIER)
		{
			if (line == "s off")
				std::cout << "Warning: Smooth shading disabled in model " << filename << std::endl;
		}
	}

	Mesh mesh = packDataToMesh(vdata, indices, calculateTangents);
	
	return mesh;
}