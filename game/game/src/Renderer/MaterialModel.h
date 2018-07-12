#pragma once
#include <vector>
#include <memory>
#include <glm/vec3.hpp>

#include "ResourceMgr/TextureResource.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Mesh.h"

struct MaterialModelProperties
{
	
	TextureResource *specularMap, *normalMap, *parallaxMap;
	bool fullyRender;
	float shineDistanceDamper, reflectivity, parallaxMapAmplitude;

	MaterialModelProperties()
		: specularMap(nullptr), normalMap(nullptr), parallaxMap(nullptr),
		fullyRender(false), shineDistanceDamper(0), reflectivity(0), parallaxMapAmplitude(0)
	{
	}

	inline bool operator==(const MaterialModelProperties& other) const
	{
		return specularMap == other.specularMap 
			&& fullyRender == other.fullyRender
			&& shineDistanceDamper == other.shineDistanceDamper
			&& reflectivity == other.reflectivity
			&& normalMap == other.normalMap
			&& parallaxMap == other.parallaxMap
			&& parallaxMapAmplitude == other.parallaxMapAmplitude;
	}

	inline bool hasSpecularMap() const
	{
		return specularMap != nullptr;
	}

	inline bool hasNormalMap() const
	{
		return normalMap != nullptr;
	}

	inline bool hasParallaxMap() const 
	{
		return parallaxMap != nullptr;
	}

	inline bool operator!=(const MaterialModelProperties& other) const
	{
		return !operator==(other);
	}
};

template<>
struct std::hash<MaterialModelProperties>
{
	size_t operator()(const MaterialModelProperties& obj)
	{
		size_t i = hash<bool>()(obj.fullyRender) ^
			hash<float>()(obj.shineDistanceDamper) ^
			hash<float>()(obj.reflectivity) ^
			hash<float>()(obj.parallaxMapAmplitude);
	
		if (obj.hasSpecularMap())
			i ^= hash<std::string>()(obj.specularMap->getFilepath());
		if (obj.hasNormalMap())
			i ^= hash<std::string>()(obj.normalMap->getFilepath());
		if (obj.hasParallaxMap())
			i ^= hash<std::string>()(obj.parallaxMap->getFilepath());
		return i;
	}
};

class MaterialModel
{
private:
	TextureResource& texture;
	std::string objName;

public:
	MaterialModelProperties properties;

public:
	MaterialModel(TextureResource& texture, const std::string& name);

	inline TextureResource& getTexture() { return texture; }
	inline const TextureResource& getTexture() const { return texture; }
	inline const std::string& getObjName() const { return objName; }

	inline bool operator==(const MaterialModel& model) const
	{
		return model.objName == objName &&
			model.texture.getFilepath() == texture.getFilepath() &&
			model.properties == properties;
	}
};

class RenderableMaterialModel
	: public MaterialModel
{
private:
	GlModel model;

public:
	RenderableMaterialModel(GlModel&& model, TextureResource& texture, const std::string& name);
	
	inline const GlModel& getGlModel() const { return model; }
};

template<>
struct std::hash<MaterialModel>
{
	std::size_t operator()(const MaterialModel& model) const
	{
		std::size_t h = std::hash<std::string>()(model.getObjName()) ^
			std::hash<std::string>()(model.getTexture().getFilepath()) ^
			std::hash<MaterialModelProperties>()(model.properties);
			
		return h;
	}
};