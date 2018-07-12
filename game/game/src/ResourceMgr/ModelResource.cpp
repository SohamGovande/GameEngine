#include "ModelLoader.h"
#include "ModelResource.h"

ModelResource::ModelResource(const std::string& modelFile, TextureResource& texture)
	: modelFile(modelFile), texture(texture)
{
	freeFunc = &ModelResource::freeObject;
}

ModelResource::ModelResource(ModelResource&& other)
	: modelFile(std::move(other.modelFile)), texture(other.texture),
	propertySetters(std::move(other.propertySetters))
{
	freeFunc = other.freeFunc;
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
	mesh = Loader::loadBinaryMeshData(modelFile);
	
	value = new RenderableMaterialModel(
		Loader::loadModelToGL(mesh, hasNormalMap()),
		texture,
		modelFile
	);

	for (const PropertySetter& propertySetter : propertySetters)
		propertySetter.apply(*value);
}

void ModelResource::freeObject(void* thisPtr)
{
	ModelResource* This = reinterpret_cast<ModelResource*>(thisPtr);
	
	This->mesh.free();
	std::cout << "Freed a ModelResource: " << This->modelFile << std::endl;
	delete This->value;
}
