#include "ModelLoader.h"
#include "ModelResource.h"

ModelResource::ModelResource(const std::string& modelFile, const std::string& texFile)
	: modelFile(modelFile), texFile(texFile)
{
	freeFunc = &ModelResource::free;
}

ModelResource::~ModelResource()
{
}

void ModelResource::generate()
{

	value = new MaterialModel(
		new TextureResource(texFile),
		Loader::loadModel((0) ?
			Loader::loadObjMeshData(modelFile)
			: Loader::loadBinaryMeshData(modelFile)),
		modelFile
	);
	
}

void ModelResource::free(void* thisPtr)
{
	ModelResource* This = reinterpret_cast<ModelResource*>(thisPtr);
	
	delete &This->value->getTexture();
	delete This->value;
}
