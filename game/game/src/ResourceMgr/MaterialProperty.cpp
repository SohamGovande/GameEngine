#include <memory.h>
#include "MaterialProperty.h"

MaterialProperty::MaterialProperty(unsigned int offset, unsigned int size, MaterialPropertyType type)
	: offset(offset), size(size), type(type)
{
}

void MaterialProperty::update(MaterialModelProperties& properties, const void* data) const
{
	memcpy(reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(&properties) + offset),
		data, size);
}