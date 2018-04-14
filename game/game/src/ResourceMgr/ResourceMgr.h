#pragma once
#include <list>
#include <memory>
#include "TextureResource.h"
#include "ModelResource.h"
#include "MeshResource.h"

class ResourceMgr
{
private:
	std::list<TextureResource> textures;
	std::list<ModelResource> models;

public:
	TextureResource *grass, *axeSpecularMap;
	ModelResource *playerModel, *treeModel, *fernModel, *axeModel, 
		*icosphere;

	ResourceMgr();
	~ResourceMgr();

	void loadResources();
};

