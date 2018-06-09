#include <filesystem>
#include <vector>
#include "EntityRegistry.h"
#include "IOHelper.h"

EntityRegistry::EntityRegistry(const ComponentRegistry& componentRegistry, ResourceMgr& resourceMgr)
{
	namespace fs = std::experimental::filesystem::v1;
	for (const fs::directory_entry& dirEntry : fs::directory_iterator("res/entities"))
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
