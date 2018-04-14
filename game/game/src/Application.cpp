#include <SFML/Window.hpp>
#include <chrono>

#include "ResourceMgr/ResourceMgr.h"
#include "Renderer/Renderers/MasterRenderer.h"
#include "Renderer/GlMacros.h"
#include "Terrain/World.h"
#include "Terrain/TerrainGen.h"
#include "TimedScope.h"

int main()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 0;
	settings.majorVersion = 3;
	settings.minorVersion = 3;

	sf::Window window(sf::VideoMode(933, 700), "OpenGL", sf::Style::Default, settings);

	window.setVerticalSyncEnabled(true);
	window.setActive(true);

	Glfw::setWindow(window);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize glew" << std::endl;
	}

	// load resources, initialize the OpenGL states, ...
	{
		ResourceMgr resourceMgr;
		resourceMgr.loadResources();

		Camera camera;

		MasterRenderer renderer(60, 0.1f, 1000, resourceMgr);

		World world(resourceMgr);

		TerrainGen terrainGen(29);

		long long lastTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		long long startTime = lastTime;
		double delta = 0;
		constexpr double ticksPerSecond = 20;

		camera.controlEntity(&world.getPerson());

		Glfw::hideCursor();

		{
			TimedScope g("generate terrain");
			constexpr int size = 1;
			for (int x = -size; x < size; x++)
				for (int y = -size; y < size; y++)
					terrainGen.generate(world, resourceMgr, x, y);
		}
	//	terrainGen.generate(world, resourceMgr, 0, 0);

		world.sendEntities(renderer);
		world.sendTerrain(renderer);

		bool running = true;
		while (running)
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					running = false;
				else if (event.type == sf::Event::Resized)
					glViewport(0, 0, event.size.width, event.size.height);
				else if (event.type == sf::Event::GainedFocus)
					Glfw::hideCursor();
				else if (event.type == sf::Event::LostFocus)
					Glfw::showCursor();
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// draw...
			long long now = std::chrono::high_resolution_clock::now().time_since_epoch().count();

			delta += (now - lastTime) / 1000000000.0 * ticksPerSecond;

			while (delta >= 1)
			{
				world.tick();
				camera.tick(world);
				delta--;
			}

			const float partialTicks = (float)delta;
			lastTime = now;


			world.getPerson().position.y =
				world.getTerrainHeight(world.getPerson().position.x, world.getPerson().position.z) + .5f;

			renderer.prepare();

			camera.performRotations(partialTicks);

			renderer.render(partialTicks, camera);

			// end the current frame (internally swaps the front and back buffers)
			window.display();
		}

		// release resources...
	}
	return 0;
}