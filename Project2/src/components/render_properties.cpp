#include "components/render_properties.h"

RenderProperties::RenderProperties()
{
}

RenderProperties::RenderProperties(sf::Texture* texture)
{
	sprite.setTexture(*texture);
}
