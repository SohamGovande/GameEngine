#include <iostream>
#include <SFML/Window/Mouse.hpp>
#include "Toolkit.h"

static sf::Window* window;
static bool cursorGrabbed = false;

namespace Toolkit
{
	sf::Window& getWindow() 
	{
		return *window;
	}

	void setWindow(sf::Window& win)
	{
		window = &win;
	}

	unsigned int getWidth()
	{
		return window->getSize().x;
	}

	unsigned int getHeight()
	{
		return window->getSize().y;
	}

	glm::uvec2 getSize()
	{
		sf::Vector2u size = window->getSize();
		glm::uvec2* ptr = reinterpret_cast<glm::uvec2*>(&size);
		return *ptr;
	}

	glm::ivec2 getPosition()
	{
		sf::Vector2i pos = window->getPosition();
		return *reinterpret_cast<glm::ivec2*>(&pos);
	}

	void hideCursor()
	{
		window->setMouseCursorVisible(false);
		window->setMouseCursorGrabbed(true);
		cursorGrabbed = true;
	}

	void showCursor()
	{
		if (!cursorGrabbed)
			return;
		sf::Vector2u windowSize = window->getSize();
		sf::Vector2i relativeCenterPos((int)windowSize.x/2, (int)windowSize.y/2);
		sf::Mouse::setPosition(relativeCenterPos, *window);

		window->setMouseCursorVisible(true);
		window->setMouseCursorGrabbed(false);
		cursorGrabbed = false;
	}

	bool isKeyPressed(sf::Keyboard::Key key)
	{	
		return sf::Keyboard::isKeyPressed(key);
	}

	glm::ivec2 getCursorPos()
	{
		sf::Vector2i pos = sf::Mouse::getPosition();
		return *reinterpret_cast<glm::ivec2*>(&pos);
	}

	void setCursorPos(const glm::ivec2& pos)
	{
		sf::Mouse::setPosition(reinterpret_cast<const sf::Vector2i&>(pos));
	}

	bool isCursorGrabbed()
	{
		return cursorGrabbed;
	}
}