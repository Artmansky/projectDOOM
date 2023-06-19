#include "enemies.hpp"


void enemy::initAnimations() { this->animationTimer.restart(); }

enemy::enemy(sf::Texture* texture, const int& stage, bool air) {
	this->enemySprite.setTexture(*texture);
	this->stage = stage;
	this->hp = this->stage * 2;
	this->hpMax = this->stage * 2;
	this->damage = this->stage * 2;
	this->isAir = air;
	if (air) {
		this->currentFrame = sf::IntRect(0, 0, 107, 80);
		this->enemySprite.setPosition(500.f, generateRandomFloat(65.f,100.f));
		this->enemySprite.setScale(0.95f, 0.95f);
	}
	else {
		this->currentFrame = sf::IntRect(0, 0, 33, 35);
		this->enemySprite.setPosition(500.f, 330.f);
		this->enemySprite.setScale(1.8f, 1.8f);
	}
	this->enemySprite.setTextureRect(this->currentFrame);
	this->initAnimations();
}

enemy::~enemy() {}

const sf::FloatRect enemy::getGlobalBounds() const { return this->enemySprite.getGlobalBounds(); }

const int& enemy::getDamage() const { return this->damage; }

const double& enemy::getHp() const { return this->hp; }

void enemy::dealDamage(const double& dmg) {
	this->hp -= dmg;
	if (this->hp < 0.0) this->hp = 0.0;
}

void enemy::updateAnimations() {
	if (this->animationTimer.getElapsedTime().asSeconds() >= 0.1f) {
		if (isAir) {
			this->currentFrame.top = 0.f;
			this->currentFrame.left += 107.0f;
			if (this->currentFrame.left >= 214.f) this->currentFrame.left = 0.f;
			this->enemySprite.setTextureRect(this->currentFrame);
			this->animationTimer.restart();
		}
		else {
			this->currentFrame.top = 0.f;
			this->currentFrame.left += 33.75f;
			if (this->currentFrame.left >= 101.25f) this->currentFrame.left = 0.f;
			this->enemySprite.setTextureRect(this->currentFrame);
			this->animationTimer.restart();
		}
	}
}

void enemy::update() {
	this->updateAnimations();
	this->enemySprite.move(-2.f * this->stage * 0.2f, 0.f);
}

void enemy::render(sf::RenderTarget& window) { window.draw(this->enemySprite); }