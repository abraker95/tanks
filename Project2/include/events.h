#pragma once
#include "core/environment.h"

struct CollisionEvent : public Event<CollisionEvent>
{
	CollisionEvent(unsigned _entity1, unsigned _entity2) : entity1(_entity1), entity2(_entity2) {}
	unsigned entity1, entity2;
};

struct DestroyEvent : public Event<DestroyEvent>
{
	DestroyEvent(unsigned _entity) : entity(_entity) {}
	unsigned entity;
};

struct ScoreEvent: public Event<ScoreEvent>
{
	ScoreEvent(unsigned _winner, unsigned _losser): winner(_winner), losser(_losser) {}
	unsigned winner, losser;
};

struct WindowModeEvent: public Event<WindowModeEvent>
{
	WindowModeEvent(bool* _fullscreen): fullscreen(_fullscreen) {}
	bool *fullscreen; // needs to be pointer to pass value across systems and the UI Manager
};

struct NewGameEvent: public Event<NewGameEvent>
{
	NewGameEvent(bool _resetScores): resetScores(_resetScores) {}
	bool resetScores;
};

struct ResizeEvent : public Event<ResizeEvent>
{
	ResizeEvent(int width, int height) : width(width), height(height) {}
	int width, height;
};
