#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Line: public Shape
{
	public:
		Line(const sf::Vector2f& pt1, const sf::Vector2f& pt2, float thick = 1.0f);
		virtual ~Line();

		unsigned int getPointCount() const;
		Vector2f getPoint(unsigned int index) const;

	private:
		Vector2f point[4];
};

