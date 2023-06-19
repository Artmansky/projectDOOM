#include "game.hpp"


void game::initTextures(sf::Font newFont) {
	file::path p("assets/background.png");
	file::path p1("assets/bullet.png");
	file::path p2("assets/DemonGround.png");
	file::path p3("assets/DemonAir.png");
	if (file::exists(p)&& file::exists(p1)&& file::exists(p2)&& file::exists(p3)) {
		this->font = newFont;
		this->scoreText.setFont(this->font);
		this->scoreText.setCharacterSize(40);
		this->scoreText.setFillColor(sf::Color::White);
		this->scoreText.setString("Score 0");
		this->backgroundTexture.loadFromFile(p.string());
		this->backgroundSprite.setTexture(this->backgroundTexture);
		this->backgroundSprite.setScale(0.16f, 0.21f);
		this->bulletTexture = new sf::Texture();
		this->bulletTexture->loadFromFile(p1.string());
		this->enemyGroundTexture = new sf::Texture();
		this->enemyGroundTexture->loadFromFile(p2.string());
		this->enemyAirTexture = new sf::Texture();
		this->enemyAirTexture->loadFromFile(p3.string());
	}
	else throw(std::runtime_error("Basic Textures Couldn't be Loaded!"));
}

void game::initWindow(bool music) {
	file::path p1("assets/MainGame.ogg");
	file::path p2("assets/shoot.ogg");
	file::path p3("assets/DemonDeath.ogg");
	file::path p4("assets/DoomDeath.ogg");
	file::path p5("assets/hit.ogg");
	if (file::exists(p1) && file::exists(p2) && file::exists(p3) && file::exists(p4) && file::exists(p5)) {
		if (music) {
			this->sound.openFromFile(p1.string());
			this->sound.setVolume(10.f);
			this->sound.setLoop(true);
			this->sound.play();
		}
		this->shootBuffer.loadFromFile(p2.string());
		this->shootSound.setBuffer(this->shootBuffer);
		this->shootSound.setVolume(20.f);
		this->demonDeathBuffer.loadFromFile(p3.string());
		this->demonDeath.setBuffer(this->demonDeathBuffer);
		this->demonDeath.setVolume(20.f);
		this->doomDeathBuffer.loadFromFile(p4.string());
		this->doomDeath.setBuffer(this->doomDeathBuffer);
		this->doomDeath.setVolume(20.f);
		this->hitBuffer.loadFromFile(p5.string());
		this->hitSound.setBuffer(this->hitBuffer);
		this->hitSound.setVolume(20.f);
		this->timeInGame.restart();
	}
	else throw(std::runtime_error("Music Couldn't be Loaded!"));
}

void game::initPlayer() { this->doomguy = std::make_unique<player>(); }

void game::initEnemies() {
	this->spawnTimerMax = 20.f;
	this->spawnTimer = this->spawnTimerMax;
	this->stage = 1;
	this->enemiesDestroyed = 0;
	this->points = 0;
	this->chances = 3;
}

void game::initPlayerGUI() {
	this->hpText.setFont(this->font);
	this->hpText.setPosition(sf::Vector2f(470.f, 0.f));
	this->hpText.setFillColor(sf::Color::White);
	this->hpText.setCharacterSize(40);
	this->hpText.setString("HP");
	this->playerHpBar.setSize(sf::Vector2f(100.f, 35.f));
	this->playerHpBar.setFillColor(sf::Color::Red);
	this->playerHpBar.setPosition(sf::Vector2f(500.f, 0.f));
	this->playerHpBarBack = this->playerHpBar;
	this->playerHpBarBack.setFillColor(sf::Color::Black);
}

game::game(const sf::RenderWindow& window,sf::Font newFont, bool music) {
	this->sizeX = window.getSize().x;
	this->sizeY = window.getSize().y;
	this->running = true;
	this->initWindow(music);
	this->initTextures(newFont);
	this->initPlayer();
	this->initEnemies();
	this->initPlayerGUI();
}

game::~game() {
	for (auto* bulletin : this->bullets) { delete bulletin; }
	for (auto* enemy : this->enemies) { delete enemy; }
	delete bulletTexture; 
}

void game::updatePlayer() { this->doomguy->update(); }

void game::updateBullets() {
	for (int i = 0; i < bullets.size(); i++) {
		this->bullets[i]->update();
		if (this->bullets[i]->getGlobalBounds().left - this->bullets[i]->getGlobalBounds().width > sizeX) {
			delete this->bullets[i];
			this->bullets.erase(this->bullets.begin() + i);
		}
		else if (this->bullets[i]->getGlobalBounds().left + this->bullets[i]->getGlobalBounds().width < 0.f) {
			delete this->bullets[i];
			this->bullets.erase(this->bullets.begin() + i);
		}
	}
}

