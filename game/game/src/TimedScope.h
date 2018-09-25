#pragma once
#include <string>

class TimedScope
{
private:
	std::string taskName;
	long long int begin;

public:
	TimedScope(const std::string& taskName);
	TimedScope(const TimedScope& other) = delete;
	TimedScope(TimedScope&& other) = delete;
	~TimedScope();

	TimedScope& operator=(const TimedScope& other) = delete;
};

