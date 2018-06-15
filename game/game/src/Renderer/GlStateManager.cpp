#include "GlStateManager.h"

GlStateManager::GlStateManager()
	: blendFunc({ 0, 0 }),
	cullFace(0)
{
	
}

void GlStateManager::disable(unsigned int state)
{
	std::unordered_map<unsigned int, bool>::iterator it = enabledStates.find(state);
	if (it == enabledStates.end())
	{
		enabledStates.emplace(state, false);
		GlCall(glDisable(state));
	}
	else
	{
		if (it->second != false) //the state is disabled
		{
			GlCall(glDisable(state));
			it->second = false;
		}
	}
}


void GlStateManager::enable(unsigned int state)
{
	std::unordered_map<unsigned int, bool>::iterator it = enabledStates.find(state);
	if (it == enabledStates.end())
	{
		enabledStates.emplace(state, true);
		GlCall(glEnable(state));
	}
	else
	{
		if (it->second != true) //the state is disabled
		{
			GlCall(glEnable(state));
			it->second = true;
		}
	}
}
