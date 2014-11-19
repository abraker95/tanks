#pragma once
#include "Tank.h"
#include "Tilemap.h"
#include "GameObject.h"
#include "Camera.h"
using namespace sf;

class GameScene
{
	friend class Environment;

public:

	GameScene();
	virtual ~GameScene();

private:
	Tilemap tilemap;
	std::vector<GameObject*> objects;
	std::vector<Camera*> cameras;

	void setTilemap(Tilemap& _tilemap) { tilemap = _tilemap; }
	void addCamera(Camera* _cam) { cameras.push_back(_cam); }
	void addObject(GameObject* _obj){ objects.push_back(_obj); }
};

