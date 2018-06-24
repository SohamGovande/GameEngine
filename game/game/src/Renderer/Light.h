#pragma once
#include <glm/vec3.hpp>
#define MAX_LIGHTS 5
#define MAX_LIGHTS_STR "5"

class Light
{
private:
	glm::vec3 pos, color;
	bool posChanged, colorChanged, attenuationChanged, brightnessChanged;
	float attenuation, brightness;

public:
	Light(const glm::vec3& pos, const glm::vec3& color, float attenuation, float brightness);

	inline bool wasAttenuationChanged() const { return attenuationChanged; }
	inline bool wasColorChanged() const { return colorChanged; }
	inline bool wasPosChanged() const { return posChanged; }
	inline bool wasBrightnessChanged() const { return brightnessChanged; }

	inline float getBrightness() const { return brightness; }
	inline void setBrightness(float brightness) { this->brightness = brightness; brightnessChanged = true; }

	inline float getAttenuation() const { return attenuation; }
	inline void setAttenuation(float attenuation) { this->attenuation = attenuation; attenuationChanged = true; }

	inline const glm::vec3& getPos() const { return pos; }
	inline void setPos(const glm::vec3& pos) { this->pos = pos; posChanged = true; }

	inline const glm::vec3& getColor() const { return color; }
	inline void setColor(const glm::vec3& color) { this->color = color; colorChanged = true; }
};

