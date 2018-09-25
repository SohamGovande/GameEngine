#pragma once
#include <iostream>
#include "Resource.h"

template<typename T>
Resource<T>::Resource()
	: value(nullptr), present(false)
{
}

template<typename T>
Resource<T>::Resource(T* value)
	: value(value), present(true)
{

}

template<typename T>
Resource<T>::~Resource()
{
}

template<typename T>
void Resource<T>::free()
{
	if (present)
	{
		releaseMemory();
		delete value;
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
inline const T& Resource<T>::promisedFetch() const
{
	if (value == nullptr)
		__debugbreak();
	return *value;
}

template<typename T>
inline T& Resource<T>::promisedFetch()
{
	if (value == nullptr)
		__debugbreak();
	return *value;
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