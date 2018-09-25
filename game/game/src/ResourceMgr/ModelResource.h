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

public:
	ModelResource(const std::string& modelFile, TextureResource& texture);
	ModelResource(ModelResource&& other);
	ModelResource(const ModelResource& other) = delete;
	~ModelResource();

	template<typename... ConstructorArgs>
	inline void addPropertySetter(ConstructorArgs&&... args) { propertySetters.emplace_back(args...); }

protected:
	void generate() override;
	void releaseMemory() override;
};
