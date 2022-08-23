

#ifndef TRAIL_H
#define TRAIL_H
#include "ParticleSystem.h"

class trail
{
public:
	trail(sf::RenderWindow* window, sf::Texture* tex, 
		const sf::Vector2f& line_start, const sf::Vector2f& line_end);
	~trail();

	void update(const float& delta_time);
	void render();

	bool get_active() const;

private:
	sf::RenderWindow* window_;
	sf::Texture* texture_;
	sf::Sprite trail_;
	particle_data pd_;
	particle_system particlesystem_;

	float max_timer_ = .3f;
	float trail_timer_ = 0.f;
	bool active_;
};
#endif

