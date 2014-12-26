#pragma once
#include "core/environment.h"

struct CollisionEvent : public Event<CollisionEvent>
{
	CollisionEvent(unsigned _entity1, unsigned _entity2) : entity1(_entity1), entity2(_entity2) {}

	unsigned entity1;
	unsigned entity2;
};

struct DestroyEvent : public Event<DestroyEvent>
{
	DestroyEvent(unsigned _entity) : entity(_entity) {}

	unsigned entity;
};

struct MenuEvent: public Event<MenuEvent>
{
	MenuEvent() {}
};

struct WindowModeEvent: public Event<WindowModeEvent>
{
	WindowModeEvent(bool* _fullscreen): fullscreen(_fullscreen) {}

	bool* fullscreen;
};
