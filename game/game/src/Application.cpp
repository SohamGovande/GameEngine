#include <SFML/Window.hpp>
#include <chrono>

#include "ResourceMgr/ResourceMgr.h"
#include "Renderer/Renderers/MasterRenderer.h"
#include "Renderer/GlMacros.h"
#include "Renderer/VertexBufferLayout.h"
#include "Renderer/PPE/GaussianSinglePassBlur.t.h"
#include "Renderer/PPE/TestEffect.h"
#include "Terrain/World.h"
#include "Terrain/TerrainGen.h"
#include "TimedScope.h"

static void runGame(sf::Window& window)
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

	Toolkit::hideCursor();

	{
		TimedScope timer("generate terrain");
		constexpr int size = 1;
		for (int x = -size; x < size; x++)
			for (int y = -size; y < size; y++)
				terrainGen.generate(world, resourceMgr, x, y);
	}

	world.sendEntities(renderer);
	world.sendTerrain(renderer);

	TestEffect effect(933, 700);

	float quadVertices[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};
	VertexBuffer vbo(quadVertices, sizeof(quadVertices));
	VertexArray vao;
	VertexBufferLayout layout;
	layout.push<float>(2);
	layout.push<float>(2);
	vao.addBuffer(vbo, layout);

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
				Toolkit::hideCursor();
			else if (event.type == sf::Event::LostFocus)
				Toolkit::showCursor();
			else if (event.type == sf::Event::KeyPressed)
				if (event.key.code == sf::Keyboard::Key::T)
					renderer.toggleWireframeView();
		}
		
		long long int now = std::chrono::high_resolution_clock::now().time_since_epoch().count();

		delta += (now - lastTime) / 1000000000.0 * ticksPerSecond;

		while (delta >= 1)
		{
			world.tick();
			camera.tick(world);

			delta--;
		}

		const float partialTicks = (float)delta;
		lastTime = now;

		world.getPerson().position.y = world.getTerrainHeight(world.getPerson().position.x, world.getPerson().position.z) + .5f;

		renderer.prepare();
		camera.performRotations(partialTicks);
		renderer.render(partialTicks, camera);

		GlCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

		window.display();
	}
	
}

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

	Toolkit::setWindow(window);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize glew" << std::endl;
	}

	runGame(window);
	return 0;
}