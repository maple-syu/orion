

#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

//#include <SFML/Graphics.hpp>
#include "Particle.h"
#include <random>
#include <functional>

#define PI 3.14159265f

class particle_system
{
public:
	particle_system(sf::RenderWindow* window, const std::string& tex_file_name, const float& spawn_interval, const int& max_particles, const float& random_angle, const bool& loop, const particle_data& pd);
	~particle_system();

	void update(const float& delta_time);
	void render();
	void rotate(const float& theta);
	void set_pos(sf::Vector2f new_pos);
	void set_dir(const sf::Vector2f& new_dir);
	void enable();
	void disable();

	bool is_active();

private:

	sf::RenderWindow* window_;
	sf::Texture texture_;
	sf::Vector2f position_;
	sf::Vector2f direction_;
	bool active_;
	bool burst_delay_;
	float timer_;
	float spawn_interval_;
	unsigned max_particles_;
	bool loop_;

	particle_data p_d_;
	std::vector<particle> particles_;
	float random_angle_;
	sf::Vector2f random_rotation(const sf::Vector2f& start_dir, const float& angle_range);
	void spawn_particle();
};
#endif