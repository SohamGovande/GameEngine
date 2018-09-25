#pragma once
#include "TextureResource.h"

/*
Use verbose names because dumb microsoft decided to typedef BOOL, BOOLEAN, INT, UINT,
and FLOAT, so nobody can use them as enums
*/
enum MaterialPropertyType
{
	UNSIGNED_INTEGER, INTEGER, FLOATING_DECIMAL, SIMPLE_BOOLEAN, TEXTURE_RESOURCE
};

template<typename T>
inline MaterialPropertyType GetMaterialPropertyType()
{
	static_assert(false);
}
template<> inline MaterialPropertyType GetMaterialPropertyType<float>() { return MaterialPropertyType::FLOATING_DECIMAL; }

template<> inline MaterialPropertyType GetMaterialPropertyType<bool>() { return MaterialPropertyType::SIMPLE_BOOLEAN; }

template<> inline MaterialPropertyType GetMaterialPropertyType<int>() { return MaterialPropertyType::INTEGER; }

template<> inline MaterialPropertyType GetMaterialPropertyType<unsigned int>() { return MaterialPropertyType::UNSIGNED_INTEGER; }

template<> inline MaterialPropertyType GetMaterialPropertyType<TextureResource*>() { return MaterialPropertyType::TEXTURE_RESOURCE; }
