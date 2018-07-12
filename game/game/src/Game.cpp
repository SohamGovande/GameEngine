#include "Game.h"
#include "Toolkit.h"
#include "TimedScope.h"

Game::Game()
	: resourceMgr(),
	componentRegistry(),
	entityRegistry(componentRegistry, resourceMgr),

	camera(),
	renderer(60, 0.1f, 1000, resourceMgr),
	world(resourceMgr),

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

	std::mt19937 engine;
	std::uniform_real_distribution<float> distribution(-TERRAIN_SIZE, TERRAIN_SIZE);

	for (unsigned int i = 0; i < 3; i++)
	{
		float x = distribution(engine);
		float z = distribution(engine);
		float y = world.getInterpolatedTerrainHeight(x, z);
		
		Entity& entity = world.emplaceEntity();
		entityRegistry.getConstructor("lantern").construct(entity);
		entity.position = glm::vec3(x, y + 10, z);
		entity.scale = 2;

		renderer.getLights().push_back(Light(
			glm::vec3(x, y + 50, z),
			glm::vec3(1, 0, 0),
			glm::vec3(0.01, 0, 0.001)
		));
	}

	Toolkit::hideCursor();
}

void Game::tick()
{
	world.tick();
	camera.tick(world);
}

void Game::render(const float partialTicks)
{
	world.getPerson().position.y = world.getInterpolatedTerrainHeight(world.getPerson().position.x, world.getPerson().position.z) + .5f;
	camera.performRotations(partialTicks);

	renderer.prepare();
	renderer.render(partialTicks, camera);
}