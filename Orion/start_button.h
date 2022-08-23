

#ifndef STARTBUTTON_H
#define STARTBUTTON_H
#include "Entity.h"
#include "Player.h"

class start_button :
	public entity
{
public:
	start_button(sf::RenderWindow* window, sf::View* view, player* player);
	~start_button();

	void update(const float& delta_time) override;
	void render(sf::RenderTarget* target = nullptr) override;
	bool is_delaying() const;
	bool is_dead() const;

	void reset();

private:
	enum substate { enter, idle, exit, delay };
	substate substate_;
	player* player_;
	sf::Texture texture_;
	sf::Clock anim_timer_;

	bool first_frame_;
	bool colliding_;
	bool dead_ = false;
	
	void init_animations();
	
	void entering();
	void idling();
	void die();
	void delaying();

	bool check_if_colliding() const;
};

#endif