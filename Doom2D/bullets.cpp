#include "bullets.hpp"


bullet::bullet(sf::Texture* text,const float& pos_x,const float& pos_y, bool isRight) {
	this->bulletSprite.setTexture(*text);
	this->bulletSprite.setPosition(pos_x, pos_y);
	if (isRight) {
		this->bulletSprite.setScale(0.1f, 0.1f);
		this->dir.x = 1.f;
		this->dir.y = 0.f;
	}
	else {
		this->bulletSprite.setScale(-0.1f, 0.1f);
		this->bulletSprite.setOrigin(this->bulletSprite.getGlobalBounds().width / 0.2f, 0.f);
		this->dir.x = -1.f;
		this->dir.y = 0.f;
	}
	this->movementSpeed = 15.f;
}

bullet::~bullet() {}

const sf::FloatRect bullet::getGlobalBounds() const { return this->bulletSprite.getGlobalBounds(); }

void bullet::update() { this->bulletSprite.move(this->movementSpeed * this->dir); }

void bullet::render(sf::RenderTarget& window) { window.draw(this->bulletSprite); }