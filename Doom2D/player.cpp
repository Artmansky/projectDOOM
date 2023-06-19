#include "player.hpp"


void player::initVariables() {
	this->animState = IDLE;
	this->isRight = true;
	this->attackCooldownMax = 8.f;
	this->attackCooldown = this->attackCooldownMax;
	this->hpMax = 35;
	this->hp = this->hpMax;
}

void player::initSheet() {
	file::path p("assets/doomguysheet.png");
	if (file::exists(p)) this->sheet.loadFromFile(p.string());
	else throw(std::runtime_error("Player Sheet Couldn't be Loaded!"));
}

void player::initSprite() {
	this->sprite.setTexture(this->sheet);
	this->currentFrame = sf::IntRect(0, 0, 25, 37);
	this->sprite.setTextureRect(this->currentFrame);
	this->sprite.setScale(2.f, 2.f);
	this->sprite.setPosition(75.f, 0.f);
}

void player::initAnimations() { this->animationTimer.restart(); }

void player::initPhysics() {
	this->maxVel = 10.f;
	this->minVel = 1.f;
	this->acc = 3.f;
	this->drag = 0.93f;
	this->gravity = 4.f;
	this->maxGravY = 15.f;
	this->canJump = true;
}

player::player() {
	this->initVariables();
	this->initSheet();
	this->initSprite();
	this->initAnimations();
	this->initPhysics();
}

player::~player() {}

const sf::Vector2f player::getPosition() const { return this->sprite.getPosition(); }

const sf::FloatRect player::getGlobalBounds() const { return this->sprite.getGlobalBounds(); }

const int& player::getHp() const { return this->hp; }

const int& player::getHpMax() const { return this->hpMax; }

const bool player::directionRight() { return this->isRight; }

void player::setPosition(const float& x, const float& y) { this->sprite.setPosition(x, y); }

void player::resetVelosityY() { this->vel.y = 0.f; }

void player::setHp(const int& value) { 
	this->hp -= value;
	if (this->hp < 0) this->hp = 0;
}

void player::move(const float& x, const float& y) {
	if (this->canJump) {
		this->vel.x += x * this->acc;
		this->vel.y = -10.f;
		this->canJump = false;
	}
	else this->vel.x += x * this->acc * 0.5f;
	if (std::abs(this->vel.x) > this->maxVel) this->vel.x = this->maxVel * ((this->vel.x < 0.f) ? -1.f : 1.f);
}

const bool player::canAttack() {
	if (this->attackCooldown >= this->attackCooldownMax) {
		this->attackCooldown = 0.f;
		return true;
	}
	else return false;
}

void player::updatePhysics() {
	this->vel.y += 1.0 * this->gravity;
	if (std::abs(this->vel.x) > this->maxVel) this->maxGravY = this->maxVel * ((this->vel.x < 0.f) ? -1.f : 1.f);
	if (this->vel.y >= 0.f && this->sprite.getPosition().y >= 480.f) {
		this->resetVelosityY();
		this->canJump = true;
	}
	this->vel *= this->drag;
	if (std::abs(this->vel.x) < this->minVel) this->vel.x = 0.f;
	if (std::abs(this->vel.y) < this->minVel) this->vel.y = 0.f;
	this->sprite.move(this->vel);
}

void player::updateMove() {
	this->animState = IDLE;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		this->move(-1.f, 0.f);
		this->animState = LEFT;
		this->isRight = false;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		this->move(1.f, 0.f);
		this->animState = RIGHT;
		this->isRight = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		if (this->vel.y == 0.f) this->vel.y = -60.f;
	}
}

void player::updateAnimations() {
	if (this->animState == IDLE) {
		this->currentFrame = sf::IntRect(0, 0, 25, 37);
		this->sprite.setTextureRect(this->currentFrame);
	}
	else if (this->animState == RIGHT) {
		if (this->animationTimer.getElapsedTime().asSeconds() >= 0.1f) {
			this->currentFrame.top = 0.f;
			this->currentFrame.left += 25.2f;
			if (this->currentFrame.left >= 154.3f) this->currentFrame.left = 25;
			this->animationTimer.restart();
			this->sprite.setTextureRect(this->currentFrame);
		}
		this->sprite.setScale(2.f, 2.f);
		this->sprite.setOrigin(0.f, 0.f);
	}
	else if (this->animState == LEFT) {
		if (this->animationTimer.getElapsedTime().asSeconds() >= 0.1f) {
			this->currentFrame.top = 0.f;
			this->currentFrame.left += 25.2f;
			if (this->currentFrame.left >= 154.3f) this->currentFrame.left = 25;
			this->animationTimer.restart();
			this->sprite.setTextureRect(this->currentFrame);
		}
		this->sprite.setScale(-2.f, 2.f);
		this->sprite.setOrigin(this->sprite.getGlobalBounds().width / 1.75f, 0.f);
	}
	else this->animationTimer.restart();
}

void player::updateCooldown() { if (this->attackCooldown < this->attackCooldownMax) this->attackCooldown += 0.5f; }

void player::update() {
	this->updateCooldown();
	this->updateMove();
	this->updateAnimations();
	this->updatePhysics();
}

void player::render(sf::RenderTarget& target) { target.draw(this->sprite); }