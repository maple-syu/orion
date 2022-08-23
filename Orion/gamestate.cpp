

#include "Gamestate.h"

gamestate::gamestate(sf::RenderWindow* window, sf::View* view, std::stack<state*>* states)
	: state(window, view, states), substate_(enter), player_(window, view), wave_ui_(window, view), level_(6), waves_cleared_(0)
{
	this->spawning_bounds_ = sf::FloatRect(0, 45, view->getSize().x, view->getSize().y * .66f - 45);
	this->player_forming_bounds_ = sf::FloatRect(0, this->spawning_bounds_.height, view->getSize().x, view->getSize().y - this->spawning_bounds_.height);
	this->current_timer_ = 0.f;
	this->idle_timer_ = 1.5f;
	this->entering_timer_ = 1.f;


	init_textures();
	this->player_.set_enemies_list(&this->enemies_);
	this->score_ = 0;
	this->max_game_timer_ = 30.f;
	this->game_timer_ = this->max_game_timer_;
	this->game_over_timer_ = 0.f;
	this->game_over_substate_ = 0;
	this->ui_timer_.setOrigin(0, 0);
	this->ui_timer_.setPosition(0, 0);
	this->ui_timer_.setSize(sf::Vector2f(0, 25.f));
	this->ui_timer_.setFillColor(sf::Color(255, 255, 255, 255));
	if (!this->font_.loadFromFile("Resources/Fonts/Lato-Regular.ttf"))
	{
		throw std::exception("ERROR::GAMESTATE::COULD NOT LOAD FONT");
	}
	this->ui_score_.setFont(this->font_);
	this->ui_score_.setCharacterSize(24);
	this->ui_score_.setString(std::to_string(this->score_));
	this->ui_score_.setFillColor(sf::Color(255, 255, 255, 0));
	this->ui_score_.setOrigin(this->ui_score_.getLocalBounds().width / 2.f, 0.f);
	this->ui_score_.setPosition(this->view_->getSize().x / 2.f, 25.f);
	this->game_over_text_.setFont(this->font_);
	this->game_over_text_.setString("GAME OVER");
	this->game_over_text_.setOrigin(this->game_over_text_.getLocalBounds().width / 2.f, this->game_over_text_.getLocalBounds().height);
	this->game_over_text_.setPosition(this->view_->getSize().x / 2.f, this->view_->getSize().y / 2.f);
	this->game_over_text_.setFillColor(sf::Color(0, 0, 0, 0));
	this->game_over_text_score_.setFont(this->font_);
	this->game_over_text_score_.setFillColor(sf::Color(255, 255, 255, 255));
	populate_nodes();
	first_level();
}

gamestate::~gamestate()
{
	
}

void gamestate::update_player_input()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		this->mouse_pos_ = this->window_->mapPixelToCoords(sf::Mouse::getPosition(*this->window_));
		if (this->player_forming_bounds_.contains(this->mouse_pos_))
		{
			this->is_pressed_ = true;
		}
	}
	else
	{
		this->is_pressed_ = false;
	}
}

void gamestate::update(const float& delta_time)
{
	if (this->first_frame_)
	{
		
	}
	this->current_timer_ += delta_time;
	switch (this->substate_)
	{
	case enter:
		entering();
		break;
	case idle:
		decrease_game_timer(delta_time);
		this->player_.update(this->is_pressed_, this->mouse_pos_, delta_time);
		idling();
		break;
	case spawn:
		decrease_game_timer(delta_time);
		this->player_.update(this->is_pressed_, this->mouse_pos_, delta_time);
		spawning();
		break;
	case recharge:
		this->player_.update(this->is_pressed_, this->mouse_pos_, delta_time);
		recharging(delta_time);
		break;
	case end:
		this->player_.update(false, sf::Vector2f(0.f,0.f), delta_time);
		ending(delta_time);
		break;
	default: ;
	}
	if (!this->enemies_.empty())
	{
		for (unsigned i = 0; i < this->enemies_.size(); i++)
		{
			this->enemies_[i].update(delta_time);
			if (this->enemies_[i].points_to_collect())
			{
				if (this->enemies_[i].get_power_up_code() != 0)
				{
					increment_score(this->enemies_[i].collect_points(), this->enemies_[i].get_power_up_code());
					
				}else
				{
					increment_score(this->enemies_[i].collect_points());
				}
			}
			if (this->enemies_[i].is_dead())
			{
				this->enemies_.erase(this->enemies_.begin() + i);
				
			}
		}
	}
	else
	{
		if (this->current_en_to_spawn_ <= 0 && this->substate_ != end)
		{
			wave_cleared();
			this->recharge_snapshot_ = this->game_timer_;
			this->recharge_timer_ = 0.f;
			this->substate_ = recharge;
		}
	}
	this->wave_ui_.update(delta_time);
}

