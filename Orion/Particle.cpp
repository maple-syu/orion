

#include "Particle.h"

particle::particle(sf::RenderWindow* window, sf::Texture* tex, const sf::Vector2f& pos, const sf::Vector2f& dir, const float s_spd, const float e_spd, const particle_data& pd) :
	active_(false),
	timer_(0), start_speed_(s_spd), end_speed_(e_spd)
{
	this->window_ = window;
	this->body_.setTexture(*tex);
	this->body_.setPosition(pos);
	this->body_.setScale(sf::Vector2f(pd.start_size, pd.start_size));
	this->body_.setOrigin(this->body_.getLocalBounds().width / 2.f, this->body_.getLocalBounds().height / 2.f);
	this->direction_ = dir;
	this->p_d_ = pd;
	
}

particle::~particle()
{
}

void particle::update(const float& delta_time)
{
	if (this->active_)
	{
		this->timer_ += delta_time;
		auto t = this->timer_ / this->p_d_.lifetime;
		if (t >= 1.f)
		{
			this->active_ = false;
			t = 1.f;
		}

		const int r = static_cast<int>(this->p_d_.start_color.r + (this->p_d_.end_color.r - this->p_d_.start_color.r)*t);
		const int g = static_cast<int>(this->p_d_.start_color.g + (this->p_d_.end_color.g - this->p_d_.start_color.g)*t);
		const int b = static_cast<int>(this->p_d_.start_color.b + (this->p_d_.end_color.b - this->p_d_.start_color.b)*t);
		const int a = static_cast<int>(this->p_d_.start_color.a + (this->p_d_.end_color.a - this->p_d_.start_color.a)*t);
		this->body_.setColor(sf::Color(r, g, b, a));
		this->body_.setScale(sf::Vector2f(this->p_d_.start_size + (this->p_d_.end_size - this->p_d_.start_size)*t, this->p_d_.start_size + (this->p_d_.end_size - this->p_d_.start_size)*t));
		const float spd = this->start_speed_ + (this->end_speed_ - this->start_speed_)*t;
		this->body_.move(this->direction_.x * spd * delta_time, this->direction_.y * spd * delta_time);

	}
	if (this->timer_ >= this->p_d_.lifetime)
	{
		this->active_ = false;
	}

}

void particle::enable()
{
	this->active_ = true;
}

void particle::render()
{
	this->window_->draw(this->body_);
}

bool particle::is_active()
{
	return this->active_;
}

sf::Vector2f particle::get_pos()
{
	return this->body_.getPosition();
}

sf::Vector2f particle::get_dir()
{
	return this->direction_;
}