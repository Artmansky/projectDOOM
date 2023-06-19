#pragma once
#include "imports.hpp"


class bullet {
private:
	sf::Sprite bulletSprite;

	sf::Vector2f dir;
	float movementSpeed;

public:
	bullet(sf::Texture* text,const float& pos_x,const float& pos_y, bool isRight);
	virtual ~bullet();

	const sf::FloatRect getGlobalBounds() const;
	void update();
	void render(sf::RenderTarget& window);
};