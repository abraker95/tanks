#include "components/ViewController.h"

ViewController::ViewController(
		const sf::FloatRect& borders, const sf::FloatRect& viewport, float ratio, 
		float minWidth, float maxWidth, float margin, std::vector<unsigned> focusedObjects)
: 
	borders(borders), 
	viewport(viewport), 
	minWidth(minWidth), 
	maxWidth(maxWidth), 
	margin(margin), 
	focusedObjects(focusedObjects),
	prevViewCenter(sf::Vector2f(0.f, 0.f)),
	prevInit(false)
{
	this->ratio = ratio * viewport.width/viewport.height;
	this->prevViewSize = sf::Vector2f(minWidth, minWidth / this->ratio);
}
