#pragma once
#include <unordered_map>
#include <string>
#include "ReplacableElement.h"

class ElementMap
{
public:
	using MapType = std::unordered_map<std::string, ReplacableElement>;

private:
	MapType elements;

public:
	ElementMap();

	std::string apply(const std::string& in) const;
	
	void insert(const ElementMap& other);
	const ReplacableElement& requiredFetch(const std::string& key) const;

	const std::string& operator[](const std::string& key) const;

	inline MapType& getUnderlyingMap() { return elements; }
	inline const MapType& getUnderlyingMap() const { return elements; }
};
