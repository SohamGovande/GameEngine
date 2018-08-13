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

	MaterialModelProperties();

	int compare(const MaterialModelProperties& other) const;
	bool operator==(const MaterialModelProperties& other) const;

	void loadAllTextures();

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

class MaterialModel
{
private:
	TextureResource& texture;
	std::string objName;

public:
	MaterialModelProperties properties;

public:
	MaterialModel(TextureResource& texture, const std::string& name);

	void loadAllTextures();
	int compare(const MaterialModel& other) const;

	inline TextureResource& getTexture() { return texture; }
	inline const TextureResource& getTexture() const { return texture; }
	inline const std::string& getObjName() const { return objName; }

	bool operator==(const MaterialModel& model) const;
	
};

class RenderableMaterialModel
	: public MaterialModel
{
private:
	GlModel model;

public:
	RenderableMaterialModel(GlModel&& model, TextureResource& texture, const std::string& name);
	
	int compareAgainstRenderable(const RenderableMaterialModel& other) const;

	inline const GlModel& getGlModel() const { return model; }
	inline GlModel& getGlModel() { return model; }
};


template<>
struct std::hash<MaterialModelProperties>
{
	size_t operator()(const MaterialModelProperties& obj) const;
};


template<>
struct std::hash<MaterialModel>
{
	std::size_t operator()(const MaterialModel& model) const;
};