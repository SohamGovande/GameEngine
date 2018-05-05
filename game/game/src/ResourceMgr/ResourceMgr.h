#pragma once
#include <list>
#include <memory>
#include "TextureResource.h"
#include "ModelResource.h"
#include "MeshResource.h"

class ResourceMgr
{
	using TexRes = TextureResource*;
	using MdlRes = ModelResource*;
private:
	std::list<TextureResource> textures;
	std::list<ModelResource> models;

	TexRes loadTex(const std::string& texture);
	MdlRes loadMdl(const std::string& mesh, const std::string& texture);

public:
	TexRes grass, axeSpecularMap;
	MdlRes playerModel, evergreenTree, birchTree, bt1, bt2, fernModel, axeModel, lanternModel, dragonModel;

	ResourceMgr();
	~ResourceMgr();

	void loadResources();
};

