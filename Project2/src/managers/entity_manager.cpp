#define PI 3.14159f
#include "managers/entity_manager.h"
#include "Components.h"

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
}

unsigned EntityManager::spawnTankPlayer(std::string _name,
	Environment* env, TextureManager* tex_man, 
	float x, float y, 
	std::array<sf::Keyboard::Key, 5> keys)
{
	unsigned new_tank = env->createEntity(_name,
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

unsigned EntityManager::spawnBullet(std::string _name,
	Environment* env, TextureManager* tex_man, 
	unsigned tank_id)
{
	auto transform = env->get<Transform>();
	auto bounding_circle = env->get<BoundingCircle>();
	auto sprite = env->get<Sprite>();
	auto texture = env->get<Texture>();

	if(!env->hasComponents<Transform, BoundingCircle>(tank_id))
		return 0;
	
	float start_x = 
		transform[tank_id].x + cosf(transform[tank_id].rot * PI/180.f + PI/2.f) * bounding_circle[tank_id].radius;
	float start_y = 
		transform[tank_id].y + sinf(transform[tank_id].rot * PI/180.f + PI/2.f)  * bounding_circle[tank_id].radius;

	unsigned new_bullet = env->createEntity(_name,
		new Transform(start_x, start_y, transform[tank_id].rot),
		new Velocity(500.f, 0.f),
		new Texture(tex_man->load("Bullet_0.png")),
		new Expires(5.f),
		new Projectile(-20, tank_id),
		new BoundingCircle(),
		new Sprite()
	);

	
	if(texture[new_bullet].texture)
	{
		sf::Vector2u size = texture[new_bullet].texture->getSize();

		sprite[new_bullet].sprite.setOrigin((float)size.x/2.f, (float)size.y/2.f);	
		bounding_circle[new_bullet].radius = size.x < size.y ? (float)size.x/2.f : (float)size.y/2.f;
	}

	return new_bullet;
}

unsigned EntityManager::createCamera(std::string _name,
	Environment* env, 
	sf::FloatRect& borders, sf::FloatRect& viewport, 
	std::vector<unsigned> focusedObjects)
{
	return env->createEntity(_name,
		new ViewController(borders, viewport, 400.f, 1200.f, 0.4f, focusedObjects)
	);
}
