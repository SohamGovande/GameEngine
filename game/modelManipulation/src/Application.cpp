#include <iostream>
#include "OBJLoader.h"
#include "Simplifier.h"
#include "BinIO/BinaryWriter.t.h"

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
	BinaryWriter writer(filename);

	if (!writer.getWriter().is_open())
		__debugbreak();

	writer.writeHeader("model", Version(1, 0, 0));
	writer.write<unsigned int>(mesh.vCount);

	writer.writeBlock<float, unsigned int>(mesh.positions, mesh.vCount * 3);
	writer.writeBlock<float, unsigned int>(mesh.uvs, mesh.vCount * 2);
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
	IndexedModel model = LoadOBJ(R"(X:\dev\cpp\game\game\game\res\models\obj\axe.obj)");
	SimplifyMesh(model);

	Mesh exported = model.exportMeshData();
	ConvertObjToBinary(exported, R"(X:\dev\cpp\game\game\game\res\models\axe.dat)");

	std::cin.get();

	return 0;
}