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
	PropertySetter(const MaterialProperty& target, const PropertySetterValue& value);
	
	void apply(RenderableMaterialModel& model) const;

	inline const PropertySetterValue& getValue() const { return value; }
	inline const MaterialProperty& getTargetProperty() const { return target; }
};
