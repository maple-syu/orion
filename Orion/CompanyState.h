

#ifndef COMPANYSTATE_H
#define COMPANYSTATE_H

#include "State.h"
#include "Animation.h"

namespace orion
{
	class company_state
		:public state
	{
	public:
		company_state(sf::RenderWindow* window, sf::View* view, std::stack<state*>* states);
		virtual ~company_state();

	private:
		sf::Sprite logo_;

		float entrance_scale_t_;
		float entrance_rotation_t_;
		float exit_t_;
		animation entrance_anim_rotation_;
		animation entrance_anim_scale_;
		animation exit_anim_rotation_;
		animation exit_anim_scale_;

		//Sub State
		enum substate { intro, exit, quit };
		substate substate_;
		
		//Unique Functions
		void intro_anim();
		void exit_anim();
		
		//Base Functions
		void update(const float& delta_time) override;
		void update_player_input() override;
		void render(sf::RenderTarget* target) override;
		void start_end_state() override;
		void end_state() override;
	};
}


#endif
