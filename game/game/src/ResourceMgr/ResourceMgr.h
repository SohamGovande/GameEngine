#pragma once
#include <rapidjson/document.h>
#include <list>
#include <string>
#include <unordered_map>
#include "TextureResource.h"
#include "ModelResource.h"
#include "MaterialProperty.h"

class ResourceMgr
{
private:
	std::unordered_map<std::string, TextureResource> textures;
	std::unordered_map<std::string, ModelResource> models;

	std::unordered_map<std::string, MaterialProperty> materialProperties;

public:
	ResourceMgr();
	ResourceMgr(const ResourceMgr& other) = delete;
	ResourceMgr(ResourceMgr&& other) = delete;
	~ResourceMgr();

	ResourceMgr& operator=(const ResourceMgr& other) = delete;

	void initMaterialProperties();

	void loadTexturesFile();
	void loadModelsFile();
	void loadResources();
	
	inline const ModelResource& model(const std::string& name) const { return models.find(name)->second; }
	inline ModelResource& model(const std::string& name) { return models.find(name)->second; }

	inline const TextureResource& texture(const std::string& name) const { return textures.find(name)->second; }
	inline TextureResource& texture(const std::string& name) { return textures.find(name)->second; }

private:
	TextureResource& loadTex(const std::string& name, const std::string& texture);
	TextureResource& parseJsonTexture(const rapidjson::Value& value);

};

