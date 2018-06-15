#pragma once
#include <unordered_map>
#include <glm/vec2.hpp>
#include "GlMacros.h"

class GlStateManager
{
private:
	std::unordered_map<unsigned int, bool> enabledStates;

	unsigned int cullFace;
	glm::uvec2 blendFunc;
public:
	GlStateManager();
	
	inline void setCulling(unsigned int faces)
	{
		enable(GL_CULL_FACE);
		if (cullFace != faces)
		{
			GlCall(glCullFace(faces));
			cullFace = faces;
		}
	}
	inline void stopCulling()
	{
		disable(GL_CULL_FACE);
	}

	inline void setBlending(unsigned int srcBlend, unsigned int dstBlend)
	{
		enable(GL_BLEND);
		glm::uvec2 blend(srcBlend, dstBlend);
		if (blendFunc != blend)
		{
			blendFunc = blend;
			GlCall(glBlendFunc(blend.x, blend.y));
		}
	}
	inline void stopBlend() 
	{
		disable(GL_BLEND);
	}

	void enable(unsigned int state);
	void disable(unsigned int state);
};
