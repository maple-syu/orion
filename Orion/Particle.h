

#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML/Graphics.hpp>


struct particle_data
{
	float lifetime{};
	float start_size{};
	float end_size{};
	float min_speed{};
	float max_speed{};
	float delta_speed{};

	sf::Color start_color;
	sf::Color end_color;
	particle_data() = default;
	particle_data(const float& life, const float& s_size, const float& e_size, const float& min_spd, const float& max_spd, const float& delta_spd, const sf::Color& s_col, const sf::Color& e_color)
	{
		this->lifetime = life;
		this->start_size = s_size;
		this->end_size = e_size;
		this->min_speed = min_spd;
		this->max_speed = max_spd;
		this->delta_speed = delta_spd;
		this->start_color = s_col;
		this->end_color = e_color;
	}
	particle_data(const particle_data& pd)
	{
		this->lifetime = pd.lifetime;
		this->start_size = pd.start_size;
		this->end_size = pd.end_size;
		this->min_speed = pd.min_speed;
		this->max_speed = pd.max_speed;
		this->delta_speed = pd.delta_speed;
		this->start_color = pd.start_color;
		this->end_color = pd.end_color;
	}
};

class particle
{
public:
	particle(sf::RenderWindow* window, sf::Texture* tex, const sf::Vector2f& pos, const sf::Vector2f& dir, const float s_spd, const float e_spd, const particle_data& pd);
	~particle();

	void update(const float& delta_time);
	void enable();
	void render();
	bool is_active();

	sf::Vector2f get_pos();
	sf::Vector2f get_dir();

private:
	sf::RenderWindow* window_;
	bool active_;
	sf::Sprite body_;
	float timer_;
	sf::Vector2f direction_;

	particle_data p_d_;
	float start_speed_;
	float end_speed_;
};

#endif




#pragma region Old Code
//#pragma once
//
//#include <SFML/Graphics.hpp>
//class particle
//{
//public:
//	particle(const particle& particle);
//	particle(sf::RenderWindow* window, sf::Texture* tex, const float& lifetime,
//		const float& start_speed, const float& end_speed,
//		const float& start_size, const float& end_size,
//		const sf::Color& start_color, const sf::Color& end_color);
//	~particle();
//
//	void update(const float& delta_time);
//	void render(sf::RenderTarget* target = nullptr) const;
//
//	bool is_active();
//
//	void enable(sf::Vector2f direction, sf::Vector2f start_position);
//	void disable();
//	
//	
//private:
//	sf::RenderWindow* window_{};
//	bool active_{};
//	float timer_{};
//	sf::RectangleShape body_;
//	float lifetime_{};
//	sf::Vector2f direction_{};
//	float start_speed_{};
//	float end_speed_{};
//	//float start_rotation_speed_;
//	//float end_rotation_speed_;
//	float start_size_{};
//	float end_size_{};
//	sf::Color start_color_;
//	sf::Color end_color_;
//};


#pragma endregion

