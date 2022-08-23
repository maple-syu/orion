

#include "PowerUpUI.h"
#include <iostream>

powerupui::powerupui(sf::RenderWindow* window, sf::View* view)
{
	this->curr_scroll_timer_ = 0.f;
	this->window_ = window;
	this->view_ = view;
	this->text_start_pos_ = sf::Vector2f(this->view_->getSize().x/2.f, this->view_->getSize().y/2.f);
	this->state_ = idle;
	init_fonts();
}

powerupui::~powerupui()
{
}

void powerupui::add_powerup(const std::string& powerup)
{
	this->powerup_queue_.push(powerup);
}

void powerupui::update(const float& delta_time)
{
	if (this->first_frame_)
	{
		this->first_frame_ = false;
		this->state_ = idle;
	}
	switch (this->powerupui::state_)
	{
		case idle:
			if (!this->powerup_queue_.empty())
			{
				std::cout << "Power Up Added!" << std::endl;
				init_scroll(this->powerup_queue_.front());
				this->state_ = scrolling;
			}
		break;
		case scrolling:
			scroll(delta_time);
		break;
		default: ;
	}
}

void powerupui::render()
{
	switch (this->powerupui::state_)
	{
		case idle:
			{
				return;
			}
		case scrolling:
			{
				this->window_->draw(this->powerup_text_);
			}
		break;
		default: ;
	}
}

void powerupui::init_fonts()
{
	if (!this->powerup_font_.loadFromFile("Resources/Fonts/Lato-Regular.ttf"))
	{
		throw std::exception("ERROR::POWERUPUI::COULD NOT LOAD FONT");
	}
	this->powerup_text_.setFont(this->powerup_font_);
	this->powerup_text_.setFillColor(sf::Color(255, 255, 255, 0));
	this->powerup_text_.setString("SPEED+");
	this->powerup_text_.setOrigin(this->powerup_text_.getLocalBounds().width / 2.f, this->powerup_text_.getLocalBounds().height / 2.f);
	this->powerup_text_.setPosition(this->text_start_pos_);
}

void powerupui::init_scroll(const std::string& value)
{
	this->curr_scroll_timer_ = 0.f;
	if (this->powerup_text_.getString() != value)
	{
		this->powerup_text_.setString(this->powerup_queue_.front());
		this->powerup_text_.setOrigin(sf::Vector2f(this->powerup_text_.getLocalBounds().width / 2.f, this->powerup_text_.getLocalBounds().height / 2.f));
		this->powerup_text_.setPosition(this->text_start_pos_);
	}
}

void powerupui::scroll(const float& delta_time)
{
	this->curr_scroll_timer_ += delta_time;
	const auto t = this->curr_scroll_timer_ / this->max_scroll_timer_;
	const auto delta_pos = -100.f;
	const auto delta_alpha = -255;
	if (t < 1)
	{
		this->powerup_text_.setPosition(this->text_start_pos_.x, this->text_start_pos_.y + delta_pos * t);
		this->powerup_text_.setFillColor(sf::Color(255, 255, 255, 255 + delta_alpha * t));
	}
	else
	{
		this->powerup_text_.setPosition(this->text_start_pos_.x, this->text_start_pos_.y + delta_pos);
		this->powerup_text_.setFillColor(sf::Color(255, 255, 255, 0));
		if (!this->powerup_queue_.empty())
		{
			this->powerup_queue_.pop();
		}
		this->state_ = idle;
	}
}
