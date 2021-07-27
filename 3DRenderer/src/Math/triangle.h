#pragma once

#include "vector.h"
#include "SFML/Graphics/Color.hpp"

struct Triangle
{
	Vec3f v[3];								/* 3 triangle points */
	Vec3f normal;							/* triangle normal vector */
	sf::Color color = sf::Color::White;		/* default color */
};



