#pragma once
#include <memory.h>
#include <functional>
#include <string>
#include "Renderer/MaterialModel.h"
#include "MaterialPropertyTypes.h"

class MaterialProperty
{
private:
	unsigned int offset, size;
	MaterialPropertyType type;

public:
	inline MaterialProperty(unsigned int offset, unsigned int size, MaterialPropertyType type)
		: offset(offset), size(size), type(type)
	{
	}

	inline void update(MaterialModelProperties& properties, const void* data) const
	{
		memcpy(reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(&properties) + offset),
			data, size);
	}
	inline MaterialPropertyType getType() const { return type; }
	inline unsigned int getOffset() const { return offset; }
};