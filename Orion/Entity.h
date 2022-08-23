

#ifndef ENTITY_H
#define ENTITY_H


#include <SFML/Graphics.hpp>
#include <map>
#include "Animation.h"

class entity
{
public:
	entity(sf::RenderWindow* window, sf::View* view);
	entity(sf::RenderWindow* window, sf::View* view, sf::Texture* texture);
	virtual ~entity();

	virtual void add_animation(const std::string& key, const animation& anim, const float& length);
	virtual sf::Vector2f play_animation(const std::string& key, const float & t);
	virtual void update(const float& delta_time);
	virtual void render(sf::RenderTarget* target = nullptr);

protected:
	
	sf::Texture* texture_;
	sf::Sprite body_;
	sf::RenderWindow* window_;
	sf::View* view_;
	float get_anim_length(const std::string& key);
	
private:

	std::map<std::string, animation> animations_;
	std::map < std::string, float> animation_lengths_;

};

#endif
