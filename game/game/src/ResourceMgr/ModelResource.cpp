#include "ModelLoader.h"
#include "ModelResource.h"

ModelResource::ModelResource(const std::string& modelFile, TextureResource& texture)
	: modelFile(modelFile), texture(texture)
{
	freeFunc = &ModelResource::free;
}

ModelResource::ModelResource(ModelResource&& other)
	: modelFile(std::move(other.modelFile)), texture(other.texture),
	propertySetters(std::move(other.propertySetters))
{
}

ModelResource::~ModelResource()
{
}

bool ModelResource::hasNormalMap()
{
	for (const PropertySetter& prop : propertySetters)
	{
		if (prop.getTargetProperty().getOffset() == offsetof(MaterialModelProperties, normalMap))
			return true;
	}
	return false;
}

void ModelResource::generate()
{
	value = new MaterialModel(
		texture,
		Loader::loadModelToGL(Loader::loadObjMeshData(modelFile, hasNormalMap())),
		modelFile
	);

	for (const PropertySetter& propertySetter : propertySetters)
		propertySetter.apply(*value);
}

void ModelResource::free(void* thisPtr)
{
	ModelResource* This = reinterpret_cast<ModelResource*>(thisPtr);
	
	delete This->value;
}
