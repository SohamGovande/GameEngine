#pragma once
#include <iostream>
#include "Resource.h"

template<typename T>
Resource<T>::Resource()
	: value(nullptr), present(false), freeFunc(nullptr)
{
}

template<typename T>
Resource<T>::Resource(T* value)
	: value(value), present(true), freeFunc(nullptr)
{

}

template<typename T>
Resource<T>::~Resource()
{
	free();
}

template<typename T>
void Resource<T>::free()
{
	if (present)
	{
		if (freeFunc == nullptr)
			delete value;
		else
			freeFunc(this);
		present = false;
	}
}

template<typename T>
T& Resource<T>::ensuredFetch()
{
	load();
	return *value;
}

template<typename T>
inline Resource<T>::operator T*()
{
	load();
	return value;
}

template<typename T>
inline Resource<T>::operator T&()
{
	return ensuredFetch();
}

template<typename T>
inline Resource<T>::operator const T&() const
{
	return promisedFetch();
}

template<typename T>
void Resource<T>::load()
{
	if (!present)
	{
		generate();
		present = true;
	}
}