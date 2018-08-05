#include <iostream>
#include <fstream>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "OBJLoader.h"

template<typename Number>
Number ParseNumber(std::string_view view, size_t start, size_t end)
{
	static_assert(false, "Invalid numeric type!");
}

template<>
float ParseNumber<float>(std::string_view view, size_t start, size_t end)
{
	return std::stof(std::string(view.substr(start, end - start)));
}

template<>
int ParseNumber<int>(std::string_view view, size_t start, size_t end)
{
	return std::stoi(std::string(view.substr(start, end - start)));
}

template<>
unsigned int ParseNumber<unsigned int>(std::string_view view, size_t start, size_t end)
{
	return static_cast<unsigned int>(std::stoi(std::string(view.substr(start, end - start))));
}

template<typename Number, unsigned int N>
static std::array<Number, N> ReadNumbers(std::string_view line, char delimeter)
{
	size_t off = 0;
	std::array<Number, N> data {};

	for (unsigned int i = 0; i < N; i++)
	{
		size_t index = line.find(delimeter, off);
		data[i] = ParseNumber<Number>(line, off, index);
		off = index + 1;
	}

	return data;
}

Index ProcessTriangle(
	std::string_view triData,
	IndexedModel& outModel,
	const std::vector<glm::vec3>& positions,
	const std::vector<glm::vec2>& uvs,
	const std::vector<glm::vec3>& normals
)
{
	std::array<unsigned int, 3> data = ReadNumbers<unsigned int, 3>(triData, '/');
	Vertex& vertex = outModel.vertices[data[0] - 1];
	size_t dataIndex = vertex.addData({ uvs[data[1] - 1], normals[data[2] - 1] });
	return Index(data[0] - 1, dataIndex); 
}

IndexedModel LoadOBJ(const std::string& filename)
{
	std::ifstream fs(filename);
	if (!fs.is_open())
	{
		std::cout << "Unable to open file " << filename << std::endl;
	}
	std::string content((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
	
	fs.close();

	size_t lastNewline = 0;

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	IndexedModel indexedModel;
	bool firstFace = true;

	for (size_t i = 0; i < content.size(); i++)
	{
		if (content[i] == '\n')
		{
			std::string_view line(&content[lastNewline + 1], i - lastNewline - 1);

			if (line.find("f") == 0)
			{
				if (firstFace)
				{
					firstFace = false;

					indexedModel.vertices.reserve(positions.size());
					for (const glm::vec3& pos : positions)
						indexedModel.vertices.emplace_back(pos);
				}

				line.remove_prefix(2); //"f "

				size_t firstSpace = line.find(' ');
				size_t secondSpace = line.find(' ', firstSpace + 1);

				indexedModel.triangles.emplace_back(
					ProcessTriangle(line.substr(0, firstSpace), indexedModel, positions, uvs, normals),
					ProcessTriangle(line.substr(firstSpace + 1, secondSpace - firstSpace - 1), indexedModel, positions, uvs, normals),
					ProcessTriangle(line.substr(secondSpace + 1), indexedModel, positions, uvs, normals)
				);
				Triangle& tri = indexedModel.triangles.back();

				//Calculate the quadric error metric
				glm::vec4 p = tri.getPlane(indexedModel);

				glm::mat4 quadric = glm::outerProduct(p, p);

				indexedModel.vertices[tri.v[0].pos].quadric += quadric;
				indexedModel.vertices[tri.v[1].pos].quadric += quadric;
				indexedModel.vertices[tri.v[2].pos].quadric += quadric;
			}
			else if (line.find("v ") == 0)
			{
				line.remove_prefix(2);
				std::array<float, 3> nums = ReadNumbers<float, 3>(line, ' ');
				positions.push_back({ nums[0], nums[1], nums[2] });
			}
			else if (line.find("vn") == 0)
			{
				line.remove_prefix(3);
				std::array<float, 3> nums = ReadNumbers<float, 3>(line, ' ');
				normals.push_back({ nums[0], nums[1], nums[2] });
			}
			else if (line.find("vt") == 0)
			{
				line.remove_prefix(3);
				std::array<float, 2> nums = ReadNumbers<float, 2>(line, ' ');
				uvs.push_back({ nums[0], nums[1] });
			}
			lastNewline = i;
		}
	}

	return indexedModel;
}
