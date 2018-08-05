#pragma once
#include <functional>
#include <array>
#include "Uniform.t.h"
#include "Shader.h"


template<typename T, size_t NShaders>
class SyncWithUniform
{
public:
	using PairType = std::pair<std::reference_wrapper<Shader>, std::reference_wrapper<Uniform<T>>>;
	using StorageType = std::array<PairType, NShaders>;

private:
	T value;
	StorageType uniforms;

public:
	SyncWithUniform(const StorageType& targets);

	void operator=(const T& value);

	inline void set(const T& value) { this->value = value; }
	void sync() const;

	//Use SFINAE to check whether arithmetic operators are overloadable, and if so, override them
	template<class = decltype(std::declval<T>() + std::declval<T>())>
	inline decltype(auto) operator+(const T& right) const { return this->value + right; }

	template<class = decltype(std::declval<T>() - std::declval<T>())>
	inline decltype(auto) operator-(const T& right) const { return this->value - right; }

	template<class = decltype(std::declval<T>() * std::declval<T>())>
	inline decltype(auto) operator*(const T& right) const { return this->value * right; }

	template<class = decltype(std::declval<T>() / std::declval<T>())>
	inline decltype(auto) operator/(const T& right) const { return this->value / right; }

	template<class = decltype(std::declval<T>() += std::declval<T>())>
	inline void operator+=(const T& right) { this->value += right; sync(); }

	template<class = decltype(std::declval<T>() -= std::declval<T>())>
	inline void operator-=(const T& right) { this->value -= right; sync(); }

	template<class = decltype(std::declval<T>() *= std::declval<T>())>
	inline void operator*=(const T& right) { this->value *= right; sync(); }

	template<class = decltype(std::declval<T>() /= std::declval<T>())>
	inline void operator/=(const T& right) { this->value /= right; sync(); }

	template<class = decltype(std::declval<T>() + std::declval<T>()), size_t RightNShaderN>
	inline decltype(auto) operator+(const SyncWithUniform<T, RightNShaderN>& right) const { return this->value + right.get(); }

	template<class = decltype(std::declval<T>() - std::declval<T>()), size_t RightNShaderN>
	inline decltype(auto) operator-(const SyncWithUniform<T, RightNShaderN>& right) const { return this->value - right.get(); }

	template<class = decltype(std::declval<T>() * std::declval<T>()), size_t RightNShaderN>
	inline decltype(auto) operator*(const SyncWithUniform<T, RightNShaderN>& right) const { return this->value * right.get(); }

	template<class = decltype(std::declval<T>() / std::declval<T>()), size_t RightNShaderN>
	inline decltype(auto) operator/(const SyncWithUniform<T, RightNShaderN>& right) const { return this->value / right.get(); }

	template<class = decltype(std::declval<T>() += std::declval<T>()), size_t RightNShaderN>
	inline void operator+=(const SyncWithUniform<T, RightNShaderN>& right) { this->value += right.get(); sync(); }

	template<class = decltype(std::declval<T>() -= std::declval<T>()), size_t RightNShaderN>
	inline void operator-=(const SyncWithUniform<T, RightNShaderN>& right) { this->value -= right.get(); sync(); }

	template<class = decltype(std::declval<T>() *= std::declval<T>()), size_t RightNShaderN>
	inline void operator*=(const SyncWithUniform<T, RightNShaderN>& right) { this->value *= right.get(); sync(); }

	template<class = decltype(std::declval<T>() /= std::declval<T>()), size_t RightNShaderN>
	inline void operator/=(const SyncWithUniform<T, RightNShaderN>& right) { this->value /= right.get(); sync(); }

	inline const T& get() const { return value; }
	inline operator const T&() const { return value; }
};
