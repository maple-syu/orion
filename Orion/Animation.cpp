

#include "Animation.h"

animation::animation()
{
	this->curve_ = curve();
}

animation::animation(const curve& curve)
{
	this->curve_ = curve;
}


animation::~animation()
{
}

sf::Vector2f animation::evaluate_curve(const float& t)
{

	const float ax = this->curve_.p0.x + (this->curve_.p1.x - this->curve_.p0.x)*t;
	const float ay = this->curve_.p0.y + (this->curve_.p1.y - this->curve_.p0.y)*t;
	const float bx = this->curve_.p1.x + (this->curve_.p2.x - this->curve_.p1.x)*t;
	const float by = this->curve_.p1.y + (this->curve_.p2.y - this->curve_.p1.y)*t;
	const float cx = this->curve_.p2.x + (this->curve_.p3.x - this->curve_.p2.x)*t;
	const float cy = this->curve_.p2.y + (this->curve_.p3.y - this->curve_.p2.y)*t;

	const float dx = ax + (bx - ax)*t;
	const float dy = ay + (by - ay)*t;
	const float ex = bx + (cx - bx)*t;
	const float ey = by + (cy - by)*t;

	const sf::Vector2f p(dx + (ex - dx)*t, dy + (ey - dy)*t);
	return p;

}

sf::Vector2f animation::evaluate_curve(const sf::Vector2f& p0, 
	const sf::Vector2f& p1, 
	const sf::Vector2f& p2, 
	const sf::Vector2f& p3,
    const float& t)
{
	const sf::Vector2f q0(p0.x + (p1.x - p0.x)*t, p0.y + (p1.y - p0.y)*t);
	const sf::Vector2f q1(p1.x + (p2.x - p1.x)*t, p1.y + (p2.y - p1.y)*t);
	const sf::Vector2f q2(p2.x + (p3.x - p2.x)*t, p2.y + (p3.y - p2.y)*t);

	const sf::Vector2f r0(q0.x + (q1.x - q0.x)*t, q0.y + (q1.y - q0.y)*t);
	const sf::Vector2f r1(q1.x + (q2.x - q1.x)*t, q1.y + (q2.y - q1.y)*t);

	const sf::Vector2f c(r0.x + (r1.x - r0.x)*t, r0.y + (r1.y - r0.y)*t);
	return c;
}


sf::Vector2f animation::evaluate_curve(const curve& curve, const float& t)
{
	const sf::Vector2f q0(curve.p0.x + (curve.p1.x - curve.p0.x)*t, curve.p0.y + (curve.p1.y - curve.p0.y)*t);
	const sf::Vector2f q1(curve.p1.x + (curve.p2.x - curve.p1.x)*t, curve.p1.y + (curve.p2.y - curve.p1.y)*t);
	const sf::Vector2f q2(curve.p2.x + (curve.p3.x - curve.p2.x)*t, curve.p2.y + (curve.p3.y - curve.p2.y)*t);

	const sf::Vector2f r0(q0.x + (q1.x - q0.x)*t, q0.y + (q1.y - q0.y)*t);
	const sf::Vector2f r1(q1.x + (q2.x - q1.x)*t, q1.y + (q2.y - q1.y)*t);

	const sf::Vector2f c(r0.x + (r1.x - r0.x)*t, r0.y + (r1.y - r0.y)*t);
	return c;

}
