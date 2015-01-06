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

	IDs.push_back(new_tank);
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
	IDs.push_back(new_bullet);
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

void EntityManager::NewGame(Environment* _env, TextureManager* _textmgr)
{
	// double-braces init because of std::array
	std::array<sf::Keyboard::Key, 5> p1_keys = {{sf::Keyboard::Right, sf::Keyboard::Left, sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Space}};
	unsigned tank1 = spawnTankPlayer("tank1", _env, _textmgr, 200.f, 300.f, p1_keys);

	std::array<sf::Keyboard::Key, 5> p2_keys = {{sf::Keyboard::D, sf::Keyboard::A, sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::F}};
	unsigned tank2 = spawnTankPlayer("tank2", _env, _textmgr, 400.f, 300.f, p2_keys);

	// camera
	sf::FloatRect borders = sf::FloatRect(0.f, 0.f, 64.f * 20.f, 64.f * 20.f);
	sf::FloatRect viewport = sf::FloatRect(0.f, 0.f, 1.f, 1.f);
	createCamera("mainCamera", _env, borders, viewport, {tank1, tank2});
}

void EntityManager::EndGame(Environment* _env)
{
	_env->destroyEntity(_env->getID("tank1"));
	_env->destroyEntity(_env->getID("tank2"));
	_env->destroyEntity(_env->getID("mainCamera"));
}

void EntityManager::ResetGame(Environment* _env, TextureManager* _textmgr)
{
	EndGame(_env);
	NewGame(_env, _textmgr);
}
