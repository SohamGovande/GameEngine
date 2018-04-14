#include "ModelLoader.h"
#include "ModelResource.h"

ModelResource::ModelResource(const std::string& objFile, const std::string& texFile)
	: objFile(objFile), texFile(texFile)
{
	freeFunc = &ModelResource::free;
}

ModelResource::~ModelResource()
{
}

void ModelResource::generate()
{

	value = new MaterialModel(
		new TextureResource(texFile, false),
		Loader::loadModel(Loader::loadBinaryMeshData(objFile)),
		objFile
	);
	
}

void ModelResource::free(void* thisPtr)
{
	ModelResource* This = reinterpret_cast<ModelResource*>(thisPtr);
	
	delete &This->value->getTexture();
	delete This->value;
}
