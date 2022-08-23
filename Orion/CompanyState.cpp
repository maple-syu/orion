

#include "CompanyState.h"

namespace orion
{
	company_state::company_state(sf::RenderWindow* window, sf::View* view, std::stack<state*>* states)
	:state(window, view, states),
	entrance_anim_rotation_(curve(
		sf::Vector2f(0.f, .8f),
		sf::Vector2f(.1f, .4f),
		sf::Vector2f(.2f, .05f),
		sf::Vector2f(1.f, 0.f))),
	entrance_anim_scale_(curve(
		sf::Vector2f(0.f, 0.f),
		sf::Vector2f(0.1f, 0.8f),
		sf::Vector2f(.2f, 1.f),
		sf::Vector2f(1.f, 1.f))),
	exit_anim_rotation_(curve(
		sf::Vector2f(0.f,0.f),
		sf::Vector2f(0.9f, 0.f),
		sf::Vector2f(1.f, .3f),
		sf::Vector2f(1.f,1.23f))),
	exit_anim_scale_(curve(
		sf::Vector2f(0.f, 1.f),
		sf::Vector2f(0.9f, 1.f),
		sf::Vector2f(1.f, .6f),
		sf::Vector2f(1.f, 0.f))),
	substate_(intro)
	{
		this->entrance_rotation_t_ = 2.f;
		this->entrance_scale_t_ = 1.8f;
		this->exit_t_ = 2.f;
		this->textures_["COMPANY_LOGO"].loadFromFile("Resources/Sprites/Company-Logo.png");
		this->logo_.setTexture(this->textures_["COMPANY_LOGO"]);
		this->logo_.setOrigin(this->logo_.getLocalBounds().width / 2.f, this->logo_.getLocalBounds().height / 2.f);
		this->logo_.setPosition(this->view_->getSize().x *0.5f, this->view_->getSize().y * 0.33f);
	}
	
	company_state::~company_state()
	{
		
	}

	void company_state::intro_anim()
	{
		const float time = this->state_time_.getElapsedTime().asSeconds();
		if (time <= this->entrance_scale_t_)
		{
			const float t1 = time / this->entrance_scale_t_;
			const float cur1 = this->entrance_anim_scale_.evaluate_curve(t1).y;
			this->logo_.setScale(.2f*cur1, .2f*cur1);
		}
		if (time <= this->entrance_rotation_t_)
		{
			const float t2 = time / this->entrance_rotation_t_;
			const float cur2 = this->entrance_anim_rotation_.evaluate_curve(t2).y;
			this->logo_.setRotation(cur2 * -360);
		}
		if (time > this->entrance_scale_t_ && time > this->entrance_rotation_t_)
		{
			std::cout << "Intro anim finished!" << std::endl;
			this->state_time_.restart();
			this->substate_ = exit;
		}
	}

	void company_state::exit_anim()
	{

		const float time = this->state_time_.getElapsedTime().asSeconds();

		if (time <= this->exit_t_)
		{
			const float t = time / this->exit_t_;
			const float cur1 = this->exit_anim_scale_.evaluate_curve(t).y;
			const float cur2 = this->exit_anim_rotation_.evaluate_curve(t).y;

			this->logo_.setScale(.2f*cur1, .2f*cur1);
			this->logo_.setRotation(cur2 * 360);
		}
		else
		{
			this->substate_ = quit;

			this->wants_to_end_ = true;
		}
	}

	void company_state::update(const float& delta_time)
	{
		if (this->first_frame_)
		{
			std::cout << "First frame Logo State" << std::endl;
			this->state_time_.restart();
			this->first_frame_ = false;
		}
		switch (this->substate_)
		{
		case intro:
			intro_anim();
			break;
		case exit:
			exit_anim();
			break;
		default: ;
		}
		if (this->wants_to_end_)
		{
			end_state();
		}
	}

	void company_state::update_player_input()
	{
	}

	void company_state::render(sf::RenderTarget* target)
	{
		if (target)
		{
			target->draw(this->logo_);
		}else
		{
			this->window_->draw(this->background_);
			this->window_->draw(this->logo_);
		}
	}

	void company_state::start_end_state()
	{
	}

	void company_state::end_state()
	{
		state::end_state();
	}

}
