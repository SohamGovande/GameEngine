#pragma once
#include <memory.h>
#include "Renderer/MaterialModel.h"
#include "MaterialPropertyTypes.h"

class MaterialProperty
{
private:
	unsigned int offset, size;
	MaterialPropertyType type;

public:
	MaterialProperty(unsigned int offset, unsigned int size, MaterialPropertyType type);

	void update(MaterialModelProperties& properties, const void* data) const;

	inline MaterialPropertyType getType() const { return type; }
	inline unsigned int getOffset() const { return offset; }
};