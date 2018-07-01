#include <SFML/Window.hpp>
#include <chrono>
#include <thread>

#include "Game.h"

static void RunGame(sf::Window& window)
{
	Game game;
	game.init();

	long long int lastTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	long long int startTime = lastTime;
	double delta = 0;
	constexpr double ticksPerSecond = 20;

	bool running = true;

	while (running)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				running = false;
				game.onWindowClosed();
			}
			else if (event.type == sf::Event::Resized)
			{
				game.onWindowResized(event);
			}
			else if (event.type == sf::Event::GainedFocus)
			{
				game.onFocusGained(event);
			}
			else if (event.type == sf::Event::LostFocus)
			{
				game.onFocusLost(event);
			}
		}
		
		long long int now = std::chrono::high_resolution_clock::now().time_since_epoch().count();

		delta += (now - lastTime) / 1000000000.0 * ticksPerSecond;

		while (delta >= 1)
		{
			game.tick();
			delta--;
		}
		lastTime = now;

		game.render((float)delta);
		window.display();
	}
	
}

int main()
{
	sf::ContextSettings settings;
	settings.attributeFlags = settings.Default;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 0;
	settings.majorVersion = 3;
	settings.minorVersion = 3;

	sf::Window window(sf::VideoMode(933, 700), "OpenGL", sf::Style::Default, settings);

	window.setVerticalSyncEnabled(true);
	window.setActive(true);

	Toolkit::setWindow(window);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize glew" << std::endl;
	}

	RunGame(window);
	return 0;
}