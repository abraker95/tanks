#include "components/ViewController.h"

ViewController::ViewController(
		const sf::FloatRect& borders, const sf::FloatRect& viewport, 
		float minWidth, float maxWidth, float margin, std::vector<unsigned> focusedObjects)
: 
	borders(borders), 
	viewport(viewport), 
	minWidth(minWidth), 
	maxWidth(maxWidth), 
	margin(margin), 
	focusedObjects(focusedObjects),
	prevViewCenter(sf::Vector2f(0.f, 0.f)),
	prevViewSize(sf::Vector2f(0.f, 0.f)),
	prevRatio(0.f)
{
}
