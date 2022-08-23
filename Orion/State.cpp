
#include "State.h"
namespace orion
{
	state::state(sf::RenderWindow* window, sf::View* view, std::stack<state*>* states)
	{
		this->states_stack_ = states;
		this->window_ = window;
		this->view_ = view;
		this->wants_to_end_ = false;
		this->quit_ = false;
		this->first_frame_ = true;
		this->background_.setSize(sf::Vector2f(this->view_->getSize().x, this->view_->getSize().y));
		this->background_.setFillColor(sf::Color(10, 10, 10, 255));
	}

	state::~state()
	{
	}
	void state::update_player_input()
	{
	}
	void state::start_end_state()
	{
		this->wants_to_end_ = true;
	}
	const bool& state::get_quit() const
	{
		return this->quit_;
	}
	void state::update(const float& delta_time)
	{
		if (this->wants_to_end_)
		{
			this->end_state();
		}
	}

	void state::render(sf::RenderTarget* target)
	{
		
	}

	void state::end_state()
	{
		this->quit_ = true;
	}
}

