

#ifndef POWERUPUI_H
#define POWERUPUI_H

#include <SFML/Graphics.hpp>
#include <queue>

class powerupui
{
public:
	powerupui(sf::RenderWindow* window, sf::View* view);
	~powerupui();
	void add_powerup(const std::string& powerup);
	void update(const float& delta_time);
	void render();

private:
	sf::RenderWindow* window_;
	sf::View* view_;
	sf::Text powerup_text_;
	sf::Font powerup_font_;
	std::queue<std::string> powerup_queue_;
	enum state { idle, scrolling };
	state state_;
	bool first_frame_ = true;
	float curr_scroll_timer_;
	const float max_scroll_timer_ = 1.f;
	sf::Vector2f text_start_pos_;
	
	void init_fonts();
	void init_scroll(const std::string& value);

	void scroll(const float& delta_time);
	
};

#endif
