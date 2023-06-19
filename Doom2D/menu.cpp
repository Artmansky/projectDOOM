#include "menu.hpp"


void menu::getUsers() {
	file::path p1("Users/Users.txt");
	std::regex example("User:\\s*(\\w+),\\s*Score:\\s*(\\d+)");
	std::smatch found;
	std::string text;
	int ite = 0;
	if (file::exists(p1)) {
		std::ifstream file(p1.string());
		std::getline(file, text);
		while (std::regex_search(text, found, example)) {
			this->users[ite].name = found[1];
			this->users[ite].score = found[2];
			text = found.suffix();
			ite++;
		}
		if (ite < 9) {
			for (int i = ite; i < 10; i++) { this->users[i].name = this->users[i].score = "---"; }
		}
		file.close();
	}
	else for (int i = 0; i < 10; i++) { this->users[i].name = this->users[i].score = "---"; }
}

void menu::insertNewScore(const int& score) {
	for (int i = 0; i < 10; i++) {
		if (this->users[i].name == "---" && this->users[i].score == "---") {
			this->users[i].name = this->playerName;
			this->users[i].score = std::to_string(score);
			break;
		}
		else if (std::stoi(this->users[i].score) <= score) {
			for (int j = 9; j > i; j--) {
				this->users[j].name = this->users[j - 1].name;
				this->users[j].score = this->users[j - 1].score;
			}
			this->users[i].name = this->playerName;
			this->users[i].score = std::to_string(score);
			break;
		}
	}
}

menu::menu(const float& width, const float& height) {
	file::path p("assets/DoomFont.ttf");
	file::path p1("assets/menuB.png");
	file::path p2("assets/logo.png");
	file::path p3("assets/MenuMusic.ogg");
	if (file::exists(p) && file::exists(p1) && file::exists(p2) && file::exists(p3)) {
		this->font.loadFromFile(p.string());
		this->text[0].setFont(font);
		this->text[0].setFillColor(sf::Color::Red);
		this->text[0].setString("Play");
		this->text[0].setPosition(sf::Vector2f((width - this->text[0].getGlobalBounds().width) / 2, height / (this->max + 1)));
		this->text[1].setFont(font);
		this->text[1].setFillColor(sf::Color::White);
		this->text[1].setString("Mute");
		this->text[1].setPosition(sf::Vector2f((width - this->text[1].getGlobalBounds().width) / 2, height / (this->max + 1) * 2));
		this->text[2].setFont(font);
		this->text[2].setFillColor(sf::Color::White);
		this->text[2].setString("Exit");
		this->text[2].setPosition(sf::Vector2f((width - this->text[2].getGlobalBounds().width) / 2, height / (this->max + 1) * 3));
		this->backgroundTexture.loadFromFile(p1.string());
		this->backgroundSprite.setTexture(this->backgroundTexture);
		this->backgroundSprite.setScale(0.31f, 0.37f);
		this->scoreBoardTop.setFont(font);
		this->scoreBoardTop.setCharacterSize(30);
		this->scoreBoardTop.setFillColor(sf::Color::Red);
		this->scoreBoardTop.setString("You lost!");
		this->scoreBoardTop.setPosition(275.f, 10.f);
		this->logoTexture.loadFromFile(p2.string());
		this->logoSprite.setTexture(this->logoTexture);
		this->logoSprite.setScale(0.075f, 0.075f);
		this->logoSprite.setPosition(sf::Vector2f((width - this->logoSprite.getGlobalBounds().width) / 2, 0.025 * height));
		this->sound.openFromFile(p3.string());
		this->sound.setVolume(15.0f);
		this->sound.setLoop(true);
		this->sound.play();
	}
	else throw(std::runtime_error("Couldn't Load Menu Assets!"));
	this->getUsers();
	this->selected = 0;
}

menu::~menu() {}

void menu::draw(sf::RenderWindow& window) {
	window.draw(this->backgroundSprite);
	window.draw(this->logoSprite);
	for (int i = 0; i < this->max; i++) { window.draw(this->text[i]); }
}

