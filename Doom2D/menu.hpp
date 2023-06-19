#pragma once
#include "game.hpp"


struct user {
	std::string name;
	std::string score;
};

class menu {
private:
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	
	sf::Text scoreBoardTop;

	sf::Texture logoTexture;
	sf::Sprite logoSprite;

	sf::Music sound;

	std::string playerName; 
	user users[10];

	const short max = 3;
	short selected;
	sf::Font font;
	sf::Text text[3];

	void getUsers();
	void insertNewScore(const int& score);
public:
	menu(const float& width,const float& height);
	virtual ~menu();

	void draw(sf::RenderWindow& window);
	bool loginMenu(sf::RenderWindow& window);
	bool printScoreBoard(sf::RenderWindow& window,const int& score);
	void moveUp();
	void moveDown();
	void muteSound();
	void saveUsers();
	sf::Music::Status getMusicStatus();
	const short getPressedItem() const;
	const sf::Font getFont() const;
};