#pragma once
#include "imports.hpp"


class enemy {
private:
	sf::Sprite enemySprite;
	int stage;
	double hp;
	double hpMax;
	int damage;
	bool isAir;

	sf::IntRect currentFrame;
	
	sf::Clock animationTimer;

	void initAnimations();
public:
	enemy(sf::Texture* texture, const int& stage, bool air);
	virtual ~enemy();

	const sf::FloatRect getGlobalBounds() const;
	const int& getDamage() const;
	const double& getHp() const;

	void dealDamage(const double& dmg);
	void updateAnimations();
	void update();
	void render(sf::RenderTarget& window);
};