void gamestate::render(sf::RenderTarget* target)
{
	if (target)
	{
		
	}else
	{

		this->window_->draw(this->background_);
		this->window_->draw(this->ui_timer_);
		this->window_->draw(this->ui_score_);
		this->player_.render();
		if (!this->enemies_.empty())
		{
			for (auto& enemies : this->enemies_)
			{
				enemies.render();
			}
		}
		if (this->substate_ == end)
		{
			this->window_->draw(this->game_over_text_);
			this->window_->draw(this->game_over_text_score_);
		}
	}
	this->wave_ui_.render();
}

void gamestate::increment_score(const int& value)
{
	this->score_ += value;
	this->ui_score_.setString(std::to_string(this->score_));

	this->ui_score_.setOrigin(this->ui_score_.getLocalBounds().width / 2.f, 0);
	this->ui_score_.setPosition(this->view_->getSize().x / 2, 25.f);
}

void gamestate::increment_score(const int& bonus, const int& power_up)
{
	increment_score(bonus);
	this->player_.power_up(power_up);
}

void gamestate::init_textures()
{
	if (!this->textures_["ENEMY_NULL"].loadFromFile("Resources/Sprites/orbs_enemy_0.png"))
	{
		throw std::exception("ERROR::GAMESTATE::COULD NOT LOAD NORMAL ENEMY TEXTURE");
	}
	this->textures_["ENEMY_NULL"].setSmooth(true);
	if (!this->textures_["ENEMY_SPEED"].loadFromFile("Resources/Sprites/orbs_enemy_spd.png"))
	{
		throw std::exception("ERROR::GAMESTATE::COULD NOT LOAD SPEED ENEMY TEXTURE");
	}
	this->textures_["ENEMY_SPEED"].setSmooth(true);
	if (!this->textures_["ENEMY_BOUNCE"].loadFromFile("Resources/Sprites/orbs_enemy_bounce.png"))
	{
		throw std::exception("ERROR::GAMESTATE::COULD NOT LOAD BOUNCE ENEMY TEXTURE");
	}
	this->textures_["ENEMY_BOUNCE"].setSmooth(true);
	if (!this->textures_["ENEMY_PENETRATE"].loadFromFile("Resources/Sprites/orbs_enemy_penetrate.png"))
	{
		throw std::exception("ERROR::GAMESTATE::COULD NOT LOAD PENETRATE ENEMY TEXTURE");
	}
	this->textures_["ENEMY_PENETRATE"].setSmooth(true);
	if (!this->textures_["RING"].loadFromFile("Resources/Sprites/orbs_ring.png"))
	{
		throw std::exception("ERROR::GAMESTATE::COULD NOT LOAD RING TEXTURE");
	}
	this->textures_["RING"].setSmooth(true);

}

void gamestate::entering()
{
	const int ui_col = static_cast<int>(255 * this->current_timer_ / this->entering_timer_);
	const float ui_size = this->view_->getSize().x;
	const float t = this->current_timer_ / this->entering_timer_;
	this->ui_timer_.setSize(sf::Vector2f(ui_size*t, 25.f));
	this->ui_score_.setFillColor(sf::Color(255, 255, 255, ui_col));

	if (this->current_timer_>=this->entering_timer_)
	{
		this->ui_timer_.setSize(sf::Vector2f(this->view_->getSize().x, 25.f));
		this->ui_score_.setFillColor(sf::Color(255, 255, 255, 255));
		this->current_timer_ = 0;
		this->substate_ = idle;
	}
}

void gamestate::idling()
{
	if (this->current_timer_>=this->idle_timer_)
	{
		this->current_timer_ = 0.f;
		if (this->current_en_to_spawn_ > 0)
		{
			this->substate_ = spawn;
		}
	}
}

void gamestate::spawning()
{
	if (this->current_timer_ >= this->level_spawn_timer_[this->level_])
	{
		this->current_timer_ = 0.f;
		if (this->current_en_to_spawn_ > 0)
		{
			const bool res = spawn_enemy();
			if (res)
			{
				if (this->first_enemy_spawned_ == false)
				{
					this->first_enemy_spawned_ = true;
				}
				this->current_en_to_spawn_--;
			}
			else
			{
				this->substate_ = idle;
			}
		}
		else
		{
			this->substate_ = idle;
		}
	}
}

