#include <iostream>
#include <string>
#include <limits>
#include <vector>

#include "Header.h"
#include "ModelLoader.h"
#include "BinIO/BinaryWriter.t.h"
#include "BinIO/BinaryReader.t.h"

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
	
	writer.writeHeader("model", Version(1, 0, 0));
	writer.write<unsigned int>(mesh.vCount);

	writer.writeBlock<float, unsigned int>(mesh.vertices, mesh.vCount * 3);
	writer.writeBlock<float, unsigned int>(mesh.textures, mesh.vCount * 2);
	writer.writeBlock<float, unsigned int>(mesh.normals, mesh.vCount * 3);

	writer.write<bool>(mesh.hasTangentAttribute());
	if (mesh.hasTangentAttribute())
		writer.writeBlock<float, unsigned int>(mesh.tangents, mesh.vCount * 3);

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
				"| OBJ to Binary Format Converter  |\n" << 
				"+---------------------------------+" << std::endl;
	
	std::string modelName;

	std::cout << "OBJ name: ";
	std::cin >> modelName;
	
	while (!modelName.empty())
	{
		Mesh mesh = Loader::loadObjMeshData(RES + "obj/" + modelName + ".obj", true);

		std::cout << "Binary name: ";
		std::string binaryName;
		std::cin >> binaryName;

		if (mesh.vertices != nullptr)
		{
			std::cout << "Saved " << binaryName << ".dat\n";
			convertObjToBinary(mesh, modelName);
		}
		std::cout << std::endl;

		mesh.free();
		
		std::cout << "OBJ name: ";
		std::cin >> modelName;
	}

	char a;
	std::cin >> a;
	return 0;
}