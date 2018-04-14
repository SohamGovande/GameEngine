#pragma once
#include <vector>
#include <memory>
#include <glm/vec3.hpp>

#include "ResourceMgr/TextureResource.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Mesh.h"

class MaterialModel
{
private:
	TextureResource *texture, *specularMap;
	GlModel model;
	
	std::string objName;

	bool hasSpecularMap;

public:
	bool fullyRender;
	float shineDistanceDamper, reflectivity;

public:
	MaterialModel(TextureResource* texture, const GlModel& model, const std::string& name);

	inline void useSpecularMap(TextureResource* specularMap)
	{
		hasSpecularMap = true;
		this->specularMap = specularMap;
	}
	inline bool doesHaveSpecularMap() const { return hasSpecularMap; }
	inline TextureResource* getSpecularMap() const { return specularMap; }

	inline const GlModel& getGlModel() const { return model; }
	inline TextureResource& getTexture() { return *texture; }
	inline const TextureResource& getTexture() const { return *texture; }
	inline const std::string& getObjName() const { return objName; }

	inline bool operator==(const MaterialModel& model) const
	{
		if (model.objName != objName || 
			model.texture->getFilepath() != texture->getFilepath() ||
			model.hasSpecularMap != hasSpecularMap ||
			model.shineDistanceDamper != shineDistanceDamper ||
			model.reflectivity != reflectivity ||
			model.fullyRender != fullyRender)
			return false;

		if (model.hasSpecularMap && model.specularMap->promisedFetch().getID() != specularMap->promisedFetch().getID())
			return false;

		return true;
	}
};

namespace std
{
	template<>
	struct hash<MaterialModel>
	{
		std::size_t operator()(const MaterialModel& model) const
		{
			std::size_t h = std::hash<std::string>()(model.getObjName()) ^
				hash<std::string>()(model.getTexture().getFilepath()) ^
				std::hash<bool>()(model.doesHaveSpecularMap()) ^
				std::hash<bool>()(model.fullyRender) ^
				std::hash<float>()(model.shineDistanceDamper) ^
				std::hash<float>()(model.reflectivity);

			if (model.doesHaveSpecularMap())
				h ^= hash<std::string>()(model.getSpecularMap()->getFilepath());

			return h;
		}
	};
}