void gamestate::recharging(const float& delta_time)
{
	float max_recharge_timer;
	if (this->level_ < 4)
	{
		max_recharge_timer = 2.f;
	}else
	{
		max_recharge_timer = 1.f;
	}
	const float delta_game_timer = this->max_game_timer_ - this->recharge_snapshot_;
	if (this->recharge_timer_ < max_recharge_timer)
	{
		this->recharge_timer_ += delta_time;
		float t = this->recharge_timer_ / max_recharge_timer;
		if (t > 1)
		{
			t = 1.f;
		}
		if (t < 0)
		{
			t = 0.f;
		}
		this->game_timer_ = this->recharge_snapshot_ + (delta_game_timer*t);
	}
	else
	{
		if (this->level_ < 4)
		{
			this->substate_ = spawn;
		}else
		{
			this->substate_ = idle;
		}
	}
	update_game_timer_ui();
}

void gamestate::ending(const float& delta_time)
{
	this->game_over_timer_ += delta_time;
	switch (this->game_over_substate_)
	{
	case 0:
	{
		const auto max0 = 1.f;
		const int bg_new_col = static_cast<int>(255 * (this->game_over_timer_ / max0));
		this->background_.setFillColor(sf::Color(bg_new_col, bg_new_col, bg_new_col, 255));
		if (this->game_over_timer_ >= max0)
		{
			this->player_.game_end();
			this->enemies_.clear();
			this->ui_timer_.setFillColor(sf::Color(0, 0, 0, 0));
			this->ui_score_.setFillColor(sf::Color(0, 0, 0, 0));
			this->background_.setFillColor(sf::Color(255, 255, 255, 255));
			this->game_over_text_score_.setString(std::to_string(this->score_));
			this->game_over_text_score_.setOrigin(this->game_over_text_score_.getLocalBounds().width / 2.f, 0);
			this->game_over_text_score_.setPosition(this->game_over_text_.getPosition().x, this->game_over_text_.getPosition().y+10.f);
			advance_game_over();
		}
		break;
	}
	case 1:
	{
		const auto max1 = 1.f;
		const int go_new_col = static_cast<int>(255 * (this->game_over_timer_ / max1));
		this->game_over_text_.setFillColor(sf::Color(0, 0, 0, go_new_col));
		this->game_over_text_score_.setFillColor(sf::Color(0, 0, 0, go_new_col));
		if (this->game_over_timer_ >= max1)
		{
			this->game_over_text_.setFillColor(sf::Color(0, 0, 0, 255));
			this->game_over_text_score_.setFillColor(sf::Color(0, 0, 0, 255));
			advance_game_over();
		}
		break;
	}
	case 2:
	{
		const auto max2 = 1.5f;
		if (this->game_over_timer_ >= max2)
		{
			advance_game_over();
		}
		break;
	}
	case 3:
	{
		if (this->is_pressed_)
		{
			advance_game_over();
		}
		break;
	}
	case 4:
	{
		const auto max4 = 1.f;
		const int go_exit_col = static_cast<int>(255 - 255 * (this->game_over_timer_ / max4));
		this->game_over_text_.setFillColor(sf::Color(0, 0, 0, go_exit_col));
		this->game_over_text_score_.setFillColor(sf::Color(0, 0, 0, go_exit_col));
		if (this->game_over_timer_ >= max4)
		{
			this->game_over_text_.setFillColor(sf::Color(0, 0, 0, 0));
			this->game_over_text_score_.setFillColor(sf::Color(0, 0, 0, 0));
			advance_game_over();
		}

		break;
	}
	case 5:
	{
		const auto max5 = 1.f;
		const int bg_exit_col = static_cast<int>(255 - 245 * (this->game_over_timer_ / max5));
		this->background_.setFillColor(sf::Color(bg_exit_col, bg_exit_col, bg_exit_col, 255));
		if (this->game_over_timer_ >= max5)
		{
			this->background_.setFillColor(sf::Color(10, 10, 10, 255));
			advance_game_over();
		}
		break;
	}
	case 6:
	{
		this->quit_ = true;
		break;
	}
	default: ;
	}
}

void gamestate::advance_game_over()
{
	this->game_over_timer_ = 0.f;
	this->game_over_substate_++;
}

