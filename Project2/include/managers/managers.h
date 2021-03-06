#pragma once

#include "managers/entity_manager.h"
#include "managers/texture_manager.h"
#include "managers/map_loader.h"
#include "managers/InputControls_Mgr.h"
#include "managers/UI_Manager.h"
#include "managers/font_manager.h"
#include "managers/score_manager.h"
#include "managers/net_manager.h"
#include "managers/sprite_batch.h"
#include "managers/game_manager.h"

struct Managers
{
	EntityManager entity_manager;
	TextureManager texture_manager;
	MapLoader map_loader;
	UI_Manager UI_manager;
	FontManager font_manager;
	ScoreManager score_manager;
	GameManager game_manager;
	NetManager net_manager;
	SpriteBatch sprite_batch;
};
