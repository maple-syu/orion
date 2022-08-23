
#include "ParticleSystem.h"
#include <iostream>

particle_system::particle_system(sf::RenderWindow* window, const std::string& tex_file_name, const float& spawn_interval, const int& max_particles, const float& random_angle, const bool& loop, const particle_data& pd)
	: window_(window), active_(false), timer_(0), spawn_interval_(spawn_interval), max_particles_(max_particles),
	  loop_(loop),
	  random_angle_(random_angle)
{
	this->p_d_ = pd;
	if (!this->texture_.loadFromFile(tex_file_name))
	{
		throw std::exception("ERROR::PARTICLESYSTEM::COULD NOT LOAD TEXTURE");
	}
	this->direction_ = sf::Vector2f(0.f, -1.f);
	this->burst_delay_ = false;
	srand(time(nullptr));
}

particle_system::~particle_system()
{
}

void particle_system::update(const float& delta_time)
{
	if (this->active_)
	{
		if (this->spawn_interval_ == 0)
		{
			if (this->burst_delay_ == false)
			{
				while (this->particles_.size() < this->max_particles_)
				{
					spawn_particle();
				}
				if (this->loop_)
				{
					this->burst_delay_ = true;
				}else
				{
					this->active_ = false;
				}
			}
			else
			{
				if (this->particles_.empty())
				{
					this->burst_delay_ = false;
				}
			}
		}
		else
		{
			this->timer_ += delta_time;
			if (this->particles_.size() < this->max_particles_)
			{
				if (this->timer_ >= this->spawn_interval_)
				{
					this->timer_ -= this->spawn_interval_;
					spawn_particle();
				}
			}
			else
			{
				this->timer_ = this->spawn_interval_;
			}
		}
	}
	
	if (!this->particles_.empty())
	{
		for (unsigned i = 0; i<this->particles_.size(); i++)
		{
			if (this->particles_[i].is_active())
			{
				this->particles_[i].update(delta_time);
			}
			else
			{
				this->particles_.erase(this->particles_.begin() + i);
			}
		}
	}
}
void particle_system::render()
{
	for (auto& particle : this->particles_)
	{
		particle.render();
	}
}

void particle_system::rotate(const float& theta)
{
	const float n_x = (cosf(theta) - sinf(theta)) * this->direction_.x;
	const float n_y = (sinf(theta) + cosf(theta)) * this->direction_.y;
	this->direction_ = sf::Vector2f(n_x, n_y);
}

void particle_system::set_pos(const sf::Vector2f new_pos)
{
	this->position_ = new_pos;
}

void particle_system::set_dir(const sf::Vector2f& new_dir)
{
	this->direction_ = new_dir;
}

void particle_system::enable()
{
	this->active_ = true;
}

void particle_system::disable()
{
	this->active_ = false;
}

bool particle_system::is_active()
{
	return this->active_;
}

sf::Vector2f particle_system::random_rotation(const sf::Vector2f& start_dir, const float& angle_range)
{
	if (angle_range == 0)
	{
		return start_dir;
	}
	const int r = rand() % static_cast<int>(floor((angle_range)));
	const float r_angle = r - angle_range / 2.f;
	const float n_x = start_dir.x*cosf(r_angle*PI / 180) - start_dir.y*sinf(r_angle*PI / 180);
	const float n_y = start_dir.x*sinf(r_angle*PI / 180) + start_dir.y*cosf(r_angle*PI / 180);
	const sf::Vector2f n_dir(n_x, n_y);
	return n_dir;
}
void particle_system::spawn_particle()
{
	float spd;
	if (this->p_d_.max_speed != this->p_d_.min_speed)
	{
		spd = rand() % abs(static_cast<int>(floor(this->p_d_.max_speed - this->p_d_.min_speed))) + this->p_d_.min_speed;

	}else
	{
		spd = this->p_d_.min_speed;
	}
	this->particles_.emplace_back(this->window_, &this->texture_, this->position_, random_rotation(this->direction_, this->random_angle_), spd, spd + this->p_d_.delta_speed, this->p_d_);
	this->particles_.back().enable();
}