void gamestate::wave_cleared()
{
	this->waves_cleared_++;
	this->wave_ui_.add_powerup("WAVE " + std::to_string(this->waves_cleared_) + " CLEARED");
	std::cout << "wave cleared! waves until next level: "<<this->waves_to_next_level_[this->level_] - this->waves_cleared_ << std::endl;
	if (this->level_ < this->max_level_)
	{
		if (this->waves_cleared_ == this->waves_to_next_level_[this->level_])
		{
			next_level();
		}
	}
	this->current_en_to_spawn_ = this->enemies_to_spawn_[this->level_];
	this->first_enemy_spawned_ = false;
}

void gamestate::first_level()
{
	this->level_ = 0;
	this->waves_cleared_ = 0;
	this->current_en_to_spawn_ = this->enemies_to_spawn_[this->level_];
}

void gamestate::next_level()
{
	if (this->substate_ != end)
	{
		if (this->level_ < this->max_level_)
		{
			this->level_++;
			std::cout << "next level!" << std::endl;
		}
	}
}

void gamestate::populate_nodes()
{
	std::cout << "populating nodes..." << std::endl;
	this->nodes_.clear();
	for (unsigned i = 0; i < 16; i++)
	{
		for (unsigned q = 0; q < 16; q++)
		{
			this->nodes_.emplace_back(sf::Vector2f((this->spawning_bounds_.width / 17)*(i + 1), (this->spawning_bounds_.height / 17) * (q + 1)));
		}
	}
	shuffle_nodes();
}

void gamestate::shuffle_nodes()
{
}

float gamestate::random_enemy_size()
{
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(this->nodes_.begin(), this->nodes_.end(), g);
	const auto min = this->enemy_sizes_per_level_[this->level_][1];
	const auto max = this->enemy_sizes_per_level_[this->level_][0];
	
	std::uniform_real_distribution<float> gen(min, max);
	const auto val = static_cast<float>(gen(g));
	return val;
}

bool gamestate::spawn_enemy()
{
	const float desired_size = random_enemy_size();
	for (unsigned i=0; i < this->nodes_.size(); i++)
	{
		if (!this->enemies_.empty())
		{
			if (check_surroundings(i, desired_size))
			{
				this->enemies_.emplace_back(this->window_, this->view_, &this->textures_, desired_size, this->nodes_[i]);
				return true;
			}
		}
		else
		{
			this->enemies_.emplace_back(this->window_, this->view_, &this->textures_, desired_size, this->nodes_[i]);
			return true;
		}
	}
	std::cout << "couldn't spawn enemy" << std::endl;
	return false;
}

void gamestate::decrease_game_timer(const float& delta_time)
{
	if (this->first_enemy_spawned_)
	{
		float multiplier = (this->enemies_.size() * .1f) + 1.f;
		if (multiplier > 1.5f)
		{
			multiplier = 1.5f;
		}
		this->game_timer_ -= delta_time * multiplier;
	}
	if (this->game_timer_ <= 0.f)
	{
		this->game_timer_ = 0.f;
		this->substate_ = end;
	}
	if (this->game_timer_>= this->max_game_timer_)
	{
		this->game_timer_ = this->max_game_timer_;
	}

	update_game_timer_ui();
}

void gamestate::reward_timer(const float& amount)
{
	this->game_timer_ += amount;
	if (this->game_timer_ >= this->max_game_timer_)
	{
		this->game_timer_ = this->max_game_timer_;
	}
}

bool gamestate::check_surroundings(const unsigned node_index, const float desired_size)
{
	if (this->nodes_[node_index].x + desired_size * this->textures_["ENEMY_NULL"].getSize().x > this->spawning_bounds_.width || this->nodes_[node_index].x - desired_size < this->spawning_bounds_.left)
	{
		return false;
	}
	if (this->nodes_[node_index].y + desired_size * this->textures_["ENEMY_NULL"].getSize().x > this->spawning_bounds_.height || this->nodes_[node_index].y - desired_size < this->spawning_bounds_.top)
	{
		return false;
	}
	
	for (auto& enemy : this->enemies_)
	{
		const auto dist = sqrt(powf(this->nodes_[node_index].x - enemy.get_position().x, 2)
			+ powf(this->nodes_[node_index].y - enemy.get_position().y, 2));

		if (dist < enemy.get_radius()*2.f + desired_size)
		{
			return false;
		}
	}

	return true;
}

void gamestate::update_game_timer_ui()
{
	const auto ui_timer_size = this->view_->getSize().x;
	const auto t = this->game_timer_ / this->max_game_timer_;
	this->ui_timer_.setSize(sf::Vector2f(ui_timer_size*t, 25.f));
}
