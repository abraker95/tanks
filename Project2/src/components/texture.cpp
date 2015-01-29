#include "components/texture.h"

Texture::Texture(sf::Texture* texture) : texture(texture)
{
	Vec2u size = texture->getSize();
	rect = sf::IntRect(0, 0, size.x, size.y);
}

Texture::Texture(sf::Texture* texture, const sf::IntRect& rect) : texture(texture), rect(rect)
{
}
