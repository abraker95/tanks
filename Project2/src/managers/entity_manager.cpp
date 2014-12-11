#include "managers/entity_manager.h"
#include "Components.h"

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
}

unsigned EntityManager::spawnTankPlayer(
	Environment* env, TextureManager* tex_man, 
	float x, float y, 
	std::array<sf::Keyboard::Key, 5> keys)
{
	unsigned new_tank = env->createEntity(
		new Transform(x, y, 0.f),
		new Velocity(0.f, 0.f),
		new Texture(tex_man->load("Tank_0.png")),
		new TankControls(keys),
		new BoundingCircle(),
		new Gun(),
		new Sprite()
	);

	auto sprite = env->get<Sprite>();
	auto texture = env->get<Texture>();
	auto bounding_circle = env->get<BoundingCircle>();

	if(texture[new_tank].texture)
	{
		sf::Vector2u size = texture[new_tank].texture->getSize();

		sprite[new_tank].sprite.setOrigin((float)size.x/2.f, (float)size.y/2.f);	
		bounding_circle[new_tank].radius = size.x < size.y ? (float)size.x/2.f : (float)size.y/2.f;
	}

	return new_tank;
}

unsigned EntityManager::spawnBullet(
	Environment* env, TextureManager* tex_man, 
	unsigned tank_id)
{
	auto transform = env->get<Transform>();

	unsigned new_bullet = env->createEntity(
		new Transform(transform[tank_id].x, transform[tank_id].y, transform[tank_id].rot),
		new Velocity(500.f, 0.f),
		new Texture(tex_man->load("Bullet_0.png")),
		new Expires(5.f),
		new Projectile(-20, tank_id),
		new BoundingCircle()
	);

	auto sprite = env->get<Sprite>();
	auto texture = env->get<Texture>();
	auto bounding_circle = env->get<BoundingCircle>();

	if(texture[new_bullet].texture)
	{
		sf::Vector2u size = texture[new_bullet].texture->getSize();

		sprite[new_bullet].sprite.setOrigin((float)size.x/2.f, (float)size.y/2.f);	
		bounding_circle[new_bullet].radius = size.x < size.y ? (float)size.x/2.f : (float)size.y/2.f;
	}

	return new_bullet;
}

unsigned EntityManager::createCamera(
	Environment* env, 
	sf::FloatRect& borders, sf::FloatRect& viewport, 
	std::vector<unsigned> focusedObjects)
{
	return env->createEntity(
		new ViewController(borders, viewport, 400.f, 1200.f, 0.4f, focusedObjects)
	);
}
