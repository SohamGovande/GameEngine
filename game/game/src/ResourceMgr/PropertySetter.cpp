#include "PropertySetter.h"

PropertySetter::PropertySetter(const MaterialProperty& target, const PropertySetterValue& value)
	: target(target), value(value)
{
}

void PropertySetter::apply(RenderableMaterialModel& model) const
{
	target.update(model.properties, reinterpret_cast<const void*>(&value));
}