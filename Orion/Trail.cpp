
#include "Trail.h"

trail::trail(sf::RenderWindow* window, sf::Texture* tex, const sf::Vector2f& line_start, const sf::Vector2f& line_end)
	: window_(window), texture_(tex),
	pd_(.3f, .1f, .05f, 300.f, 600.f, 50.f, sf::Color(255,255,255,255), sf::Color(255,255,255,0)),
	particlesystem_(window, "Resources/Sprites/particle_alpha.png", 0, 15, 90, false, this->pd_), active_(true)
{
	this->trail_.setTexture(*this->texture_);
	this->trail_.setOrigin(this->trail_.getLocalBounds().width / 2.f, this->trail_.getLocalBounds().height);
	this->trail_.setPosition(line_start);
	this->particlesystem_.set_pos(line_end);
	sf::Vector2f trail_dir(line_end.x - line_start.x, line_end.y - line_start.y);
	const float magnitude = sqrtf(powf(trail_dir.x, 2.f) + powf(trail_dir.y, 2.f));
	trail_dir.x /= magnitude;
	trail_dir.y /= magnitude;
	this->particlesystem_.set_dir(sf::Vector2f(trail_dir.x*-1.f, trail_dir.y*-1.f));
	this->trail_.setScale(.8f, magnitude/this->trail_.getLocalBounds().height);
	float theta = acosf(0.f* trail_dir.x + -1.f * trail_dir.y) * (180 / PI);
	if (trail_dir.x < 0.f)
	{
		theta *= -1.f;
	}
	this->trail_.rotate(theta);
	this->particlesystem_.enable();
}

trail::~trail()
{
	
}

void trail::update(const float& delta_time)
{
	this->particlesystem_.update(delta_time);
	this->trail_timer_ += delta_time;
	if (this->trail_timer_ < this->max_timer_)
	{
		const float t = this->trail_timer_ / this->max_timer_;
		int alpha = static_cast<int>(255 - 255 * t);
		if (alpha > 255)
		{
			alpha = 255;
		}
		this->trail_.setColor(sf::Color(255, 255, 255, alpha));
	}else
	{
		this->trail_.setColor(sf::Color(255, 255, 255, 0));
		this->active_ = false;
	}
}

void trail::render()
{
	this->window_->draw(this->trail_);
	this->particlesystem_.render();
}

bool trail::get_active() const
{
	return this->active_;
}
