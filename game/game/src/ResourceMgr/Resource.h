#pragma once
#include <functional>

template<typename T>
class Resource
{
protected:
	T* value;
	bool present;

public:
	Resource();
	Resource(T* val);
	virtual ~Resource();

	void free();
	void load();

	operator T*();
	operator T&();
	operator const T&() const;

	T& ensuredFetch();
	const T& promisedFetch() const;
	T& promisedFetch();

protected:
	virtual void releaseMemory() {}
	virtual void generate() = 0;
};
