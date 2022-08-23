
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <stack>
#include "State.h"

class game
{
public:
	game();
	~game();

	void update_delta_time();
	void update_sfml_events();
	void update();
	void render();
	void run();
	
private:
	sf::RenderWindow* window_;
	sf::View* view_;
	sf::Event sf_event_;
	sf::Clock dt_clock_;
	std::stack<orion::state*> states_stack_;
	float delta_time_;

	void init_window();
	void init_states();

	void resize_window();
	sf::View letterbox_view(sf::View view, int window_width, int window_height);
};
#endif
