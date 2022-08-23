

#include "Game.h"
#include "mainmenustate.h"
#include "CompanyState.h"

game::game()
{
	init_window();
	init_states();
}
game::~game()
{
	delete this->window_;
	delete this->view_;
	while (!this->states_stack_.empty())
	{
		delete this->states_stack_.top();
		this->states_stack_.pop();
	}
}
void game::update_delta_time()
{
	//Time passed since last frame (as seconds)
	this->delta_time_ = this->dt_clock_.restart().asSeconds();
}
void game::update_sfml_events()
{
	while (this->window_->pollEvent(this->sf_event_))
	{
		if (this->sf_event_.type == sf::Event::Closed)
		{
			this->window_->close();
		}
		if (this->sf_event_.type == sf::Event::Resized)
		{
			resize_window();
		}
	}
}
void game::update()
{
	this->update_sfml_events();
	if (!this->states_stack_.empty())
	{
		this->states_stack_.top()->update_player_input();
		this->states_stack_.top()->update(this->delta_time_);
		if (this->states_stack_.top()->get_quit())
		{
			delete this->states_stack_.top();
			this->states_stack_.pop();
		}
	}else
	{
		this->window_->close();
	}
}
void game::render()
{
	this->window_->clear(sf::Color(0,0,0,255));
	this->window_->setView(*this->view_);
	//Render items
	if (!this->states_stack_.empty())
	{
		this->states_stack_.top()->render();
	}
	this->window_->display();
}
void game::run()
{
	while (this->window_->isOpen())
	{
		this->update_delta_time();
		this->update();
		this->render();
	}
}

//Init Functions
void game::init_window()
{
	sf::VideoMode window_bounds(576, 1024);
	std::string title = "ORION";
	unsigned framerate_limit = 144;
	bool v_sync_enabled = false;

	//Setting window values
	this->window_ = new sf::RenderWindow(window_bounds, title);
	this->window_->setFramerateLimit(framerate_limit);
	this->window_->setVerticalSyncEnabled(v_sync_enabled);

	this->view_ = new sf::View(sf::FloatRect(0,0, window_bounds.width, window_bounds.height));
	this->view_->setCenter(static_cast<float>(window_bounds.width/2.f), static_cast<float>(window_bounds.height/2.f));
	this->view_->zoom(1);
	*this->view_ = letterbox_view(*this->view_, static_cast<int>(window_bounds.width), static_cast<int>(window_bounds.height));
	
}
void game::init_states()
{
	this->states_stack_.push(new orion::mainmenustate(this->window_, this->view_, &this->states_stack_));
	this->states_stack_.push(new orion::company_state(this->window_, this->view_, &this->states_stack_));
}

void game::resize_window()
{
	*this->view_ = letterbox_view(*this->view_, this->sf_event_.size.width, this->sf_event_.size.height);
}

sf::View game::letterbox_view(sf::View view, int window_width, int window_height)
{
	const float window_ratio = static_cast<float>(window_width) / static_cast<float>(window_height);
	const float view_ratio = static_cast<float>(view.getSize().x) / static_cast<float>(view.getSize().y);
	float size_x = 1;
	float size_y = 1;
	float pos_x = 0;
	float pos_y = 0;

	bool horizontal_spacing = true;
	if (window_ratio<view_ratio)
	{
		horizontal_spacing = false;
	}
	if (horizontal_spacing)
	{
		size_x = view_ratio / window_ratio;
		pos_x = (1 - size_x) / 2.f;
	}else
	{
		size_y = window_ratio / view_ratio;
		pos_y = (1 - size_y) / 2.f;
	}
	view.setViewport(sf::FloatRect(pos_x, pos_y, size_x, size_y));
	return view;
}
