#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "core/environment.h"

struct ViewController : public Component<ViewController>
{
	ViewController();
	ViewController(
		const sf::FloatRect& borders, const sf::FloatRect& viewport, float ratio, 
		float minWidth, float maxWidth, float margin, std::vector<unsigned> focusedObjects);

	std::vector<unsigned> focusedObjects;
	sf::FloatRect borders;
	sf::FloatRect viewport;
	float ratio;
	float minWidth, maxWidth;
	float margin;
	sf::Vector2f prevViewSize, prevViewCenter; // previous view center and view size
	bool prevInit;
	sf::View view;
};
