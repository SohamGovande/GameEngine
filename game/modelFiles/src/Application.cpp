#include <iostream>
#include <string>
#include <limits>
#include <vector>

#include "ModelLoader.h"
#include "BinIO/BinaryWriter.t.h"
#include "BinIO/BinaryReader.t.h"

template<typename T, unsigned char type>
static bool WriteIndices(BinaryWriter& writer, const Mesh& mesh)
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

static void ConvertObjToBinary(const Mesh& mesh, const std::string& filename)
{
	BinaryWriter writer(filename + ".dat");
	
	writer.writeHeader("model", Version(1, 0, 0));
	writer.write<unsigned int>(mesh.vCount);

	writer.writeBlock<float, unsigned int>(mesh.vertices, mesh.vCount * 3);
	writer.writeBlock<float, unsigned int>(mesh.textures, mesh.vCount * 2);
	writer.writeBlock<float, unsigned int>(mesh.normals, mesh.vCount * 3);

	writer.write<bool>(mesh.hasTangentAttribute());
	if (mesh.hasTangentAttribute())
		writer.writeBlock<float, unsigned int>(mesh.tangents, mesh.vCount * 3);

	writer.write<unsigned int>(mesh.iCount);
	
	//Choose the smallest data type that works for the model's indices:
	//uchar, ushort, then uint
	if (!WriteIndices<unsigned char, 0>(writer, mesh))
		if (!WriteIndices<unsigned short, 1>(writer, mesh))
			WriteIndices<unsigned int, 2>(writer, mesh);

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
	
	while (!modelName.empty()) //Whether the user entered anything in
	{
		Mesh mesh = Loader::loadObjMeshData(modelName + ".obj", true);

		std::cout << "Binary name: ";
		std::string binaryName;
		std::cin >> binaryName;

		if (mesh.vertices != nullptr)
		{
			std::cout << "Saved " << binaryName << ".dat\n";
			ConvertObjToBinary(mesh, modelName);
		}
		std::cout << std::endl;

		mesh.free();
		
		//Continue with the next model
		std::cout << "OBJ name: ";
		std::cin >> modelName;
	}

	char a;
	std::cin >> a;
	return 0;
}