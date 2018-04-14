#include "TimedScope.h"
#include <iostream>
#include <chrono>

TimedScope::TimedScope(const std::string& taskName)
	: taskName(taskName), begin(std::chrono::high_resolution_clock::now().time_since_epoch().count())
{	
}


TimedScope::~TimedScope()
{
	long long int end = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::cout << "Task " << taskName << " took " << (end - begin) / 1000000.0 << "ms." << std::endl;
}
