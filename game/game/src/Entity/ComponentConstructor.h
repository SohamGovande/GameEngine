#pragma once
#include <vector>
#include <string>
#include <rapidjson/document.h>
#include "Components/ComponentIDs.h"

class ComponentConstructor
{
private:
	ComponentID id;
	rapidjson::Value::ConstObject params;

public:
	inline ComponentConstructor(ComponentID id, const rapidjson::Value::ConstObject& params)
		: id(id), params(params)
	{}

	inline const rapidjson::Value::ConstObject& getParams() const { return params; }
	inline ComponentID getId() const { return id; }
};

