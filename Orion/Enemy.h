

#ifndef ENEMY_H
#define ENEMY_H
#include <random>
#include "Entity.h"

class enemy :
	public entity
{
public:
	enemy(sf::RenderWindow* window, sf::View* view, std::map<std::string, sf::Texture>* tex, float size, const sf::Vector2f& pos);
	~enemy();
	
	void update(const float& delta_time) override;
	void render(sf::RenderTarget* target = nullptr) override;
	void set_pos(sf::Vector2f pos);
	void collision(bool hyper, int pts);
	sf::Vector2f get_position() const;
	float get_radius() const;
	bool points_to_collect() const;
	int collect_points();
	int get_power_up_code() const;
	bool is_colliding() const;
	bool is_dead() const;

private:
	enum substate { enter, idle, exit };
	substate substate_;
	float max_scale_;
	sf::Sprite ring_;
	sf::Clock anim_timer_;
	
	bool first_frame_;
	bool show_ring_;
	int power_up_code_;
	bool colliding_;
	bool points_to_collect_;
	bool dead_;

	int points_;

	void init_animations();
	void init_sprite(std::map<std::string, sf::Texture>* tex, const sf::Vector2f& pos);

	void entering(const float& delta_time);
	void idling(const float& delta_time);
	void exiting();

};
#endif
