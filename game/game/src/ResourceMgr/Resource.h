#pragma once

template<typename T>
class Resource
{
protected:
	T* value;
	bool present;

	void(*freeFunc)(void* _thisPtr);

	virtual void generate() = 0;
public:
	Resource();
	Resource(T* val);
	virtual ~Resource();

	void free();

	void load();

	T& ensuredFetch();

	operator T*();

	operator T&();
	operator const T&() const;

	inline const T& promisedFetch() const
	{
		if (value == nullptr)
			__debugbreak();
		return *value;
	}
};
