#include "Light.h"

Light::Light(const glm::vec3& pos, const glm::vec3& color, float attenuation, float brightness)
	: pos(pos), color(color), attenuation(attenuation/1000.0f), brightness(brightness),
	posChanged(true), colorChanged(true), attenuationChanged(true),  brightnessChanged(true)
{
}
