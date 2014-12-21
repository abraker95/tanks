#define PI 3.14159f
#include "math/vector.h"
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
	auto sprite = env->get<Sprite>();

	sf::Texture* texture = tex_man->load("res/Tank_0.png");
	Vec2f origin;

	Vec2u size = texture->getSize();
	if(texture)
	{

		origin.x = (float)size.x/2.f; 
		origin.y = (float)size.y/2.f;
	}

	unsigned new_tank = env->createEntity(_name,
		new Transform(Vec2f(x, y), 0.f),
		new Velocity(0.f, 0.f),
		new Texture(texture),
		new TankControls(keys),
		new BoundingBox(Vec2f((float)size.x, (float)size.y)),
		new Health(100, 100),
		new Gun(),
		new Sprite(),
		new Solid()
	);

	sprite[new_tank].sprite.setOrigin(origin.x, origin.y);

	return new_tank;
}

unsigned EntityManager::spawnBullet(std::string _name,
	Environment* env, TextureManager* tex_man, 
	unsigned tank_id)
{
	auto transform = env->get<Transform>();
	auto bounding_box = env->get<BoundingBox>();
	auto sprite = env->get<Sprite>();

	if(!env->hasComponents<Transform, BoundingBox>(tank_id))
		return 0;
	
	float start_x = 
		transform[tank_id].pos.x + cosf(transform[tank_id].rot * PI/180.f + PI/2.f) * bounding_box[tank_id].size.x/2.f;
	float start_y = 
		transform[tank_id].pos.y + sinf(transform[tank_id].rot * PI/180.f + PI/2.f)  * bounding_box[tank_id].size.x/2.f;
	
	sf::Texture* texture = tex_man->load("res/Bullet_0.png");
	Vec2f origin;

	Vec2u size = texture->getSize();
	if(texture)
	{
		origin.x = (float)size.x/2.f; 
		origin.y = (float)size.y/2.f;
	}

	unsigned new_bullet = env->createEntity(_name,
		new Transform(Vec2f(start_x, start_y), transform[tank_id].rot),
		new Velocity(500.f, 0.f),
		new Texture(texture),
		new Expires(5.f),
		new Projectile(20, tank_id),
		new BoundingBox(Vec2f((float)size.x, (float)size.y)),
		new Solid(),
		new Sprite()
	);

	
	sprite[new_bullet].sprite.setOrigin(origin.x, origin.y);

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
