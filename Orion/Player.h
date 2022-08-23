

#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include "PowerUpUI.h"
#include "Enemy.h"
#include "Trail.h"

class player
{
public:
	player(sf::RenderWindow* window, sf::View* view);
	~player();

	void update(const bool& is_pressed, const sf::Vector2f& mouse_pos, const float& delta_time);
	void render(sf::RenderTarget* target = nullptr);

	void set_enemies_list(std::vector<enemy>* enemies);
	
	sf::Vector2f get_position() const;
	float get_size() const;
	bool check_active_hitbox() const;
	int collision();
	void power_up(const int& power_up);

	void game_end();	//Game has been ended and player's default abilities are restored.
	
private:
	sf::RenderWindow* window_;
	sf::View* view_;
	std::vector<enemy>* enemies_;
	enum phase { hidden, forming, ready, firing, hyperfire, dead };
	phase phase_;
	powerupui puui_;
	float rotation_speed_;
	float max_pull_distance_;
	bool active_hitbox_;
	bool colliding_;
	sf::RectangleShape body_;
	std::vector<sf::Vector2f> confirmed_hyper_coords_;	//Coords used for rendering.
	sf::Texture trail_texture_;
	std::vector<trail> hyp_trails_;
	sf::Vector2f fire_direction_;
	sf::Clock firing_timer_;		//temp: square will have hitbox.
	float hyper_cooldown_;

	//Abilities
	float limit_speed_ = 2750.f;	//The limit to how fast the player can travel. If at max, travel at hyper speed;
	float speed_;					//How fast the player can move
	int limit_bounces_ = 9;			//The limit of bounces the player is able to acquire (caps max_bounces_)
	int max_bounces_;				//The max number of times a player can ricochet
	int bounces_;					//Current number of times the player can ricochet
	int limit_pierce_ = 9;			//The limit of pierces the player is able to acquire (caps max_pierce_)
	int max_pierce_;				//The max number of enemies a player can travel through
	int pierce_;					//The current number of enemies the player can travel through

	//Particles
	sf::Texture particle_texture_;
	particle_data p_d_;
	particle_system particlesystem_;

	//Gameplay Functions
	void spawn(const sf::Vector2f& pos);
	void start_forming(const bool& is_pressed, const sf::Vector2f& mouse_pos);
	void ready_to_fire(const bool& is_pressed, const sf::Vector2f& mouse_pos, const float& delta_time);
	void fire(const float& delta_time);
	void hyper_fire();
	void hyper_kill_enemies(std::vector<sf::Vector2f> hyp_coords, const bool& single_dir);
	void hyper_cd(const float& delta_time);
	void die();
	void ricochet();	//Flips direction the player is traveling in.

	void check_for_collisions();

	//Other
	void place_trails();
	
	//Tools
	static float get_distance(const sf::Vector2f& p1, const sf::Vector2f& p2);
	static sf::Vector2f find_intersection_point(const sf::Vector2f& ln_1, const sf::Vector2f& ln_2, const sf::Vector2f& pt);
};

#endif
