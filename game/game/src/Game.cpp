#include "Game.h"
#include "Toolkit.h"
#include "TimedScope.h"

Game::Game()
	: resourceMgr(),
	componentRegistry(),
	entityRegistry(componentRegistry, resourceMgr),

	renderer(),
	camera(renderer, 1.0f, 1000.0f, 60.f),
	world(entityRegistry),

	terrainGen(29)
{
}

Game::~Game()
{
}

void Game::onWindowClosed()
{
}

void Game::onFocusGained(const sf::Event& event)
{
	Toolkit::hideCursor();
}

void Game::onFocusLost(const sf::Event& event)
{
	Toolkit::showCursor();
}

void Game::onWindowResized(const sf::Event& event)
{
	GlCall(glViewport(0, 0, event.size.width, event.size.height));
	camera.loadProjectionMatrix();
}

void Game::init()
{
	camera.controlEntity(&world.getPerson());

	{
		TimedScope timer("generate terrain");
		constexpr int size = 2;
		for (int x = -size; x < size; x++)
			for (int y = -size; y < size; y++)
				terrainGen.generate(world, resourceMgr, entityRegistry, x, y);
	}
	world.sendEntities(renderer);
	world.sendTerrain(renderer);

	for (Entity& entity : world.getEntities())
	{
		if (entity.hasEntityID(7)) //Lantern
		{
			renderer.getLights().emplace_back(
				entity.position + glm::vec3(0, 50, 0),
				glm::vec3(255.0f, 182.0f, 0.0f)/255.f,
				glm::vec3(0.3f, 0.0001f, 0.0001f)
			);
		}
	}

	Toolkit::hideCursor();
}

void Game::tick()
{
	world.tick();
	camera.tick(world);
}

void Game::render(float partialTicks, float frameDelta)
{
	world.getPerson().position.y = world.getInterpolatedTerrainHeight(world.getPerson().position.x, world.getPerson().position.z) + .5f;
	camera.performRotations(partialTicks, frameDelta);

	renderer.prepare();
	renderer.render(partialTicks, camera);
}