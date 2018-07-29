#pragma once
#include <string>

class ReplacableElement
{
private:
	std::string data;

public:
	ReplacableElement();
	ReplacableElement(const std::string& data);
	ReplacableElement(const char* data);
	ReplacableElement(const ReplacableElement& other);
	ReplacableElement(ReplacableElement&& other);

	ReplacableElement& operator=(const ReplacableElement& other);

	void assign(const std::string& data);
	
	inline const std::string& getData() const { return data; }
	inline std::string& getData() { return data; }
};

