#pragma once
#include "player.hpp"
#include "bullets.hpp"
#include "enemies.hpp"


class game {
private:
	int sizeX;
	int sizeY;

	bool running;
	int stage;
	int enemiesDestroyed;
	int chances;

	sf::Event eve;

	sf::Clock timeInGame;

	sf::Texture* bulletTexture;
	sf::Texture* enemyGroundTexture;
	sf::Texture* enemyAirTexture;

	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;

	sf::Font font;
	sf::Text scoreText;
	int points;

	sf::SoundBuffer shootBuffer;
	sf::Sound shootSound;
	sf::SoundBuffer hitBuffer;
	sf::Sound hitSound;
	sf::SoundBuffer doomDeathBuffer;
	sf::Sound doomDeath;
	sf::SoundBuffer demonDeathBuffer;
	sf::Sound demonDeath;
	sf::Music sound;

	float spawnTimer;
	float spawnTimerMax;

	std::vector<bullet*> bullets;
	std::vector<enemy*> enemies;

	std::unique_ptr<player> doomguy;

	sf::Text hpText;
	sf::RectangleShape playerHpBar;
	sf::RectangleShape playerHpBarBack;

	void initTextures(sf::Font newFont);
	void initWindow(bool music);
	void initPlayer();
	void initEnemies();
	void initPlayerGUI();
public:
	game(const sf::RenderWindow& window,sf::Font newFont, bool music);
	virtual ~game();

	void updatePlayer();
	void updateBullets();
	void updateEnemies();
	void updateCollision();
	void handleBullets();
	void updateStage();
	void checkEndGame();
	void updateGUI();
	void update(sf::RenderWindow& window);
	void renderPlayer(sf::RenderWindow& window);
	void renderGUI(sf::RenderWindow& window);
	void render(sf::RenderWindow& window);
	const int getScore() const;
	const bool getRunning() const;
};