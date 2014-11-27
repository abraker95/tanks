#include "Line.h"

Line::Line(const sf::Vector2f& pt1, const sf::Vector2f& pt2, float thick)
{
	point[0] = pt1-sf::Vector2f((thick/2.0f), (thick/2.0f));
	point[1] = pt1+sf::Vector2f((thick/2.0f), (thick/2.0f));
	point[2] = pt2+sf::Vector2f((thick/2.0f), (thick/2.0f));
	point[3] = pt2-sf::Vector2f((thick/2.0f), (thick/2.0f));
	setFillColor(sf::Color(255, 255, 255, 255));
	setOutlineColor(sf::Color(255, 255,255, 255));
}

Line::~Line() {}

unsigned int Line::getPointCount() const
{
	return 4;
}

sf::Vector2f Line::getPoint(unsigned int index) const
{
	return point[index];
}