

#include "Player.h"
#include <iostream>

player::player(sf::RenderWindow* window, sf::View* view)
	: phase_(hidden), puui_(window, view),
	  rotation_speed_(500.f), max_pull_distance_(100.f), hyper_cooldown_(0.f),
	  speed_(500.f),
	  max_bounces_(1), bounces_(this->max_bounces_),
	  max_pierce_(0), pierce_(this->max_pierce_),
	  p_d_(.6f, .1f, .05f, 100, 50, 0.f, sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 0)),
	  particlesystem_(window, "Resources/Sprites/particle_alpha.png", .015f, 100, 90, true, this->p_d_)
{
	this->window_ = window;
	this->view_ = view;
	this->enemies_ = nullptr;
	this->active_hitbox_ = false;
	this->colliding_ = false;
	if (!this->trail_texture_.loadFromFile("Resources/Sprites/hyper_grad_4.png"))
	{
		throw std::exception("ERROR::PLAYER::CANNOT LOAD HYPER GRADIENT TEXTURE");
	}
	this->trail_texture_.setSmooth(true);
	this->body_.setSize(sf::Vector2f(25.f, 25.f));
	this->body_.setFillColor(sf::Color(255, 255, 255, 255));
	this->body_.setOrigin(this->body_.getSize().x / 2.f, this->body_.getSize().y / 2.f);
	this->body_.setPosition(this->window_->getSize().x / 2.f, this->window_->getSize().y / 2.f);
	this->body_.setScale(0.f, 0.f);
}

player::~player()
{
	
}

void player::update(const bool& is_pressed, const sf::Vector2f& mouse_pos, const float& delta_time)
{
	this->particlesystem_.update(delta_time);
	switch (this->phase_)
	{
	case hidden:
		if (is_pressed)
		{
			spawn(mouse_pos);
		}
		break;
	case forming:
		start_forming(is_pressed, mouse_pos);
		break;
	case ready:
		ready_to_fire(is_pressed,mouse_pos, delta_time);
		break;
	case firing:
		fire(delta_time);
		if (this->enemies_ != nullptr)
		{
			check_for_collisions();
		}
		break;
	case hyperfire:
		hyper_cd(delta_time);
		break;
	case dead:
		die();
		break;
	default: ;
	}
	this->puui_.update(delta_time);
}

void player::render(sf::RenderTarget* target)
{
	this->particlesystem_.render();
	if (this->phase_ != hidden)
	{
		this->window_->draw(this->body_);
	}
	if (!this->hyp_trails_.empty())
	{
		for (auto& trail : this->hyp_trails_)
		{
			trail.render();
		}
	}
	this->puui_.render();
}

void player::set_enemies_list(std::vector<enemy>* enemies)
{
	this->enemies_ = enemies;
}

sf::Vector2f player::get_position() const
{
	return this->body_.getPosition();
}

float player::get_size() const
{
	return this->body_.getSize().x;
}

bool player::check_active_hitbox() const
{
	return this->active_hitbox_;
}

int player::collision()
{
	this->pierce_--;
	if (this->pierce_ < 0 && this->phase_ != hyperfire)
	{
		this->phase_ = dead;
	}
	const int points = (this->max_pierce_ - this->pierce_)*100 + (this->max_bounces_ - this->bounces_)*100;
	return points;
}

void player::power_up(const int& power_up)
{
	switch (power_up)
	{
	case 1:
		if (this->speed_ < this->limit_speed_)
		{
			this->speed_ += 250.f;
			if (this->speed_ < limit_speed_)
			{
				this->puui_.add_powerup("SPEED+");
			}
			else
			{
				this->puui_.add_powerup("HYPER");
			}
		}else
		{
			this->puui_.add_powerup("HYPER");
		}
		break;
	case 2:
		if (this->max_bounces_ < this->limit_bounces_)
		{
			this->max_bounces_++;
			this->puui_.add_powerup("BOUNCES+");

		}
		break;
	case 3:
		if (this->max_pierce_ < this->limit_pierce_)
		{
			this->max_pierce_++;
			this->puui_.add_powerup("PENETRATION+");

		}
		break;
	default:
		//incorrect power code
		break;
	}
}

void player::game_end()
{
	this->phase_ = dead;
	this->max_bounces_ = 1;
	this->max_pierce_ = 0;
	this->speed_ = 500.f;
}

//Private Functions
void player::spawn(const sf::Vector2f& pos)
{
	this->body_.setPosition(sf::Vector2f(pos));
	this->body_.setFillColor(sf::Color(255, 255, 255, 255));
	this->phase_ = forming;
}

