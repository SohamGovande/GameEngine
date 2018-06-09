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

struct ExtraInfo
{
	glm::vec2 tex;
	glm::vec3 normal;

	ExtraInfo(const glm::vec2& tex, const glm::vec3& normal)
		: tex(tex), normal(normal)
	{}

	inline bool operator==(const ExtraInfo& other) const
	{
		return tex == other.tex && normal == other.normal;
	}
};

struct PositionedVData
{
	glm::vec3 pos;

	ExtraInfo first;
	std::vector<ExtraInfo> additional;

	unsigned int amount;

	PositionedVData(const glm::vec3& pos)
		: pos(pos), first(glm::vec2(0), glm::vec3(0)), amount(0)
	{}

	PositionedVData(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal)
		: pos(pos), first(tex, normal), amount(1)
	{}

	inline const ExtraInfo& getInfo(unsigned int index) const
	{
		return index == 0 ? first : additional[index - 1];
	}


	unsigned int add(const glm::vec2& tex, const glm::vec3& normal)
	{
		for (unsigned int i = 0; i < amount; i++)
		{
			const ExtraInfo& ei = getInfo(i);
			if (ei.tex == tex && ei.normal == normal)
				return i;
		}

		if (amount == 0)
			first = ExtraInfo(tex, normal);
		else
			additional.emplace_back(tex, normal);
		amount++;
		return amount - 1;
	}

	unsigned int add(const ExtraInfo& info)
	{
		for (unsigned int i = 0; i < amount; i++)
			if (getInfo(i) == info)
				return i;

		if (amount == 0)
			first = info;
		else
			additional.push_back(info);
		amount++;
		return amount - 1;
	}
};

struct PositionedVDataUsage
{
	bool firstUsed;
	std::vector<bool> othersUsed;

	PositionedVDataUsage()
		: firstUsed(false)
	{}

	inline bool wasUsed(unsigned int index) const
	{
		return index == 0 ? firstUsed : (((index - 1) < othersUsed.size()) ? othersUsed[index - 1] : false);
	}

	inline void setUsed(unsigned int index, bool used)
	{
		if (index == 0)
		{
			firstUsed = used;
		}
		else
		{
			if (index >= othersUsed.size())
				othersUsed.resize(index, false);

			othersUsed[index - 1] = used;
		}
	}
};

struct Index
{
	unsigned int posIndex, extraIndex;
	bool deleted;

