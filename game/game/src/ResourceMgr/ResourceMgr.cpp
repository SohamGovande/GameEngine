#include "ResourceMgr.h"
#include "TimedScope.h"

ResourceMgr::ResourceMgr()
{
	
}

ResourceMgr::~ResourceMgr()
{
}

ResourceMgr::TexRes ResourceMgr::loadTex(const std::string& texture)
{
	textures.emplace_back(texture, false);
	return &textures.back();
}

ResourceMgr::MdlRes ResourceMgr::loadMdl(const std::string& mesh, const std::string& texture)
{
	models.emplace_back(mesh, texture);
	return &models.back();
}

void ResourceMgr::loadResources()
{
	TimedScope timer("load resources");

	grass = loadTex("grassblock");
	axeSpecularMap = loadTex("axeSpecular");

	playerModel = loadMdl("realisticPerson", "realisticPerson");
	fernModel = loadMdl("fern", "fern");
	evergreenTree = loadMdl("EvergreenTree", "EvergreenTree");
	birchTree = loadMdl("DeciduousTree", "Birchtexture");
	bt1 = loadMdl("DeciduousTree_0", "Birchtexture");
	bt2 = loadMdl("DeciduousTree_1", "Birchtexture");
	axeModel = loadMdl("axe", "axe");
	lanternModel = loadMdl("lantern", "lantern");

	dragonModel = loadMdl("dragon", "green");

	MaterialModel& axe = axeModel->ensuredFetch();
	axe.useSpecularMap(axeSpecularMap);
	axe.reflectivity = 1;
	axe.shineDistanceDamper = 8;
}
