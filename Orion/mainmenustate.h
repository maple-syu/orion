

#pragma once

#include "State.h"
#include "Player.h"
#include "start_button.h"
#include "Gamestate.h"
#include <iostream>

namespace orion
{
	class mainmenustate :
		public state
	{
	public:
		mainmenustate(sf::RenderWindow* window, sf::View* view, std::stack<state*>* states);
		virtual ~mainmenustate();
	protected:
		void end_state() override;
	private:
		//Player
		player player_;

		//other stuff
		sf::Font font_;
		sf::Text menu_text_;
		sf::Text menu_text_2_;
		start_button start_button_;
		animation text_anim_;
		sf::Clock current_animation_timer_;
		bool tutorial_active_;
		float max_animation_time_;

		//Control conditions
		bool is_pressed_ = false;
		sf::Vector2f mouse_pos_;			//Should use touch later...

		//Unique Functions
		void update_animation_timer();
		void menu_text_anim();

		//Init Functions
		void init_fonts();

		//Base Functions
		void update_player_input() override;
		void render(sf::RenderTarget* target) override;
		void update(const float& delta_time) override;
	};
}
//#endif