

#ifndef STATE_H
#define STATE_H

#include <SFML/Graphics.hpp>
#include "Animation.h"
#include <vector>
#include <stack>

namespace orion
{
	class state
	{
	public:
		state(sf::RenderWindow* window, sf::View* view, std::stack<state*>* states);
		
		virtual ~state();

		virtual void update_player_input();
		virtual void start_end_state();	//Call this when the state wants to end. Should set wants_to_end_ = true
		const bool& get_quit() const;

		//Core Functions
		virtual void update(const float& delta_time);
		virtual void render(sf::RenderTarget* target = nullptr);

	protected:
		std::stack<state*>* states_stack_;
		sf::RenderWindow* window_;
		sf::View* view_;
		sf::RectangleShape background_;
		std::map<std::string, sf::Texture> textures_;
		sf::Clock state_time_;
		bool first_frame_;
		//float state_lifetime_;
		sf::Clock exit_timer_; //The time it takes for the states ending animation to finish.
		bool wants_to_end_;
		bool quit_;	//only true when the state has completely finished it's exit animation.

		virtual void end_state();	//Called during update() if wants_to_end_ is true to animate the end of the state.

	};
}


#endif
