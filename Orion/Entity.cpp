

#include "Entity.h"

entity::entity(sf::RenderWindow* window, sf::View* view)
{
	this->window_ = window;
	this->view_ = view;
	this->texture_ = nullptr;
}

entity::entity(sf::RenderWindow* window, sf::View* view, sf::Texture* texture)
{
	this->window_ = window;
	this->view_ = view;
	this->texture_ = texture;
}


entity::~entity()
{
}

void entity::add_animation(const std::string& key, const animation& anim, const float& length)
{
	this->animations_[key] = anim;
	this->animation_lengths_[key] = length;
}

sf::Vector2f entity::play_animation(const std::string& key, const float& t)
{
	if (animations_.count(key) > 0)
	{
		return animations_[key].evaluate_curve(t / animation_lengths_[key]);
	}
	return sf::Vector2f(0.f, 0.f);
}

void entity::update(const float& delta_time)
{
	
}

void entity::render(sf::RenderTarget* target)
{
	
}

float entity::get_anim_length(const std::string& key)
{
	if (animation_lengths_.count(key)>0)
	{
		return animation_lengths_[key];
	}
	return 0.f;
}
