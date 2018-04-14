#pragma once
#include <string>

class TimedScope
{
private:
	std::string taskName;
	long long int begin;

public:
	TimedScope(const std::string& taskName);
	~TimedScope();
};