	Index(unsigned int posIndex, unsigned int extraIndex)
		: posIndex(posIndex), extraIndex(extraIndex), deleted(false)
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

static void createVertexData(const std::array<std::string, 3>& tokens,
	const std::unordered_map<unsigned int, unsigned int>& vertexRerouting,
	const std::vector<glm::vec2>& textures,
	const std::vector<glm::vec3>& normals,
	std::vector<PositionedVData>& outPairs,
	std::vector<Index>& indices)
{
	unsigned int vIndex = std::stoi(tokens[0]) - 1;
	std::unordered_map<unsigned int, unsigned int>::const_iterator it = vertexRerouting.find(vIndex);

	while (it != vertexRerouting.end())
	{
		vIndex = it->second;
		it = vertexRerouting.find(vIndex);
	}

	const glm::vec2& tex = textures[std::stoi(tokens[1]) - 1];
	const glm::vec3& norm = normals[std::stoi(tokens[2]) - 1];

	indices.emplace_back(
		vIndex,
		outPairs[vIndex].add(tex, norm)
	);
}

static Mesh packDataToMesh(const std::vector<PositionedVData>& vdata, const std::vector<Index>& indices)
{
	std::vector<PositionedVDataUsage> used(vdata.size());
	for (const Index& index : indices)
		if (!index.deleted)
			used[index.posIndex].setUsed(index.extraIndex, true);

	std::vector<unsigned int> sizeCounter;
	sizeCounter.reserve(vdata.size());
	unsigned int realCount = 0, sizeCounterCount = 0;

	for (unsigned int i = 0; i < vdata.size(); i++)
	{
		sizeCounter.push_back(sizeCounterCount);

		for (unsigned int j = 0; j < vdata[i].amount; j++)
			if (used[i].wasUsed(j))
				realCount++;

		sizeCounterCount += vdata[i].amount;
	}

	Mesh mesh;
	mesh.vCount = realCount;
	mesh.vertices = new float[mesh.vCount * 3];
	mesh.normals = new float[mesh.vCount * 3];
	mesh.textures = new float[mesh.vCount * 2];

	//Temp variable we use to add things into the arrays
	unsigned int vi = 0;

	for (unsigned int i = 0; i < vdata.size(); i++)
	{
		const PositionedVData& data = vdata[i];
		for (unsigned int j = 0; j < data.amount; j++)
		{
			if (!used[i].wasUsed(j))
				continue;

			const ExtraInfo& ei = data.getInfo(j);
			mesh.vertices[vi * 3 + 0] = data.pos.x;
			mesh.vertices[vi * 3 + 1] = data.pos.y;
			mesh.vertices[vi * 3 + 2] = data.pos.z;
			mesh.textures[vi * 2 + 0] = ei.tex.x;
			mesh.textures[vi * 2 + 1] = ei.tex.y;
			mesh.normals[vi * 3 + 0] = ei.normal.x;
			mesh.normals[vi * 3 + 1] = ei.normal.y;
			mesh.normals[vi * 3 + 2] = ei.normal.z;
			vi++;
		}
	}

	std::vector<unsigned int> vertexOffsets;
	vertexOffsets.reserve(realCount);
	unsigned int currentVOffset = 0;

	for (unsigned int i = 0; i < vdata.size(); i++)
	{
		for (unsigned int j = 0; j < vdata[i].amount; j++)
		{
			if (!used[i].wasUsed(j))
				currentVOffset++;
			vertexOffsets.push_back(currentVOffset);
		}
	}

	for (const Index& index : indices)
		if (!index.deleted)
			mesh.iCount++;

	mesh.indices = new unsigned int[mesh.iCount];

	unsigned int nextIndex = 0;
	for (const Index& index : indices)
		if (!index.deleted)
		{
			mesh.indices[nextIndex] = sizeCounter[index.posIndex] + index.extraIndex;
			mesh.indices[nextIndex] -= vertexOffsets[mesh.indices[nextIndex]];
			nextIndex++;
		}
	return mesh;
}

static void collapseEdge(const std::array<unsigned int, 2>& targetEdge, std::vector<PositionedVData>& vdata, std::vector<Index>& indices)
{
	const glm::vec3 avgPos = (vdata[targetEdge[0]].pos + vdata[targetEdge[1]].pos) * 0.5f;
	glm::vec3 avgNorm(0);
	for (unsigned int i = 0; i < vdata[targetEdge[0]].amount; i++) avgNorm += vdata[targetEdge[0]].getInfo(i).normal;
	for (unsigned int i = 0; i < vdata[targetEdge[1]].amount; i++) avgNorm += vdata[targetEdge[1]].getInfo(i).normal;
	avgNorm = glm::normalize(avgNorm);

	const unsigned int newVPosIndex = vdata.size();
	vdata.emplace_back(avgPos);
	PositionedVData& newVertex = vdata.back();

	for (unsigned int i = 0; i < indices.size(); i += 3)
	{
		if (indices[i].deleted/* || indices[i + 1].deleted || indices[i + 2].deleted*/)
			continue;

		const bool matchingPositions[6]{
			indices[i].posIndex == targetEdge[0],
			indices[i + 1].posIndex == targetEdge[0],
			indices[i + 2].posIndex == targetEdge[0],
			indices[i].posIndex == targetEdge[1],
			indices[i + 1].posIndex == targetEdge[1],
			indices[i + 2].posIndex == targetEdge[1]
		};
		unsigned int count = 0;
		for (unsigned int i = 0; i < 6; i++)
			count += matchingPositions[i];

		if (count == 1)
		{
			std::function<void(unsigned int indicesIndex)> moveVertex = [&](unsigned int indicesIndex) 
			{
				ExtraInfo info = vdata[indices[indicesIndex].posIndex].getInfo(indices[indicesIndex].extraIndex);
				info.normal = avgNorm;
				indices[indicesIndex].extraIndex = newVertex.add(info);
				indices[indicesIndex].posIndex = newVPosIndex;
			};

			if (matchingPositions[0] || matchingPositions[3])
			{
				moveVertex(i);
			}
			else if (matchingPositions[1] || matchingPositions[4])
			{
				moveVertex(i + 1);
			}
			else /* if (matchingPositions[2] || matchingPositions[5]) */
			{
				moveVertex(i + 2);
			}
		}
		else if (count == 2)
		{
			indices[i].deleted =
				indices[i + 1].deleted =
				indices[i + 2].deleted = true;
		}
	}
}

static unsigned int findIndicesCount(const std::vector<Index>& indices)
{
	unsigned int size = indices.size();
	for (const Index& index : indices)
		if (index.deleted)
			size--;
	return size;
}

static unsigned int findVerticesCount(const std::vector<PositionedVData>& vdata)
{
	unsigned int size = 0;
	for (const PositionedVData& vdata : vdata)
		size += vdata.amount;
	return size;
}

static void simplifyMesh(unsigned int edgesToReduce, std::vector<PositionedVData>& vdata, std::vector<Index>& indices)
{
	unsigned int edgesReduced = 0;

	while (true)
	{
		if (edgesReduced >= edgesToReduce)
			return;

		const unsigned int origEdgeCount = findIndicesCount(indices);

		std::array<Index, 2> shortestEdgeIndices{
			Index(0, 0),
			Index(0, 0)
		};

		float smallestDist = -1;
		unsigned int targetTriIndex = 0;
		bool foundEdge = false;

		bool firstTri = false;

		for (unsigned int i = 0; i < indices.size(); i += 3)
		{
			if (indices[i].deleted/* || indices[i + 1].deleted || indices[i + 2].deleted*/)
				continue;

			const std::array<glm::vec3, 3> diff{
				vdata[indices[i].posIndex].pos - vdata[indices[i + 1].posIndex].pos,
				vdata[indices[i + 1].posIndex].pos - vdata[indices[i + 2].posIndex].pos,
				vdata[indices[i].posIndex].pos - vdata[indices[i + 2].posIndex].pos
			};

			const std::array<float, 3> dist{
				diff[0].x * diff[0].x + diff[0].y * diff[0].y + diff[0].z * diff[0].z,
				diff[1].x * diff[1].x + diff[1].y * diff[1].y + diff[1].z * diff[1].z,
				diff[2].x * diff[2].x + diff[2].y * diff[2].y + diff[2].z * diff[2].z
			};

			const float triangleSmallestDist = std::min(std::min(dist[0], dist[1]), dist[2]);

			if (triangleSmallestDist < smallestDist || firstTri || smallestDist == -1)
			{
				foundEdge = true;
				firstTri = false;

				//Find the smallest edge
				smallestDist = triangleSmallestDist;
				targetTriIndex = i;

				if (dist[0] < dist[1] && dist[0] < dist[2])
				{
					shortestEdgeIndices[0] = indices[i];
					shortestEdgeIndices[1] = indices[i + 1];
				}
				else if (dist[1] < dist[0] && dist[1] < dist[2])
				{
					shortestEdgeIndices[0] = indices[i + 1];
					shortestEdgeIndices[1] = indices[i + 2];
				}
				else
				{
					shortestEdgeIndices[0] = indices[i];
					shortestEdgeIndices[1] = indices[i + 2];
				}
			}
		}

		collapseEdge({ shortestEdgeIndices[0].posIndex, shortestEdgeIndices[1].posIndex }, vdata, indices);

		const unsigned int edgesRemoved = origEdgeCount - findIndicesCount(indices);
		edgesReduced += edgesRemoved;

		if (!foundEdge)
		{
			std::cout << "No edges found, so breaking" << std::endl;
			break;
		}
	}
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

	bool hasReadAFaceYet = false;

	std::vector<PositionedVData> vdata;
	std::vector<Index> indices;

	unsigned int vertexCounter = 0;
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

			glm::vec3 pos(
				std::stof(words[0]),
				std::stof(words[1]),
				std::stof(words[2])
			);

			bool duplicate = false;
			//	for (unsigned int i = 0; i < vertices.size(); i++)
			//	{
			//		if (vertices[i] == pos)
			//		{
			//			duplicate = true;
			//			vertexRerouting[vertexCounter] = i;
			//			break;
			//		}
			//	}
			if (!duplicate)
				vertices.emplace_back(pos);
			else
				vertices.emplace_back(0, 0, 0);

			vertexCounter++;
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

			createVertexData(optimizedSplit<3, 0>(words[0], '/'), vertexRerouting, textures, normals, vdata, indices);
			createVertexData(optimizedSplit<3, 0>(words[1], '/'), vertexRerouting, textures, normals, vdata, indices);
			createVertexData(optimizedSplit<3, 0>(words[2], '/'), vertexRerouting, textures, normals, vdata, indices);
		}
		else if (type == SMOOTH_SHADING_SPECIFIER)
		{
			if (line == "s off")
				std::cout << "Warning: Smooth shading disabled in model " << filename << std::endl;
		}
	}

	unsigned int reduction;
	std::cout << "  Vertices: " << findVerticesCount(vdata) << "\n"
		<< "  Triangles: " << indices.size() / 3 << "\n"
		<< "  Edges: " << indices.size() << "\n\n"
		<< "Edge Collapse Mesh Simplification\n"
		<< "  Edges to decimate: ";
	std::cin >> reduction;

	Mesh mesh;

	if (reduction != 0)
	{
		const unsigned int verticesCountPrev = findVerticesCount(vdata);
		const unsigned int indicesCountPrev = indices.size();

		simplifyMesh(reduction, vdata, indices);

		mesh = packDataToMesh(vdata, indices);

		const unsigned int indicesCount = mesh.iCount;
		const unsigned int verticesCount = mesh.vCount;

		std::cout << "  Vertices: " << verticesCount << "\n"
			<< "  Triangles: " << indicesCount / 3 << "\n"
			<< "  Edges: " << indicesCount << " (Removed " << indicesCountPrev - indicesCount << ")"
			<< std::endl;
	}
	else mesh = packDataToMesh(vdata, indices);

	return mesh;
}