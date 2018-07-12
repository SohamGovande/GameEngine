#pragma once

#include <unordered_map>
#include <vector>
#include "Renderer/MaterialModel.h"
#include "Resource.h"
#include "Resource.t.h"
#include "TextureResource.h"
#include "MaterialProperty.h"
#include "PropertySetter.h"

class ModelResource
	: public Resource<RenderableMaterialModel>
{
private:
	std::string modelFile;
	TextureResource& texture;
	std::vector<PropertySetter> propertySetters;
	Mesh mesh;

private:
	bool hasNormalMap();

protected:
	void generate() override;

public:
	ModelResource(const std::string& modelFile, TextureResource& texture);
	ModelResource(ModelResource&& other);
	~ModelResource();

	static void freeObject(void* thisPtr);

	template<typename... ConstructorArgs>
	inline void addPropertySetter(ConstructorArgs&&... args)
	{
		propertySetters.emplace_back(args...);
	}
};