void player::start_forming(const bool& is_pressed, const sf::Vector2f& mouse_pos)
{
	if (is_pressed)
	{
		const float distance = sqrtf(powf(this->body_.getPosition().x - mouse_pos.x, 2) + powf(this->body_.getPosition().y - mouse_pos.y, 2));
		this->body_.setScale(distance / this->max_pull_distance_, distance / this->max_pull_distance_);
		this->body_.setRotation(80 - 80*(distance/this->max_pull_distance_));
		if (distance >= this->max_pull_distance_)
		{
			this->body_.setScale(1.f, 1.f);
			this->body_.setRotation(0.f);
			this->phase_ = ready;
		}
	}else
	{
		this->phase_ = dead;
	}
}

void player::ready_to_fire(const bool& is_pressed, const sf::Vector2f& mouse_pos, const float& delta_time)
{

	this->body_.rotate(this->rotation_speed_*delta_time);
	
	if (!is_pressed)
	{
		this->firing_timer_.restart();
		
		//Find Unit Vector of mouse_pos and the square
		const sf::Vector2f slope = sf::Vector2f(mouse_pos.x - this->body_.getPosition().x,
		                                        mouse_pos.y - this->body_.getPosition().y);
		const float magnitude = sqrt(powf(slope.x, 2) + powf(slope.y, 2));
		this->fire_direction_ = sf::Vector2f((slope.x/magnitude)*-1.f, (slope.y/magnitude)*-1.f);  //sf::Vector2f(mouse_pos.x - this->body_.getPosition().x, mouse_pos.y - (this->body_.getPosition().y));
		this->active_hitbox_ = true;

		if (this->speed_ >= 2750.f){
			hyper_fire();
			this->phase_ = hyperfire;
		}else{
			this->phase_ = firing;
		}
	}
}

void player::fire(const float& delta_time)
{
	if (!this->particlesystem_.is_active())
	{
		this->particlesystem_.enable();
		this->particlesystem_.set_dir(sf::Vector2f(-this->fire_direction_.x, -this->fire_direction_.y));
	}
	if (this->firing_timer_.getElapsedTime().asSeconds() < 5.f)
	{
		this->body_.rotate(this->rotation_speed_*delta_time*-1.5f);
		this->body_.move(sf::Vector2f(this->fire_direction_.x*this->speed_*delta_time, this->fire_direction_.y*this->speed_*delta_time));
		this->particlesystem_.set_pos(this->body_.getPosition());

	}else
	{
		this->phase_ = dead;
	}
	ricochet();
}

void player::hyper_fire()
{
	bool single_dir = false;
	float a[]{ 0.f,0.f };
	float b[]{ 0.f,0.f };
	sf::Vector2f f_dir = this->fire_direction_;
	sf::Vector2f old_pos = this->body_.getPosition();
	sf::Vector2f new_pos(0,0);
	std::vector<sf::Vector2f> hyp_coords;
	hyp_coords.emplace_back(old_pos);
	if (f_dir.x == 0 || f_dir.y == 0)
	{
		single_dir = true;
		if (f_dir.y == -1)
		{
			hyp_coords.emplace_back(sf::Vector2f(old_pos.x, 0.f));
			hyp_coords.emplace_back(sf::Vector2f(old_pos.x, this->view_->getSize().y));
		}
		if (f_dir.y == 1)
		{
			hyp_coords.emplace_back(sf::Vector2f(old_pos.x, this->view_->getSize().y));
			hyp_coords.emplace_back(sf::Vector2f(old_pos.x, 0.f));

		}
		if (f_dir.x == 1)
		{
			hyp_coords.emplace_back(sf::Vector2f(this->view_->getSize().x, old_pos.y));
			hyp_coords.emplace_back(sf::Vector2f(0.f, old_pos.y));
		}
		if (f_dir.x == -1)
		{
			hyp_coords.emplace_back(sf::Vector2f(0.f, old_pos.y));
			hyp_coords.emplace_back(sf::Vector2f(this->view_->getSize().x, old_pos.y));
		}
	}
	else
	{
		for (auto i = 0; i <= this->bounces_; i++)
		{
#pragma region Hyper Path Algorithm
			const float theta = abs(asinf(this->fire_direction_.y / 1) * (180 / PI));
			if (f_dir.x > 0)
			{
				a[0] = this->view_->getSize().x - old_pos.x;
			}
			else
			{
				a[0] = old_pos.x;
			}
			b[0] = tanf(theta * PI / 180.f) * a[0];
			if (f_dir.y > 0)
			{
				a[1] = this->view_->getSize().y - old_pos.y;
			}
			else
			{
				a[1] = old_pos.y;
			}
			b[1] = tanf((90.f - theta) * PI / 180.f) * a[1];
			if (f_dir.x > 0)
			{
				if (f_dir.y < 0)
				{
					if (b[0] < old_pos.y)
					{
						new_pos = sf::Vector2f(this->view_->getSize().x, old_pos.y - b[0]);
						f_dir.x *= -1.f;
					}
					else
					{
						new_pos = sf::Vector2f(old_pos.x + b[1], 0.f);
						f_dir.y *= -1.f;
					}
				}
				else
				{
					if (b[0] < this->view_->getSize().y - old_pos.y)
					{
						new_pos = sf::Vector2f(this->view_->getSize().x, old_pos.y + b[0]);
						f_dir.x *= -1.f;
					}
					else
					{
						new_pos = sf::Vector2f(old_pos.x + b[1], this->view_->getSize().y);
						f_dir.y *= -1.f;
					}
				}
			}
			else
			{
				if (f_dir.y < 0)
				{
					if (b[0] < this->body_.getPosition().y)
					{
						new_pos = sf::Vector2f(0, old_pos.y - b[0]);
						f_dir.x *= -1.f;
					}
					else
					{
						new_pos = sf::Vector2f(old_pos.x - b[1], 0.f);
						f_dir.y *= -1.f;
					}
				}
				else
				{
					if (b[0] < this->view_->getSize().y - old_pos.y)
					{
						new_pos = sf::Vector2f(0, old_pos.y + b[0]);
						f_dir.x *= -1.f;
					}
					else
					{
						new_pos = sf::Vector2f(old_pos.x - b[1], this->view_->getSize().y);
						f_dir.y *= -1.f;
					}
				}
			}

			hyp_coords.emplace_back(new_pos);
			old_pos = new_pos;
#pragma endregion
		}
	}

	hyper_kill_enemies(hyp_coords, single_dir);
}

