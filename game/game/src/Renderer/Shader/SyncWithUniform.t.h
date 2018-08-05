#pragma once
#include "SyncWithUniform.h"

template<typename T, size_t NShaders>
SyncWithUniform<T, NShaders>::SyncWithUniform(const SyncWithUniform<T, NShaders>::StorageType& targets)
	: uniforms(targets)
{

}

template<typename T, size_t NShaders>
void SyncWithUniform<T, NShaders>::operator=(const T& value)
{
	this->value = value;
	sync();
}

template<typename T, size_t NShaders>
inline void SyncWithUniform<T, NShaders>::sync() const
{
	for (const PairType& shaderUniformPair : uniforms)
	{
		shaderUniformPair.first.get().bind();
		shaderUniformPair.second.get().uncheckedSet(value);
	}
	
}
