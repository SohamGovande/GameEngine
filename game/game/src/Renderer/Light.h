#pragma once
class Light
{
private:
	float posX, posY, posZ;
	float red, green, blue;

public:
	Light(const float posX, const float posY, const float posZ, const float red, const float green, const float blue);

	inline float getPosX() const { return posX; }
	inline float getPosY() const { return posY; }
	inline float getPosZ() const { return posZ; }
	inline void setPosX(float nPos) { posX = nPos; }
	inline void setPosY(float nPos) { posY = nPos; }
	inline void setPosZ(float nPos) { posZ = nPos; }

	inline float getRed() const { return red; }
	inline float getGreen() const { return green; }
	inline float getBlue() const { return blue; }
	inline void setRed(float color) { red = color; }
	inline void setGreen(float color) { green = color; }
	inline void setBlue(float color) { blue = color; }
};

