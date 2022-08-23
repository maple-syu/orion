
#include "mainmenustate.h"


namespace orion
{
	mainmenustate::mainmenustate(sf::RenderWindow* window, sf::View* view, std::stack<state*>* states)
		:state(window, view, states),
		player_(window, view),
		start_button_(window, view, &this->player_),
		text_anim_(curve(
			sf::Vector2f(0.f, 0.f),
			sf::Vector2f(0.f, 1.33f),
			sf::Vector2f(1.f, 1.33f),
			sf::Vector2f(1.f, 0.f)))
	{
		init_fonts();
		this->max_animation_time_ = 2.f;
		this->tutorial_active_ = false;
	}

	mainmenustate::~mainmenustate()
	{
		
	}

	void mainmenustate::end_state()
	{
		
	}

	void mainmenustate::update_player_input()
	{
		if (this->state_time_.getElapsedTime().asSeconds() > 1.5f)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				this->mouse_pos_ = this->window_->mapPixelToCoords(sf::Mouse::getPosition(*this->window_));
				const sf::Rect<float> view_bounds(0, 0, this->view_->getSize().x, this->view_->getSize().y);
				if (view_bounds.contains(this->mouse_pos_))
				{
					this->is_pressed_ = true;
				}
			}
			else
			{
				this->is_pressed_ = false;
			}
		}
	}

	void mainmenustate::render(sf::RenderTarget* target)
	{
		if (target)
		{
		}else
		{
			this->window_->draw(this->background_);
			this->window_->draw(this->menu_text_);
			this->window_->draw(this->menu_text_2_);
			this->start_button_.render();
			this->player_.render();
		}
	}

	void mainmenustate::update(const float& delta_time)
	{
		if (this->first_frame_)
		{
			std::cout << "First Frame Main Menu State" << std::endl;
			this->state_time_.restart();
			this->menu_text_.setFillColor(sf::Color(255, 255, 255, 0));
			this->menu_text_2_.setFillColor(sf::Color(255, 255, 255, 0));
			this->current_animation_timer_.restart();
			this->first_frame_ = false;
		}
		if (this->tutorial_active_)
		{
			update_animation_timer();
		}
		menu_text_anim();
		this->start_button_.update(delta_time);
		if (!this->start_button_.is_delaying())
		{
			this->player_.update(this->is_pressed_, this->mouse_pos_, delta_time);
		}else
		{
			this->player_.update(false, this->mouse_pos_, delta_time);
		}
		if (this->start_button_.is_dead())
		{
			this->start_button_.reset();
			this->first_frame_ = true;
			this->tutorial_active_ = false;
			this->menu_text_.setFillColor(sf::Color(255, 255, 255, 0));
			this->menu_text_2_.setFillColor(sf::Color(255, 255, 255, 0));
			this->states_stack_->push(new gamestate(this->window_, this->view_, this->states_stack_));
		}
	}

	void mainmenustate::update_animation_timer()
	{
		if (this->current_animation_timer_.getElapsedTime().asSeconds() > this->max_animation_time_)
		{
			this->current_animation_timer_.restart();
		}
	}

	void mainmenustate::menu_text_anim()
	{
		if (this->state_time_.getElapsedTime().asSeconds() > 10.f)
		{
			if (!this->tutorial_active_)
			{
				this->current_animation_timer_.restart();
				this->tutorial_active_ = true;
			}
			const float text_alpha = this->text_anim_.evaluate_curve(this->current_animation_timer_.getElapsedTime().asSeconds() / this->max_animation_time_).y*255;
			this->menu_text_.setFillColor(sf::Color(255, 255, 255, text_alpha));
			this->menu_text_2_.setFillColor(sf::Color(255, 255, 255, text_alpha));
		}
	}

	void mainmenustate::init_fonts()
	{
		if (!this->font_.loadFromFile("Resources/Fonts/Lato-Regular.ttf"))
		{
			throw std::exception("ERROR::MAINMENUSTATE::COULD NOT LOAD FONT");
		}
		this->menu_text_.setFont(this->font_);
		this->menu_text_2_.setFont(this->font_);
		this->menu_text_.setFillColor(sf::Color(255,255,255,0));
		this->menu_text_2_.setFillColor(sf::Color(255,255,255,0));
		this->menu_text_.setString("CLICK AND DRAG");
		this->menu_text_2_.setString("TO BEGIN");
		const sf::FloatRect bounds1 = this->menu_text_.getLocalBounds();
		const sf::FloatRect bounds2 = this->menu_text_2_.getLocalBounds();
		
		this->menu_text_.setOrigin(bounds1.width / 2.f, 0);
		this->menu_text_2_.setOrigin(bounds2.width / 2.f, 0);

		this->menu_text_.setPosition(this->view_->getSize().x/2.f, this->view_->getSize().y/2.f);
		this->menu_text_2_.setPosition(this->menu_text_.getPosition().x, this->menu_text_.getPosition().y + this->menu_text_.getLocalBounds().height+10.f);
	}
}

