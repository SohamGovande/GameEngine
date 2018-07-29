#include "ReplacableElement.h"

ReplacableElement::ReplacableElement()
{
}

ReplacableElement::ReplacableElement(const char* data)
	: data(data)
{
}

ReplacableElement::ReplacableElement(const ReplacableElement& other)
	: data(other.data)
{
}

ReplacableElement::ReplacableElement(ReplacableElement&& other)
	: data(std::move(other.data))
{
}

ReplacableElement& ReplacableElement::operator=(const ReplacableElement& other)
{
	if (this != &other)
	{
		data = other.data;
	}
	return *this;
}

ReplacableElement::ReplacableElement(const std::string& data)
	: data(data)
{
}

void ReplacableElement::assign(const std::string& data)
{
	this->data = data;
}

