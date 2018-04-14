#pragma once
#include <SFML/Window.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <glm/vec2.hpp>

namespace Glfw
{
	sf::Window& getWindow();
	void setWindow(sf::Window& window);
	
	unsigned int getWidth();
	unsigned int getHeight();
	glm::uvec2 getSize();

	bool isKeyPressed(sf::Keyboard::Key key);

	glm::ivec2 getCursorPos();
	void setCursorPos(const glm::ivec2& pos);

	glm::ivec2 getPosition();

	void hideCursor();
	void showCursor();
}
