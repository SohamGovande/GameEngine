#include <iostream>
#include <string>
#include <limits>
#include <vector>

#include "ModelLoader.h"
#include "BinIO/BinaryWriter.h"
#include "BinIO/BinaryReader.h"

#define RES std::string("X:\\dev\\cpp\\game\\game\\game\\res\\models\\")

template<typename T, unsigned char type>
bool writeIndices(BinaryWriter& writer, const Mesh& mesh)
{
	if (mesh.iCount <= std::numeric_limits<T>::max())
	{
		writer.write<unsigned char>(type);
		for (unsigned int i = 0; i < mesh.iCount; i++)
			writer.write<T>(static_cast<T>(mesh.indices[i]));

		return true;
	}
	return false;
}

void convertObjToBinary(const Mesh& mesh, const std::string& filename)
{
	BinaryWriter writer(RES + filename + ".dat");
	
	writer.write<unsigned int>(mesh.vCount);

	for (unsigned int i = 0; i < mesh.vCount * 3; i++)
		writer.write<float>(mesh.vertices[i]);

	for (unsigned int i = 0; i < mesh.vCount * 2; i++)
		writer.write<float>(mesh.textures[i]);

	for (unsigned int i = 0; i < mesh.vCount * 3; i++)
		writer.write<float>(mesh.normals[i]);

	writer.write<unsigned int>(mesh.iCount);

	if (!writeIndices<unsigned char, 0>(writer, mesh))
		if (!writeIndices<unsigned short, 1>(writer, mesh))
			writeIndices<unsigned int, 2>(writer, mesh);

	writer.close();

}

int main()
{
	std::cout << 
				"+---------------------------------+\n" <<
				"| Modeling Command-Line Interface |\n" << 
				"+---------------------------------+" << std::endl;
	
	std::cout << "Model name: ";
	std::string modelName;
	std::cin >> modelName;

	Mesh mesh = Loader::loadObjMeshData(RES + modelName + ".obj");
	convertObjToBinary(mesh, modelName);

	mesh.free();
	
	char a;
	std::cin >> a;

	return 0;
}