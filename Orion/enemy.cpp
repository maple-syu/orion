

#include "Enemy.h"

enemy::enemy(sf::RenderWindow* window, sf::View* view, std::map<std::string, sf::Texture>* tex, const float size, const sf::Vector2f& pos)
	: entity(window, view), substate_(enter) 
{
	this->first_frame_ = true;
	this->show_ring_ = false;
	this->power_up_code_ = 0;
	this->colliding_ = false;
	this->points_to_collect_ = false;
	this->dead_ = false;
	this->points_ = 0;
	
	this->max_scale_ = size;
	init_sprite(tex, pos);

	init_animations();
}

enemy::~enemy()
{

}

void enemy::update(const float& delta_time)
{
	if (this->first_frame_)
	{
		this->anim_timer_.restart();
		this->first_frame_ = false;
	}
	switch (this->substate_)
	{
	case enter:
		entering(delta_time);
		break;
	case idle:
		idling(delta_time);
		break;
	case exit:
		exiting();
		break;
	default:;
	}
}

void enemy::render(sf::RenderTarget* target)
{
	if (target)
	{
		
	}else
	{
		if (this->show_ring_)
		{
			this->window_->draw(this->ring_);
		}
		this->window_->draw(this->body_);
	}
}

void enemy::set_pos(const sf::Vector2f pos)
{
	this->body_.setPosition(pos);
}

void enemy::collision(const bool hyper, const int pts)
{
	if (hyper)
	{
		this->points_ = 200 + pts;
	}else
	{
		this->points_ = 100 + pts;
	}
	this->colliding_ = true;
}

sf::Vector2f enemy::get_position() const
{
	return this->body_.getPosition();
}

float enemy::get_radius() const	
{
	return this->body_.getGlobalBounds().width/2.f;
}

bool enemy::points_to_collect() const
{
	return this->points_to_collect_;
}

int enemy::collect_points()
{
	this->points_to_collect_ = false;
	return this->points_;
}

int enemy::get_power_up_code() const
{
	return this->power_up_code_;
}

bool enemy::is_colliding() const
{
	return this->colliding_;
}

bool enemy::is_dead() const
{
	return this->dead_;
}

void enemy::init_animations()
{
	this->add_animation("ENTER",
		animation(curve(
			sf::Vector2f(0.f, 0.f),
			sf::Vector2f(.1f, .8f),
			sf::Vector2f(.2f, 1.f),
			sf::Vector2f(1.f, 1.f))),
		.25f);
	this->add_animation("IDLE",
		animation(curve(
			sf::Vector2f(0.f, 1.f),
			sf::Vector2f(.5f, .8f),
			sf::Vector2f(.5f, 1.2f),
			sf::Vector2f(1.f, 1.f))),
		1.5f);
	this->add_animation("DIE",
		animation(curve(
			sf::Vector2f(0.f, 1.f),
			sf::Vector2f(.2f, .2f),
			sf::Vector2f(.4f, 0.f),
			sf::Vector2f(1.f, 0.f))),
		.25f);
}

void enemy::init_sprite(std::map<std::string, sf::Texture>* tex, const sf::Vector2f& pos)
{
	this->ring_.setTexture(tex->at("RING"));
	
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_int_distribution<int> gen(0, 5);
	const int r_1 = gen(g);
	if (r_1 == 0)
	{
		this->show_ring_ = true;
		std::uniform_int_distribution<int> gen2(0, 2);
		const int r_2 = gen2(g);
		switch (r_2)
		{
		case 0:
			{
				this->power_up_code_ = 1;
				this->body_.setTexture(tex->at("ENEMY_SPEED"));
			}
			break;
		case 1:
			{
				this->power_up_code_ = 2;
				this->body_.setTexture(tex->at("ENEMY_BOUNCE"));
			}
			break;
		case 2:
			{
				this->power_up_code_ = 3;
				this->body_.setTexture(tex->at("ENEMY_PENETRATE"));
			}
			break;
		default:
			throw std::exception("ERROR::ENEMY::INVALID POWER UP CODE WHEN ASSIGNING ENEMY TEXTURE");
		}
	}else
	{
		this->show_ring_ = false;
		this->body_.setTexture(tex->at("ENEMY_NULL"));
	}
	this->body_.setOrigin(this->body_.getLocalBounds().width / 2.f, this->body_.getLocalBounds().height / 2.f);
	this->body_.setColor(sf::Color(255, 255, 255, 255));
	this->body_.setScale(0.f, 0.f);
	this->body_.setPosition(pos);
	if (this->show_ring_)
	{
		
		this->ring_.setOrigin(this->ring_.getLocalBounds().width / 2.f, this->ring_.getLocalBounds().height / 2.f);
		this->ring_.setColor(sf::Color(255, 255, 255, 255));
		this->ring_.setScale(0.f, 0.f);
		this->ring_.setPosition(pos);
	}
	
}

void enemy::entering(const float& delta_time)
{
	const float point = this->play_animation("ENTER", this->anim_timer_.getElapsedTime().asSeconds()).y;
	this->body_.setScale(point*this->max_scale_, point*this->max_scale_);
	if (this->show_ring_)
	{
		this->ring_.setScale((point*this->max_scale_)*1.4f, (point*this->max_scale_)*1.4f);
		this->ring_.rotate(70 * delta_time);
	}
	if (this->anim_timer_.getElapsedTime().asSeconds() >= this->get_anim_length("ENTER"))
	{
		this->anim_timer_.restart();
		this->substate_ = idle;
	}
}

void enemy::idling(const float& delta_time)
{
	const float point = this->play_animation("IDLE", this->anim_timer_.getElapsedTime().asSeconds()).y;
	this->body_.setScale(point*this->max_scale_, point*this->max_scale_);
	if (this->show_ring_)
	{
		//this->ring_.setScale((point*this->max_scale_)*1.4f, (point*this->max_scale_)*1.4f);
		this->ring_.rotate(60 * delta_time);

	}
	if (this->colliding_)
	{
		this->anim_timer_.restart();
		this->points_to_collect_ = true;
		this->substate_ = exit;
	}
	if (this->anim_timer_.getElapsedTime().asSeconds() > this->get_anim_length("IDLE"))
	{
		this->anim_timer_.restart();
	}
}

void enemy::exiting()
{
	const float point = this->play_animation("DIE", this->anim_timer_.getElapsedTime().asSeconds()).y;
	this->body_.setScale(point*this->max_scale_, point*this->max_scale_);
	this->body_.setColor(sf::Color(255, 255, 255, 255 * point));
	if (this->show_ring_)
	{
		this->ring_.setScale((point*this->max_scale_)*1.2f, (point*this->max_scale_)*1.2f);
		this->ring_.setColor(sf::Color(255, 255, 255, 255 * point));
	}
	if (this->anim_timer_.getElapsedTime().asSeconds()>this->get_anim_length("DIE"))
	{

		this->dead_ = true;
	}
}