void game::updateEnemies() {
	this->spawnTimer += 0.05f * this->stage;
	if (this->spawnTimer >= this->spawnTimerMax) {
		bool air = generateRandomBool();
		if (air) this->enemies.push_back(new enemy(this->enemyAirTexture, this->stage, air));
		else this->enemies.push_back(new enemy(this->enemyGroundTexture, this->stage, air));
		this->spawnTimer = 0.f;
	}
	for (int i = 0; i < this->enemies.size(); i++) {
		enemies[i]->update();
		bool removeEnemy = false;
		for (int j = 0; j < this->bullets.size(); j++) {
			if (this->bullets[j]->getGlobalBounds().intersects(this->enemies[i]->getGlobalBounds())) {
				this->enemies[i]->dealDamage(1.5 * this->stage);
				this->bullets.erase(this->bullets.begin() + j);
				if (this->enemies[i]->getHp() <= 0.0) removeEnemy = true;
				break;
			}
		}
		if (removeEnemy) {
			this->demonDeath.play();
			delete this->enemies[i];
			this->enemies.erase(this->enemies.begin() + i);
			this->enemiesDestroyed++;
			if (this->stage == 1) this->points++;
			else this->points += this->stage / 2;
			i--; 
		}
		else if (enemies[i]->getGlobalBounds().left + enemies[i]->getGlobalBounds().width < 0.f) {
			delete this->enemies[i];
			this->enemies.erase(this->enemies.begin() + i);
			this->enemiesDestroyed++;
			this->chances--;
			i--;
		}
	}
	for (auto it = this->enemies.begin(); it != this->enemies.end();) {
		if ((*it)->getGlobalBounds().intersects(this->doomguy->getGlobalBounds())) {
			this->hitSound.play();
			this->doomguy->setHp((*it)->getDamage());
			delete* it;
			it = this->enemies.erase(it);
			this->enemiesDestroyed++;
		}
		else ++it;
	}
}

void game::updateCollision() {
	if (this->doomguy->getGlobalBounds().left < 75) { this->doomguy->setPosition(75, this->doomguy->getGlobalBounds().top); }
	else if (this->doomguy->getGlobalBounds().left + this->doomguy->getGlobalBounds().width > this->sizeX) {
		this->doomguy->setPosition(this->sizeX - this->doomguy->getGlobalBounds().width, this->doomguy->getGlobalBounds().top);
	}
	if (this->doomguy->getPosition().y + this->doomguy->getGlobalBounds().height > this->sizeY) {
		this->doomguy->resetVelosityY();
		this->doomguy->setPosition(this->doomguy->getPosition().x, this->sizeY - this->doomguy->getGlobalBounds().height);
	}
}

void game::handleBullets() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && this->doomguy->canAttack()) {
		this->shootSound.play();
		this->bullets.push_back(new bullet(this->bulletTexture, this->doomguy->getPosition().x, this->doomguy->getPosition().y, this->doomguy->directionRight()));
	}
}

void game::updateStage() {
	if (this->stage < 10) {
		switch (this->enemiesDestroyed) {
		case 5:
			this->stage = 2;
			break;
		case 10:
			this->stage = 4;
			break;
		case 15:
			this->stage = 6;
			break;
		case 20:
			this->stage = 8;
			break;
		case 50:
			this->stage = 10;
			break;
		default:
			break;
		}
	}
}

void game::checkEndGame() {
	if (this->doomguy->getHp() == 0) {
		this->doomDeath.play();
		this->running = false;
	}
	else if (this->chances <= 0) this->running = false;
}

void game::updateGUI() {
	if (this->timeInGame.getElapsedTime() >= sf::seconds(3.f)) {
		this->points += this->stage;
		this->timeInGame.restart();
	}
	this->scoreText.setString("Score " + std::to_string(this->points));
	float hpPercent = static_cast<float>(this->doomguy->getHp()) / this->doomguy->getHpMax();
	this->playerHpBar.setSize(sf::Vector2f(100.f * hpPercent, this->playerHpBar.getSize().y));
}

void game::update(sf::RenderWindow& window) {
	while (window.pollEvent(this->eve)) {
		if (this->eve.type == sf::Event::Closed) {
			window.close();
			this->sound.stop();
			this->running = false;
		}
		else if (this->eve.type == sf::Event::KeyPressed && this->eve.key.code == sf::Keyboard::Escape) this->running = false;
	}
	this->updatePlayer();
	this->handleBullets();
	this->updateBullets();
	this->updateEnemies();
	this->updateCollision();
	this->updateStage();
	this->updateGUI();
	this->checkEndGame();
}

void game::renderPlayer(sf::RenderWindow& window) { this->doomguy->render(window); }

void game::renderGUI(sf::RenderWindow& window) {
	window.draw(this->hpText);
	window.draw(this->playerHpBarBack);
	window.draw(this->playerHpBar);
	window.draw(this->scoreText); 
}

void game::render(sf::RenderWindow& window) {
	window.clear();
	window.draw(backgroundSprite);
	this->renderPlayer(window);
	for (auto* bulletin : bullets) {
		bulletin->render(window);
	}
	for (auto* enemy : this->enemies) {
		enemy->render(window);
	}
	this->renderGUI(window);
	window.display();
}

const int game::getScore() const { return this->points; }

const bool game::getRunning() const { return this->running; }