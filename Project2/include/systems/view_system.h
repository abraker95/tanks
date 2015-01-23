#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "core/environment.h"

class ViewSystem : public System
{
public:
	void update(Environment* _env, sf::RenderWindow* window, float dt);

private:
	void getMinimalView(
		Environment* env, sf::Vector2f& _viewCenter, sf::Vector2f& _viewSize, 
		std::vector<unsigned>& focusedObjects);
	void correctAspectRatio(sf::Vector2f& _viewSize, float _margin, float _ratio);
	void viewSizeInRange(sf::Vector2f& _viewSize, float minWidth, float maxWidth, float ratio);
	void viewInBorders(sf::Vector2f& _viewCenter, sf::Vector2f& _viewSize, const sf::FloatRect& borders, float ratio);
	void smoothMovement(
		sf::Vector2f& _viewCenter, sf::Vector2f& _viewSize, float _elapsedTime, 
		sf::Vector2f& prevViewSize, sf::Vector2f& prevViewCenter, bool prevInit);
	
	const float cameraCooldown = 0.2f;
};