void player::hyper_kill_enemies(std::vector<sf::Vector2f> hyp_coords, const bool& single_dir)
{

	if (single_dir)
	{
		std::vector<enemy*> confirmed_hits;
		const sf::Vector2f dir(hyp_coords[2].x - hyp_coords[1].x, hyp_coords[2].y - hyp_coords[1].y);
		if (dir.x == 0)
		{
			if (this->enemies_ != nullptr)
			{
				for (auto& en : *this->enemies_)
				{
					const auto distance = get_distance(en.get_position(), sf::Vector2f(hyp_coords[1].x, en.get_position().y));
					if (distance < en.get_radius() + this->body_.getLocalBounds().width/2.f)
					{
						confirmed_hits.emplace_back(&en);
					}
				}
			}	
		}
		else if (dir.y == 0)
		{
			if (this->enemies_ != nullptr)
			{
				for (auto& en : *this->enemies_)
				{
					const auto distance = get_distance(en.get_position(), sf::Vector2f(en.get_position().x, hyp_coords[1].y));
					if (distance < en.get_radius() + this->body_.getLocalBounds().width/2.f)
					{
						confirmed_hits.emplace_back(&en);
					}
				}
			}
		}else
		{
		}
		std::sort(confirmed_hits.begin(), confirmed_hits.end(), [ptr = hyp_coords[0]](const enemy* a, const enemy* b)
		{
			return get_distance(ptr, a->get_position()) < get_distance(ptr, b->get_position());
		});
		for (auto& hit : confirmed_hits)
		{
			hit->collision(true, collision());
			if (this->pierce_ < 0)
			{
				break;
			}
		}
		this->confirmed_hyper_coords_.emplace_back(hyp_coords[0]);
		this->confirmed_hyper_coords_.emplace_back(hyp_coords[1]);
		this->confirmed_hyper_coords_.emplace_back(hyp_coords[2]);
	}
	else
	{
		unsigned hyp_index = 0;
		this->confirmed_hyper_coords_.emplace_back(hyp_coords.at(0));
		while (this->bounces_ > -1 && this->pierce_ > -1 && hyp_index < hyp_coords.size() - 1)
		{
			std::vector<enemy*> confirmed_hits;
			sf::Vector2f hyp_line[2] = { hyp_coords[hyp_index], hyp_coords[hyp_index + 1] };
			if (this->enemies_ != nullptr)
			{
				for (auto& en : *this->enemies_)
				{
					const sf::Vector2f enemy_pos = en.get_position();
					const sf::Vector2f intersection_pt = find_intersection_point(hyp_line[0], hyp_line[1], enemy_pos);
					const float distance = get_distance(enemy_pos, intersection_pt);
					if (distance < en.get_radius() + this->body_.getLocalBounds().width / 2.f)
					{
						const auto line_length = get_distance(hyp_line[0], hyp_line[1]);
						if (get_distance(hyp_line[0], enemy_pos) > line_length + en.get_radius())
						{
							if (get_distance(hyp_line[0], en.get_position())< en.get_radius())
							{
								confirmed_hits.emplace_back(&en);
							}
						}
						else if (get_distance(hyp_line[1], intersection_pt)> line_length + en.get_radius())
						{
							if (get_distance(hyp_line[1], en.get_position())< en.get_radius())
							{
								confirmed_hits.emplace_back(&en);
							}
						}
						else
						{
							confirmed_hits.emplace_back(&en);
						}
					}
				}
				std::sort(confirmed_hits.begin(), confirmed_hits.end(), [ptr = hyp_line[0]](const enemy* a, const enemy* b)
				{
					return get_distance(ptr, a->get_position()) < get_distance(ptr, b->get_position());
				});
			}
			for (auto& hit : confirmed_hits)
			{
				hit->collision(true, collision());
				if (this->pierce_ < 0)
				{
					this->confirmed_hyper_coords_.emplace_back(find_intersection_point(hyp_line[0], hyp_line[1], hit->get_position()));
					break;
				}
			}
			if (this->pierce_ >= 0)
			{
				this->confirmed_hyper_coords_.emplace_back(hyp_coords.at(hyp_index + 1));
				this->bounces_--;
				hyp_index++;
			}
		}
		//for loop version below
	}
	place_trails();
}

