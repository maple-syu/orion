

#pragma once

#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include "mainmenustate.h"


class gamestate
	: public orion::state
{
public:
	gamestate(sf::RenderWindow* window,sf::View* view, std::stack<state*>* states);
	virtual ~gamestate();

	void update_player_input() override;
	void update(const float& delta_time) override;
	void render(sf::RenderTarget* target) override;

	void increment_score(const int& value);
	void increment_score(const int& bonus, const int& power_up);

private:
	enum substate { enter, idle, spawn, recharge, end };
	substate substate_;
	player player_;
	sf::FloatRect spawning_bounds_;
	sf::FloatRect player_forming_bounds_;


	unsigned score_{};
	float game_timer_{};
	float max_game_timer_{};
	sf::RectangleShape ui_timer_;
	sf::Font font_;
	sf::Text ui_score_;
	powerupui wave_ui_;

	bool is_pressed_ = false;
	sf::Vector2f mouse_pos_;
	float current_timer_{};
	float idle_timer_;
	unsigned level_;
	const unsigned max_level_ = 8;
	unsigned waves_cleared_;
	float level_spawn_timer_[10]{ 2.2f, 2.f, 1.2f, 0.8f, 0.6f, 0.4f, 0.3f, 0.18f, 0.15f, .05f };
	unsigned waves_to_next_level_[11]{ 2, 4, 6, 8, 10, 12, 15, 18, 20, 22, 999999999 };
	unsigned enemies_to_spawn_[10]{ 3, 6, 12, 16, 20, 26, 32, 64, 128, 256 };
	unsigned current_en_to_spawn_{};
	bool first_enemy_spawned_ = false;
	float enemy_sizes_per_level_[10][2]{ { .7f, .6f } , {.6f, .55f}, {.55f, .45f}, {.4f, .3f}, {.35f, .25f}, {.35f, .2f}, {.3f, .15f}, {.25f, .15f}, { .12f, .08f } };
	std::vector<sf::Vector2f> nodes_;
	std::vector<enemy> enemies_;

	float entering_timer_;

	float recharge_snapshot_{};	
	float recharge_timer_{};

	float game_over_timer_;
	int game_over_substate_;
	sf::Text game_over_text_;
	sf::Text game_over_text_score_;

	void init_textures();

	void entering();
	void idling();
	void spawning();
	void recharging(const float& delta_time);
	void ending(const float& delta_time);
	void advance_game_over();

	void wave_cleared();
	void first_level();
	void next_level();

	void populate_nodes();
	static void shuffle_nodes();
	float random_enemy_size();
	bool spawn_enemy();

	void decrease_game_timer(const float& delta_time);
	void reward_timer(const float& amount);
	bool check_surroundings(unsigned node_index, const float desired_size);

	void update_game_timer_ui();
	
};

