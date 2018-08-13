#include <SFML/Window.hpp>
#include <chrono>
#include <thread>

#include "Renderer/PPE/GaussianSinglePassBlur.t.h"
#include "Game.h"
#include "Comparator.h"

static void RunGame(sf::Window& window)
{
	Game game;
	game.init();

	long long int lastTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	long long int startTime = lastTime;
	double delta = 0;
	constexpr double ticksPerSecond = 20;
	constexpr double ticksPerNanosecond = ticksPerSecond / 1000000000.0;
	bool running = true;

	GaussianAxisBlur<BlurAxis::HORIZONTAL> horizPass(933, 700);
	GaussianAxisBlur<BlurAxis::VERTICAL> vertPass(933, 700);

	QuadVBO quadVbo;

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
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Key::Escape)
					Toolkit::showCursor();
			}
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				Toolkit::hideCursor();
			}
		}
		
		long long int now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		float frameDelta = (now - lastTime) * ticksPerNanosecond;

		delta += (now - lastTime) * ticksPerNanosecond;

		while (delta >= 1)
		{
			game.tick();
			delta--;
		}
		lastTime = now;

		game.render((float)delta, frameDelta);
		
		window.display();
	}
	
}


static void RunApplication()
{
	sf::ContextSettings settings;
	settings.attributeFlags = settings.Default;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 0;
	settings.majorVersion = 3;
	settings.minorVersion = 3;

	sf::Window window(sf::VideoMode(933, 700), "C++ OpenGL Game Test", sf::Style::Default, settings);

	window.setVerticalSyncEnabled(true);
	window.setActive(true);

	Toolkit::setWindow(window);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize glew" << std::endl;
	}

	RunGame(window);
}

int main()
{
	RunApplication();
	return 0;
}