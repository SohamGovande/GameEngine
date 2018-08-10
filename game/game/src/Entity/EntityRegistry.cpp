#include <filesystem>
#include <vector>
#include "EntityRegistry.h"
#include "IOHelper.h"

EntityRegistry::EntityRegistry(const ComponentRegistry& componentRegistry, ResourceMgr& resourceMgr)
{
	
	for (const std::filesystem::directory_entry& dirEntry : std::filesystem::directory_iterator("res/entities"))
	{
		std::string folder = dirEntry.path().string();

		rapidjson::Document doc;
		doc.Parse(&IOHelper::readFile(folder + "/metadata.json")[0]);
		
		entityCreators.emplace(
			doc.FindMember("name")->value.GetString(),
			EntityConstructor(doc, componentRegistry, resourceMgr)
		);
	}

}
