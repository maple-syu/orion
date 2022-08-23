

#include "start_button.h"

start_button::start_button(sf::RenderWindow* window, sf::View* view, player* player)
	:entity(window, view), substate_(enter), player_(player)
{
	this->first_frame_ = true;
	this->colliding_ = false;
	if (!this->texture_.loadFromFile("Resources/Sprites/orbs_enemy_0.png"))
	{
		throw std::exception("ERROR::START_BUTTON::COULD NOT LOAD TEXTURE");
	}
	this->texture_.setSmooth(true);
	this->body_.setTexture(this->texture_);
	this->body_.setOrigin(this->body_.getLocalBounds().width / 2.f, this->body_.getLocalBounds().height / 2.f);
	this->body_.setScale(0.f, 0.f);
	this->body_.setPosition(view->getSize().x / 2.f, view->getSize().y * .33f);

	
	init_animations();
}

start_button::~start_button()
{
}

void start_button::update(const float& delta_time)
{
	if (this->first_frame_)
	{
		this->anim_timer_.restart();
		this->first_frame_ = false;
	}
	switch (this->substate_)
	{
	case enter:
		entering();
		break;
	case idle:
		this->colliding_ = check_if_colliding();
		idling();
		break;
	case exit:
		die();
		break;
	case delay:
		delaying();
		break;
	default: ;
	}
}

void start_button::render(sf::RenderTarget* target)
{
	this->window_->draw(this->body_);
}

bool start_button::is_delaying() const
{
	switch (this->substate_)
	{
	case exit:
		{
		return true;
		}
	case delay:
		{
		return true;
		}
	default:
		return false;
	}
}

bool start_button::is_dead() const
{
	return this->dead_;
}

void start_button::reset()
{
	this->first_frame_ = true;
	this->substate_ = enter;
	this->dead_ = false;
	this->body_.setColor(sf::Color(255, 255, 255, 255));
}

void start_button::init_animations()
{
	this->add_animation("ENTER",
		animation(curve(
			sf::Vector2f(0.f, 0.f), 
			sf::Vector2f(.1f, .8f), 
			sf::Vector2f(.2f, 1.f), 
			sf::Vector2f(1.f, 1.f))),
		1.f);
	this->add_animation("IDLE",
		animation(curve(
			sf::Vector2f(0.f, 1.f), 
			sf::Vector2f(.5f, .8f), 
			sf::Vector2f(.5f, 1.2f), 
			sf::Vector2f(1.f, 1.f))),
		2.f);
	this->add_animation("DIE",
		animation(curve(
			sf::Vector2f(0.f,1.f),
			sf::Vector2f(.2f,.2f),
			sf::Vector2f(.4f,0.f),
			sf::Vector2f(1.f,0.f))),
		.25f);
}

void start_button::entering()
{
	const float point = this->play_animation("ENTER", this->anim_timer_.getElapsedTime().asSeconds()).y;
	this->body_.setScale(point*.6f, point*.6f);
	if (this->anim_timer_.getElapsedTime().asSeconds() >= this->get_anim_length("ENTER"))
	{
		this->anim_timer_.restart();
		this->substate_ = idle;
	}
}

void start_button::idling()
{
	const float point = this->play_animation("IDLE", this->anim_timer_.getElapsedTime().asSeconds()).y;
	this->body_.setScale(point*.6f, point*.6f);
	if (this->colliding_)
	{
		this->anim_timer_.restart();
		this->substate_ = exit;
	}
	if (this->anim_timer_.getElapsedTime().asSeconds() > this->get_anim_length(("IDLE")))
	{
		this->anim_timer_.restart();
	}
}

void start_button::die()
{
	const float point = this->play_animation("DIE", this->anim_timer_.getElapsedTime().asSeconds()).y;
	this->body_.setScale(point*.6f, point*.6f);
	this->body_.setColor(sf::Color(255, 255, 255, 255 * point));
	if (this->anim_timer_.getElapsedTime().asSeconds() > this->get_anim_length("DIE"))
	{
		this->anim_timer_.restart();
		this->substate_ = delay;
	}
}

void start_button::delaying()
{
	if (this->anim_timer_.getElapsedTime().asSeconds() > 1.f)
	{
		this->dead_ = true;
	}
}

bool start_button::check_if_colliding() const
{
	const float distance = sqrt(powf(this->body_.getPosition().x - this->player_->get_position().x, 2) 
		+ powf(this->body_.getPosition().y - this->player_->get_position().y, 2));
	if (distance<this->body_.getGlobalBounds().width*.45f && this->player_->check_active_hitbox())
	{
		this->player_->collision();
		return true;
	}
	return false;
}
