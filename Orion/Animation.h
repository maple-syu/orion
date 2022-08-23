

#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>
#include <iostream>

struct curve
{
	sf::Vector2f p0, p1, p2, p3;
	curve() = default;
	curve(const sf::Vector2f& p0, const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Vector2f& p3)
	{
		this->p0 = p0;
		this->p1 = p1;
		this->p2 = p2;
		this->p3 = p3;
	}
};

struct curve;
class animation
{
public:
	animation();
	animation(const curve& curve);
	~animation();

	sf::Vector2f evaluate_curve(const float& t);
	sf::Vector2f evaluate_curve(const sf::Vector2f& p0, const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Vector2f& p3, const float& t);
	sf::Vector2f evaluate_curve(const curve& curve, const float& t);

private:
	curve curve_;
};



#endif
