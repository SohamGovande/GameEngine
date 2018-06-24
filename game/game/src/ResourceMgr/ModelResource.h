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
	: public Resource<MaterialModel>
{
private:
	std::string modelFile;
	TextureResource& texture;
	std::vector<PropertySetter> propertySetters;

private:
	bool hasNormalMap();

protected:
	void generate() override;

public:
	ModelResource(const std::string& modelFile, TextureResource& texture);
	ModelResource(ModelResource&& other);
	~ModelResource();

	static void free(void* thisPtr);

	template<typename... VaArgs>
	inline void addPropertySetter(VaArgs&&... args)
	{
		propertySetters.emplace_back(args...);
	}
};