bool menu::loginMenu(sf::RenderWindow& window) {
	bool enterReleased = false;
	sf::Text startMenu;
	startMenu.setFont(this->font);
	startMenu.setFillColor(sf::Color::Red);
	startMenu.setString("What's your name Slayer?");
	startMenu.setPosition(175, 100);

	sf::RectangleShape loginBox(sf::Vector2f(225, 30));
	loginBox.setFillColor(sf::Color::Black);
	loginBox.setOutlineThickness(2);
	loginBox.setOutlineColor(sf::Color::White);
	loginBox.setPosition(175, 200);

	sf::Text userInputText;
	userInputText.setFont(this->font);
	userInputText.setCharacterSize(30);
	userInputText.setFillColor(sf::Color::White);
	userInputText.setPosition(175, 200);

	std::string loginInput;
	while (!enterReleased) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (loginInput.length() == 17) {
				this->playerName = loginInput;
				return true;
			}
			if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Enter) enterReleased = true;
			if (event.type == sf::Event::Closed) return false;
			if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode < 128) {
					if (event.text.unicode == '\b' && !loginInput.empty()) loginInput.pop_back();
					else if (event.text.unicode != '\b') loginInput += static_cast<char>(event.text.unicode);
				}
			}
		}
		userInputText.setString(loginInput);
		window.clear();
		window.draw(startMenu);
		window.draw(loginBox);
		window.draw(userInputText);
		window.display();
	}
	if (loginInput.length() > 1) this->playerName = loginInput;
	else this->playerName = "User";
	return true;
}

bool menu::printScoreBoard(sf::RenderWindow& window,const int& score) {
	bool enterReleased = false;
	sf::Text nameList[10],scoreList[10];
	this->insertNewScore(score);
	for (int i = 0; i < 10; i++) {
		nameList[i].setString(std::to_string(i+1)+ ".     " + this->users[i].name);
		nameList[i].setFont(this->font);
		nameList[i].setCharacterSize(40);
		nameList[i].setFillColor(sf::Color::White);
		nameList[i].setPosition(100.f, 35.f + i * 35.f);
		scoreList[i].setString(this->users[i].score);
		scoreList[i].setFont(this->font);
		scoreList[i].setCharacterSize(40);
		scoreList[i].setFillColor(sf::Color::White);
		scoreList[i].setPosition(475.f, 35.f + i * 35.f);
	}
	while (!enterReleased) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Enter) enterReleased = true;
			if (event.type == sf::Event::Closed) return false;
		}
		for (int i = 0; i < 10; i++) { 
			window.draw(nameList[i]);
			window.draw(scoreList[i]); 
		}
		window.draw(this->scoreBoardTop);
		window.display();
		window.clear();
	}
	return true;
}

void menu::moveDown() {
	if (this->selected + 1 < this->max) {
		this->text[selected].setFillColor(sf::Color::White);
		this->selected++;
		this->text[selected].setFillColor(sf::Color::Red);
	}
}

void menu::moveUp() {
	if (this->selected - 1 >= 0) {
		this->text[selected].setFillColor(sf::Color::White);
		this->selected--;
		this->text[selected].setFillColor(sf::Color::Red);
	}
}

void menu::muteSound() {
	if (this->sound.getStatus() == sf::Music::Paused) this->sound.play();
	else this->sound.pause();
}

void menu::saveUsers() {
	file::path p1("Users/Users.txt");
	std::ofstream file(p1.string());
	for (int i = 0; i < 10; i++) {
		if (!(this->users[i].name == "---" && this->users[i].score == "---")) {
			file << "User: " << this->users[i].name << ", Score: " << this->users[i].score << ",";
		}
	}
	file.close();
}

sf::Music::Status menu::getMusicStatus() { return this->sound.getStatus(); }

const short menu::getPressedItem() const { return this->selected; }

const sf::Font menu::getFont() const { return this->font; }