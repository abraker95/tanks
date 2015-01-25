#define PI 3.14159f
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
	auto sprite = _gameEnv->get<Sprite>();
	auto transform = _gameEnv->get<Sprite>();

	sf::Texture* texture = texture_manager->load("res/Tank_0.png");
	Vec2f origin;

	Vec2u size = texture->getSize();
	if(texture)
	{
		origin.x = (float)size.x/2.f; 
		origin.y = (float)size.y/2.f;
	}

	unsigned new_tank = _gameEnv->createEntity(_name,
		new Transform(Vec2f(0.f, 0.f), 0.f),
		new Velocity(0.f, 0.f),
		new Texture(texture),
		new TankControls(keys),
		new BoundingBox(Vec2f((float)size.x, (float)size.y)),
		new Health(100, 100),
		new Gun(),
		new Sprite(),
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

	sprite[new_tank].sprite.setOrigin(origin.x, origin.y);
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
	auto sprite = _gameEnv->get<Sprite>();

	if(!_gameEnv->hasComponents<Transform, BoundingBox>(tank_id))
		return 0;
	
	float start_x = 
		transform[tank_id].pos.x + cosf(transform[tank_id].rot * PI/180.f + PI/2.f) * bounding_box[tank_id].size.x/2.5f;
	float start_y = 
		transform[tank_id].pos.y + sinf(transform[tank_id].rot * PI/180.f + PI/2.f)  * bounding_box[tank_id].size.x/2.5f;
	
	sf::Texture* texture = tex_man->load("res/Bullet_0.png");
	Vec2f origin;

	Vec2u size = texture->getSize();
	if(texture)
	{
		origin.x = (float)size.x/2.f; 
		origin.y = (float)size.y/2.f;
	}

	unsigned new_bullet = _gameEnv->createEntity(_name,
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

	for(unsigned spawn_id = 0;spawn_id < env->maxEntities();spawn_id++)
	{
		if(env->hasComponents<SpawnLocation>(spawn_id) && spawnLocation[spawn_id].player_id == (int)player_id)
		{
			transform[tank_id].pos = transform[spawn_id].pos;
			transform[tank_id].rot = 0.f;
			break;
		}
	}
}
