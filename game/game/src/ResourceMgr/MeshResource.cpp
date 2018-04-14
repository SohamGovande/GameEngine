#include "MeshResource.h"
#include "ModelLoader.h"

MeshResource::MeshResource(const std::string& objFile, const std::string& texFile)
	: objFile(objFile),
	texFile(texFile)
{
//	freeFunc = &ModelResource::free;
}

MeshResource::~MeshResource()
{
}	

void MeshResource::generate()
{
	Mesh mesh = Loader::loadObjMeshData(objFile);
	value = new Mesh(mesh);
}

void MeshResource::free(void* _thisPtr)
{
	MeshResource* resource = reinterpret_cast<MeshResource*>(_thisPtr);
	delete[] resource->value->vertices;
	delete[] resource->value->indices;
	delete[] resource->value->normals;
	delete[] resource->value->textures;
	delete resource->value;
}