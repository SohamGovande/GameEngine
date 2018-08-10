#include "ComponentRegistry.h"
#include "Components/MotionComponent.h"

ComponentRegistry::ComponentRegistry()
{
	registerComp<MOTION, MotionComponent>("motion");
}

template<ComponentID ID, typename T>
void ComponentRegistry::registerComp(const std::string& name)
{
	componentNames.emplace(name, ID);
	constructorProvider.emplace(ID, []() -> void* { return reinterpret_cast<void*>(new T); });
}