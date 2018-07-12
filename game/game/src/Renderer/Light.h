#pragma once
#include <glm/vec3.hpp>
#define MAX_LIGHTS 5
#define MAX_LIGHTS_STR "5"

class Light
{
private:
	glm::vec3 pos, color, attenuation;
	bool posChanged, colorChanged, attenuationChanged;

public:
	inline Light(const glm::vec3& pos, const glm::vec3& color, const glm::vec3& attenuation)
		: pos(pos), color(color), attenuation(attenuation),
		posChanged(true), colorChanged(true), attenuationChanged(true)
	{
	}

	inline bool wasAttenuationChanged() const { return attenuationChanged; }
	inline bool wasColorChanged() const { return colorChanged; }
	inline bool wasPosChanged() const { return posChanged; }

	inline const glm::vec3& getAttenuation() const { return attenuation; }
	inline void setAttenuation(const glm::vec3& attenuation) { this->attenuation = attenuation; attenuationChanged = true; }

	inline const glm::vec3& getPos() const { return pos; }
	inline void setPos(const glm::vec3& pos) { this->pos = pos; posChanged = true; }

	inline const glm::vec3& getColor() const { return color; }
	inline void setColor(const glm::vec3& color) { this->color = color; colorChanged = true; }
};

