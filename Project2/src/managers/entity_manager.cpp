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
	Environment* _gameEnv, TextureManager* tex_man, 
	float x, float y, 
	std::array<sf::Keyboard::Key, 5> keys)
{
	auto sprite = _gameEnv->get<Sprite>();

	sf::Texture* texture = tex_man->load("res/Tank_0.png");
	Vec2f origin;

	Vec2u size = texture->getSize();
	if(texture)
	{
		origin.x = (float)size.x/2.f; 
		origin.y = (float)size.y/2.f;
	}

	unsigned new_tank = _gameEnv->createEntity(_name,
		new Transform(Vec2f(x, y), 0.f),
		new Velocity(0.f, 0.f),
		new Texture(texture),
		new TankControls(keys),
		new BoundingBox(Vec2f((float)size.x, (float)size.y)),
		new Health(100, 100),
		new Gun(),
		new Sprite(),
		new Solid(),
		new Label("")
	);

	if(_gameEnv->getID(_name+"Score") == 0) // only if the tank doesn't have a respective score
		_gameEnv->createEntity(_name+"Score", new Score(0, 0));

	auto label = _gameEnv->get<Label>();
	if(!label[new_tank].font.loadFromFile("res/arial.ttf")) cout<<"ERROR: FONT NOT FOUND"<<endl;
	label[new_tank].label.setFont(label[new_tank].font);
	label[new_tank].label.setCharacterSize(18);

	sprite[new_tank].sprite.setOrigin(origin.x, origin.y);
	IDs.push_back(new_tank);
	return new_tank;
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
	IDs.push_back(new_bullet);
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

	IDs.push_back(new_view);
	return new_view;
}

void EntityManager::NewGame(Environment* _gameEnv, TextureManager* _textmgr)
{
	// double-braces init because of std::array
	std::array<sf::Keyboard::Key, 5> p1_keys = {{sf::Keyboard::Right, sf::Keyboard::Left, sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Space}};
	unsigned tank1 = spawnTankPlayer("tank1", _gameEnv, _textmgr, 200.f, 300.f, p1_keys);

	std::array<sf::Keyboard::Key, 5> p2_keys = {{sf::Keyboard::D, sf::Keyboard::A, sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::F}};
	unsigned tank2 = spawnTankPlayer("tank2", _gameEnv, _textmgr, 400.f, 300.f, p2_keys);

	// camera
	sf::FloatRect borders = sf::FloatRect(0.f, 0.f, 64.f * 20.f, 64.f * 20.f);
	sf::FloatRect viewport = sf::FloatRect(0.f, 0.f, 1.f, 1.f);
	createCamera("mainCamera", _gameEnv, borders, viewport, {tank1, tank2});
}

void EntityManager::EndGame(Environment* _gameEnv, bool _newScore)
{
	_gameEnv->destroyEntity(_gameEnv->getID("mainCamera"));

	if(_newScore)
	{
		for(unsigned player = 1; player<=2; player++)
		{
			unsigned ID = _gameEnv->getID("tank"+to_string(player)+"Score");
			_gameEnv->destroyEntity(ID);
		}
	}

	for(unsigned player = 1; player<=2; player++)
	{
		unsigned ID = _gameEnv->getID("tank"+to_string(player));
		_gameEnv->destroyEntity(ID);
	}
}

void EntityManager::ResetGame(Environment* _gameEnv, TextureManager* _textmgr, bool _newScore)
{
	EndGame(_gameEnv, _newScore);
	NewGame(_gameEnv, _textmgr);
}