void player::hyper_cd(const float& delta_time)
{

	if (!this->hyp_trails_.empty())
	{
		for (size_t i = 0; i < this->hyp_trails_.size(); i++)
		{
			if (this->hyp_trails_.at(i).get_active())
			{
				this->hyp_trails_.at(i).update(delta_time);
			}
			else
			{
				this->hyp_trails_.erase(this->hyp_trails_.begin() + i);
			}
		}
	}
	else
	{
		this->hyper_cooldown_ = 0.f;
		this->phase_ = dead;
	}
}

void player::die()
{

	this->particlesystem_.disable();
	this->colliding_ = false;
	this->active_hitbox_ = false;	
	this->bounces_ = this->max_bounces_;
	this->pierce_ = this->max_pierce_;
	this->hyp_trails_.clear();
	this->confirmed_hyper_coords_.clear();

	this->body_.setPosition(0, 0);
	this->body_.setScale(0.f, 0.f);
	this->phase_ = hidden;
}

void player::ricochet()
{
	const sf::Rect<float> view_bounds(0, 0, this->view_->getSize().x, this->view_->getSize().y);
	if (!view_bounds.contains(this->body_.getPosition()))
	{
		const sf::Vector2f player_pos(this->body_.getPosition());
		if (this->bounces_>0)
		{
			this->bounces_--;
			if (player_pos.x > view_bounds.width)
			{
				this->body_.setPosition(view_bounds.width, player_pos.y);
				
				this->fire_direction_.x *= -1;
			}else if(player_pos.x < view_bounds.left)
			{
				this->body_.setPosition(view_bounds.left, player_pos.y);
				this->fire_direction_.x *= -1;
			}
			if (player_pos.y>view_bounds.height)
			{
				this->body_.setPosition(player_pos.x, view_bounds.height);
				this->fire_direction_.y *= -1;
				
			}else if(player_pos.y < view_bounds.top)
			{
				this->body_.setPosition(player_pos.x, view_bounds.top);
				this->fire_direction_.y *= -1;
			}
			this->particlesystem_.set_dir(sf::Vector2f(this->fire_direction_.x, this->fire_direction_.y));
		}
		else 
		{
			this->phase_ = dead;
		}
	}
}

void player::check_for_collisions()
{
	for (auto& enemy : *this->enemies_)
	{
		const auto d = get_distance(this->body_.getPosition(), enemy.get_position());
		if (d < enemy.get_radius() + this->body_.getSize().x/2.2f && !enemy.is_colliding())
		{
			switch (this->phase_)
			{
			case firing:
				enemy.collision(false, collision());
				break;
			case hyperfire:
				enemy.collision(true, collision());
				break;
			default:
				break;
			}
		}
	}
}

void player::place_trails()
{

	this->body_.setFillColor(sf::Color(255, 255, 255, 0));
	for (size_t i = 0; i<this->confirmed_hyper_coords_.size() - 1; i++)
	{
		this->hyp_trails_.emplace_back(this->window_, &this->trail_texture_, this->confirmed_hyper_coords_.at(i), this->confirmed_hyper_coords_.at(i + 1));
	}
}

float player::get_distance(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
	return sqrtf(powf(p2.x - p1.x, 2.f) + powf(p2.y - p1.y, 2.f));
}

sf::Vector2f player::find_intersection_point(const sf::Vector2f& ln_1, const sf::Vector2f& ln_2, const sf::Vector2f& pt)
{
	const auto slope = (ln_2.y - ln_1.y) / (ln_2.x - ln_1.x);
	const auto y_intercept = ln_1.y - (slope*ln_1.x);
	const auto inverse_slope = (1 / slope) * -1.f;
	const auto alt_y_intercept = pt.y - (inverse_slope*pt.x);
	const auto x = (y_intercept - alt_y_intercept)/(inverse_slope - slope);
	const auto y = slope * x + y_intercept;
	return { x, y };
	
}
