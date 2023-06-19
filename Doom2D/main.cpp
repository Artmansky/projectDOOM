#include "menu.hpp"


int main() {
	sf::RenderWindow window;
	window.create(sf::VideoMode(600, 400), "DOOM2D", sf::Style::Close | sf::Style::Titlebar);
	window.setFramerateLimit(60);

	menu mainmenu(window.getSize().x, window.getSize().y);
	if (!mainmenu.loginMenu(window)) window.close();

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
			else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Enter) {
					switch (mainmenu.getPressedItem()) {
					case 0:
					{
						game* newGame;
						bool music = true;
						if (mainmenu.getMusicStatus() == sf::Music::Status::Playing) {
							mainmenu.muteSound();
							newGame = new game(window,mainmenu.getFont(), true);
						}
						else {
							newGame = new game(window,mainmenu.getFont(), false);
							music = false;
						}
						while (newGame->getRunning()) {
							newGame->update(window);
							newGame->render(window);
						}
						if (!mainmenu.printScoreBoard(window, newGame->getScore())) window.close();
						delete newGame;
						if (music) mainmenu.muteSound();
					}
					break;
					case 1:
						mainmenu.muteSound();
						break;
					case 2:
						window.close();
						break;
					}
				}
			}
			else if (event.type == sf::Event::KeyReleased) {
				switch (event.key.code) {
				case sf::Keyboard::W:
					mainmenu.moveUp();
					break;
				case sf::Keyboard::S:
					mainmenu.moveDown();
					break;
				}
			}
		}
		window.clear();
		mainmenu.draw(window);
		window.display();
	}

	mainmenu.saveUsers();
	return 0;
}