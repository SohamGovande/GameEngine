#include "EntityConstructor.h"

EntityConstructor::EntityConstructor(const rapidjson::Document& doc, const ComponentRegistry& componentRegistry, ResourceMgr& resourceMgr)
	: componentRegistry(componentRegistry)
{
	rapidjson::Value::ConstArray idArray = doc.FindMember("ids")->value.GetArray();
	ids.reserve(idArray.Size());
	for (const auto& id : idArray)
		ids.push_back(id.GetUint());

	rapidjson::Value::ConstMemberIterator componentContainerIt = doc.FindMember("components");
	if (componentContainerIt != doc.MemberEnd())
	{
		rapidjson::Value::ConstObject componentContainer = componentContainerIt->value.GetObject();
		components.reserve(componentContainer.MemberCount());

		for (const auto& comp : componentContainer)
			components.emplace_back(componentRegistry.nameToId(comp.name.GetString()), comp.value.GetObject());
	}

	model = &resourceMgr.model(doc.FindMember("model")->value.GetString());
}

EntityConstructor::EntityConstructor(EntityConstructor&& other)
	: model(other.model), ids(std::move(other.ids)), components(std::move(other.components)),
	componentRegistry(other.componentRegistry)
{
}

void EntityConstructor::construct(Entity& owner) const
{
	owner.setMaterialModel(*model);

	for (unsigned int id : ids)
		owner.addEID(id);

	for (const ComponentConstructor& constructor : components)
	{
		Component* component = componentRegistry.construct(constructor.getId());
		component->setParams(constructor.getParams());
		owner.addComponent(component);
	}
}
