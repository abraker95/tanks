#include <iostream>
#include "managers/sprite_batch.h"

SpriteBatch::SpriteBatch()
{
	vertex_count = 0;
	render_target = nullptr;
}

SpriteBatch::~SpriteBatch()
{
}

void SpriteBatch::setRenderTarget(sf::RenderTarget* render_target)
{
	this->render_target = render_target;
}

void SpriteBatch::add(const Transform& transform, const Texture& texture)
{
	add(transform.pos, transform.rot, transform.scale, texture.texture, texture.rect);
}

void SpriteBatch::add(const Vec2f& pos, const float angle, const float scale, const sf::Texture* texture, const sf::IntRect& rect)
{
	if(states.texture != texture)
	{
		flush();
		states.texture = texture;
	}

	if(vertex_count + 4 > vertex_array.size())
	{
		vertex_array.resize(vertex_count + 4);
	}

	sf::Transform transform;
	transform.translate(pos.x, pos.y);
	transform.rotate(angle);
	transform.scale(scale, scale);

	vertex_array[vertex_count].position = transform.transformPoint(-(float)rect.width/2.f, -(float)rect.height/2.f);
	vertex_array[vertex_count].texCoords= sf::Vector2f((float)rect.left, (float)rect.top);

	vertex_array[vertex_count+1].position = transform.transformPoint((float)rect.width/2.f, -(float)rect.height/2.f);
	vertex_array[vertex_count+1].texCoords = sf::Vector2f((float)(rect.left + rect.width), (float)rect.top);
	vertex_array[vertex_count+2].position = transform.transformPoint((float)rect.width/2.f, (float)rect.height/2.f);
	vertex_array[vertex_count+2].texCoords = sf::Vector2f((float)(rect.left + rect.width), (float)(rect.top + rect.height));
	vertex_array[vertex_count+3].position = transform.transformPoint(-(float)rect.height/2.f, (float)rect.height/2.f);
	vertex_array[vertex_count+3].texCoords = sf::Vector2f((float)rect.left, (float)(rect.top + rect.height));

	vertex_count += 4;
}

void SpriteBatch::flush()
{
	if(render_target && vertex_count != 0)
	{
		render_target->draw(&vertex_array[0], vertex_count, sf::Quads, states);
		vertex_count = 0;
	}

	else
	{
		std::cout<<"SpriteBatch: use setRenderTarget() before flush()"<<std::endl;
	}
}

void SpriteBatch::reset()
{
	vertex_count = 0;
	vertex_array.clear();
}
