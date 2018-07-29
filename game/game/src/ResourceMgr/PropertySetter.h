#pragma once
#include <string>
#include <rapidjson/document.h>
#include "MaterialProperty.h"

union PropertySetterValue
{
	float floatVal;
	int intVal;
	unsigned int uintVal;
	bool boolVal;
	TextureResource* textureVal;
};

class PropertySetter
{
private:
	const MaterialProperty target;
	const PropertySetterValue value;
	
public:
	inline PropertySetter(const MaterialProperty& target, const PropertySetterValue& value)
		: target(target), value(value)
	{
	}
	
	inline void apply(RenderableMaterialModel& model) const
	{
		target.update(model.properties, reinterpret_cast<const void*>(&value));
	}

	inline const PropertySetterValue& getValue() const { return value; }
	inline const MaterialProperty& getTargetProperty() const { return target; }
};
