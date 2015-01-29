#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "math/vector.h"
#include "components/transform.h"
#include "components/texture.h"
#include "managers/sprite_batch.h"

class SpriteBatch
{
public:
	SpriteBatch();
	~SpriteBatch();

	void add(const Transform& transform, const Texture& texture);
	void add(const Vec2f& pos, const float angle, const float scale, const sf::Texture* texture, const sf::IntRect& area);
	void flush();
	void setRenderTarget(sf::RenderTarget* render_target);

	void reset();

private:
	std::vector<sf::Vertex> vertex_array;
	unsigned vertex_count;
	sf::RenderTarget* render_target;
	sf::RenderStates states;
};
