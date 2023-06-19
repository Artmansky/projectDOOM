#pragma once
#include "imports.hpp"


class player {
private:
	sf::Sprite sprite;
	sf::Texture sheet;
	sf::Clock animationTimer;

	int animState;
	bool isRight;
	sf::IntRect currentFrame;

	float attackCooldown;
	float attackCooldownMax;

	sf::Vector2f vel;
	float maxVel;
	float minVel;
	float acc;
	float drag;
	float gravity;
	float maxGravY;
	bool canJump;

	int hpMax;
	int hp;

	void initVariables();
	void initSheet();
	void initSprite();
	void initAnimations();
	void initPhysics();
public:
	player();
	virtual ~player();

	const sf::Vector2f getPosition() const;
	const sf::FloatRect getGlobalBounds() const;
	const int& getHp() const;
	const int& getHpMax() const;
	const bool directionRight();

	void setPosition(const float& x, const float& y);
	void resetVelosityY();
	void setHp(const int& value);

	void move(const float& x, const float& y);
	const bool canAttack();
	void updatePhysics();
	void updateMove();
	void updateAnimations();
	void updateCooldown();
	void update();
	void render(sf::RenderTarget& target);
};