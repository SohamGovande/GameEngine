#include <iostream>
#include <string>
#include <limits>
#include <vector>

#include "ModelLoader.h"
#include "BinIO/BinaryWriter.h"
#include "BinIO/BinaryReader.h"

#define RES std::string("X:\\dev\\cpp\\game\\game\\game\\res\\models\\")

struct IndicesRange
{
	unsigned int start, end;
};

struct PackedSingles
{
	unsigned int startIndex, endIndex;
	bool pointsToRange;
};

template<typename T>
bool writeIndices(BinaryWriter& writer, const Mesh& mesh, unsigned char type)
{
	if (mesh.iCount <= std::numeric_limits<T>::max())
	{
		writer.write<unsigned char>(type);

		std::vector<IndicesRange> sequences;

		for (unsigned int i = 0; i < mesh.iCount; i++)
		{
			unsigned int index = mesh.indices[i];
			if (sequences.size() == 0)
				sequences.push_back({ index, index });
			else
				if (index == sequences.back().end + 1)
					sequences.back().end++;
				else
					sequences.push_back({ index, index });
		}

		std::vector<PackedSingles> packedData;

		for (unsigned int i = 0; i < sequences.size(); i++)
		{
			const IndicesRange& range = sequences[i];
			if (range.start == range.end)
			{
				if (packedData.size() == 0 || packedData.back().pointsToRange)
					packedData.push_back({ i, i, false });

				else
					packedData.back().endIndex++;
			}
			else
			{
				packedData.push_back({ i, i, true });
			}
		}

		writer.write<T>(static_cast<T>(packedData.size()));

		std::cout << std::hex << writer.getWritePos() << std::endl;

		for (const PackedSingles& pack : packedData)
		{
			if (!pack.pointsToRange)
			{
				writer.write<unsigned char>(0x0);
				writer.write<T>(static_cast<T>(pack.endIndex - pack.startIndex + 1));

				for (unsigned int i = pack.startIndex; i <= pack.endIndex; i++)
					writer.write<T>(static_cast<T>(sequences[i].start));
			}
			else
			{
				writer.write<unsigned char>(0x1);
				writer.write<T>(static_cast<T>(sequences[pack.startIndex].start));
				writer.write<T>(static_cast<T>(sequences[pack.startIndex].end));
			}
		}
		
		return true;
	}
	return false;
}

template<typename T>
void readIndices(BinaryReader& reader, Mesh& mesh)
{
	T packsCount = reader.read<T>();

	mesh.indices = new unsigned int[mesh.iCount];
	unsigned int nextIndex = 0;

	for (unsigned int i = 0; i < packsCount; i++)
	{
		unsigned char packType = reader.read<unsigned char>();
 		if (packType == 0x0)
		{
			T count = reader.read<T>();
			for (unsigned int i = 0; i < count; i++)
				mesh.indices[nextIndex++] = reader.read<T>();
		}
		else if (packType == 0x1)
		{
			T begin = reader.read<T>();
			T end = reader.read<T>();
			for (unsigned int j = begin; j <= end; j++)
				mesh.indices[nextIndex++] = j;
		}
		else __debugbreak();
	}
	
}

void convertObjToBinary(const std::string& filename)
{
	Mesh mesh = Loader::loadObjMeshData(RES + filename + ".obj");

	BinaryWriter writer(RES + filename + ".bin");
	
	writer.write<unsigned int>(mesh.vCount);

	for (unsigned int i = 0; i < mesh.vCount * 3; i++)
		writer.write<float>(mesh.vertices[i]);

	for (unsigned int i = 0; i < mesh.vCount * 2; i++)
		writer.write<float>(mesh.textures[i]);

	for (unsigned int i = 0; i < mesh.vCount * 3; i++)
		writer.write<float>(mesh.normals[i]);

	writer.write<unsigned int>(mesh.iCount);

	if (!writeIndices<unsigned char>(writer, mesh, 0))
		if (!writeIndices<unsigned short>(writer, mesh, 1))
			writeIndices<unsigned int>(writer, mesh, 2);

	writer.close();

	delete[] mesh.vertices;
	delete[] mesh.indices;
	delete[] mesh.normals;
	delete[] mesh.textures;
}

Mesh readBinary(const std::string& filename)
{
	Mesh mesh;

	BinaryReader reader(filename + ".bin");
	mesh.vCount = reader.read<unsigned int>();
	
	mesh.vertices = new float[mesh.vCount * 3];
	mesh.textures = new float[mesh.vCount * 2];
	mesh.normals = new float[mesh.vCount * 3];

	for (unsigned int i = 0; i < mesh.vCount * 3; i++)
		mesh.vertices[i] = reader.read<float>();

	for (unsigned int i = 0; i < mesh.vCount * 2; i++)
		mesh.textures[i] = reader.read<float>();

	for (unsigned int i = 0; i < mesh.vCount * 3; i++)
		mesh.normals[i] = reader.read<float>();
	
	mesh.iCount = reader.read<unsigned int>();

	unsigned char type = reader.read<unsigned char>();
	switch (type)
	{
	case 0:
		readIndices<unsigned char>(reader, mesh);
		break;
	case 1:
		readIndices<unsigned short>(reader, mesh);
		break;
	case 2:
		readIndices<unsigned int>(reader, mesh);
		break;
	}

	reader.close();

	return mesh;
}

int main()
{
	while (true)
	{
		std::cout << "Enter a file to be converted: ";
		std::string line;
		std::getline(std::cin, line);

		if (line.empty())
			break;

		convertObjToBinary(line);
	}
	return 0;
}