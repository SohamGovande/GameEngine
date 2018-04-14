#include "ResourceMgr.h"
#include "TimedScope.h"

#define Load(storeIn, container, ...) \
container.emplace_back(__VA_ARGS__);\
storeIn = &container.back()

#define LoadTex(storeIn, ...) Load(storeIn, textures, __VA_ARGS__)
#define LoadMesh(storeIn, ...) Load(storeIn, models, __VA_ARGS__)

ResourceMgr::ResourceMgr()
{
	
}

ResourceMgr::~ResourceMgr()
{
}

void ResourceMgr::loadResources()
{
	TimedScope timer("load resources");

	LoadTex(grass, "grassblock", false);
	LoadTex(axeSpecularMap, "axeSpecular", true);

	LoadMesh(playerModel, "realisticPerson", "realisticPerson");
	LoadMesh(fernModel, "fern", "fern");
	LoadMesh(treeModel, "EvergreenTree", "EvergreenTree");
	LoadMesh(axeModel, "axe", "axe");

	MaterialModel& axe = axeModel->ensuredFetch();
	axe.useSpecularMap(axeSpecularMap);
	axe.fullyRender = false;
	axe.reflectivity = 1;
	axe.shineDistanceDamper = 8;

	LoadMesh(icosphere, "icosphere", "icosphere");

}
