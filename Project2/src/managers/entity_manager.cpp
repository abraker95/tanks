#define PI 3.14159f
#include <ctime>
#include "math/vector.h"
#include "managers/entity_manager.h"
#include "events.h"
#include "Components.h"

EntityManager::EntityManager()
{
	numLivingTanks = 0;
}

EntityManager::~EntityManager()
{
}

unsigned EntityManager::spawnTankPlayer(std::string _name,
	Environment* _gameEnv, TextureManager* texture_manager,
	ScoreManager* score_manager, std::array<sf::Keyboard::Key, 5> keys)
{
	auto transform = _gameEnv->get<Sprite>();

	sf::Texture* texture = texture_manager->load("res/Tank_0.png");
	Vec2u size = texture->getSize();

	unsigned new_tank = _gameEnv->createEntity(_name,
		new Transform(Vec2f(0.f, 0.f), 0.f),
		new Velocity(0.f, 0.f),
		new Texture(texture),
		new TankControls(keys),
		new BoundingBox(Vec2f((float)size.x, (float)size.y)),
		new Health(100, 100),
		new Gun(),
		new Solid(),
		new Player(score_manager->newPlayer()),
		new Explosible(),
		new Label("")
	);

	if(_gameEnv->getID(_name+"Score") == 0) // only if the tank doesn't have a respective score
		_gameEnv->createEntity(_name+"Score", new Score(0, 0));

	auto label = _gameEnv->get<Label>();
	if(!label[new_tank].font.loadFromFile("res/arial.ttf")) cout<<"ERROR: FONT NOT FOUND"<<endl;
	label[new_tank].label.setFont(label[new_tank].font);
	label[new_tank].label.setCharacterSize(18);

	numLivingTanks++;

	auto player = _gameEnv->get<Player>();
	placeOnSpawn(_gameEnv, new_tank, player[new_tank].player_id);

	return new_tank;
}

void EntityManager::killPlayer(Environment* env, unsigned id)
{
	env->disableComponents<TankControls, Solid, Texture>(id);
	numLivingTanks--;
}

void EntityManager::revivePlayer(Environment* env, unsigned id)
{
	env->enableComponents<TankControls, Solid, Texture>(id);
	numLivingTanks++;
}

int EntityManager::getNumLivingTanks()
{
	return numLivingTanks;
}

unsigned EntityManager::spawnBullet(std::string _name,
	Environment* _gameEnv, TextureManager* tex_man,
	unsigned tank_id)
{
	auto transform = _gameEnv->get<Transform>();
	auto bounding_box = _gameEnv->get<BoundingBox>();

	if(!_gameEnv->hasComponents<Transform, BoundingBox>(tank_id))
		return 0;
	
	float start_x = 
		transform[tank_id].pos.x + cosf(transform[tank_id].rot * PI/180.f + PI/2.f) * bounding_box[tank_id].size.x/2.5f;
	float start_y = 
		transform[tank_id].pos.y + sinf(transform[tank_id].rot * PI/180.f + PI/2.f)  * bounding_box[tank_id].size.x/2.5f;
	
	sf::Texture* texture = tex_man->load("res/Bullet_0.png");
	Vec2u size = texture->getSize();

	unsigned new_bullet = _gameEnv->createEntity(_name,
		new Transform(Vec2f(start_x, start_y), transform[tank_id].rot),
		new Velocity(500.f, 0.f),
		new Texture(texture),
		new Expires(5.f),
		new Projectile(20, tank_id),
		new BoundingBox(Vec2f((float)size.x, (float)size.y)),
		new Solid()
	);

	return new_bullet;
}

unsigned EntityManager::createCamera(std::string _name,
	Environment* _gameEnv,
	sf::FloatRect& borders, sf::FloatRect& viewport, 
	std::vector<unsigned> focusedObjects)
{
	
	unsigned int new_view = _gameEnv->createEntity(_name,
		new ViewController(borders, viewport, 400.f, 1200.f, 0.4f, focusedObjects)
	);

	return new_view;
}

void EntityManager::placeOnSpawn(Environment* env, unsigned tank_id, unsigned player_id)
{
	auto spawnLocation = env->get<SpawnLocation>();
	auto transform = env->get<Transform>();

	// get all the free spawn locations
	std::vector<unsigned> free_spawns;
	for(unsigned spawn_id = 0;spawn_id < env->maxEntities();spawn_id++)
	{
		if(env->hasComponents<SpawnLocation>(spawn_id) && spawnLocation[spawn_id].occupied == false)
		{
			free_spawns.push_back(spawn_id);
		}
	}

	if(free_spawns.size() > 0)
	{
		// pick a random spawn
		int spawn_choice = (rand() % (int)free_spawns.size());
		spawnLocation[free_spawns[spawn_choice]].occupied = true;
		transform[tank_id].pos = transform[free_spawns[spawn_choice]].pos;
		transform[tank_id].rot = 0.f;
	}
}

void EntityManager::spawnExplosion(Environment* env, TextureManager* tex_man, Vec2f pos)
{
	sf::Texture* explosion_center = tex_man->load("res/explosion_center.png");
	sf::Texture* smoke = tex_man->load("res/smoke.png");

	env->createEntity("",
		new Transform(pos, 0.f, 2.f),
		new Texture(explosion_center),
		new Expires(0.2f),
		new Velocity(0.f, 0.f, 8.f)
	);

	for(int i=0;i<10;i++)
	{
		env->createEntity("",
			new Transform(pos, (float)(rand()%360), 4.f),
			new Texture(smoke),
			new Expires(0.4f),
			new Velocity(300.f, (float)(rand()%360), -10.f)
		);
	}
}
