#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <SFML\graphics.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_LENGTH 600
#define PLAYER_WIDTH 40
#define PLAYER_HEIGHT 50
#define MOV_RESOLUTION PLAYER_WIDTH // movement resolution on the screen

class Game;
const std::string playerImage = "resources\\starShip.png";

class Player {
private:
	sf::RectangleShape playerHost; // host position of the player on the screen
	sf::Vector2f position;
	sf::Texture playerTexture;
public:
	Player() :
		position({ WINDOW_WIDTH / 2, WINDOW_LENGTH - PLAYER_HEIGHT-1}),
		playerHost(sf::Vector2f(PLAYER_WIDTH, PLAYER_HEIGHT))
	{
		playerHost.setPosition(position); // set the initial position of the player
		if (playerTexture.loadFromFile(playerImage)){
			playerHost.setTexture(&playerTexture);// set the texture for the player
		} 
	}
	~Player() {
		std::cout << "Player destructed" << std::endl;
	}
	// utlity functions for player
	sf::RectangleShape&			GetPlayerHost() { return playerHost; }
	void						SetPlayerPosition(sf::Vector2f newPosition);
	const sf::Vector2f&			GetPlayerPosition();
};

class Render_API {	
public:
	Render_API() = default;
	~Render_API() { std::cout << "Deleted Render API" << std::endl; }
	virtual void Render(Game* currentGame);
};

class Game {
private:
	sf::RenderWindow			mWindow;
	std::shared_ptr<Render_API>	renderAPI;
	std::unique_ptr<Player>		starShip;
public:
	Game(std::shared_ptr<Render_API> renderAPI):
		mWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_LENGTH), "Star Ship game"),
		renderAPI(renderAPI),
		starShip(std::make_unique<Player>())
	{}
	void						PollEvents();
	void						Run();
	sf::RenderWindow&			GetWindow() { return this->mWindow; }
	std::unique_ptr<Player>&	GetPlayer() { return this->starShip; }
	void						HandlePlayerInputs(sf::Keyboard::Key key);
};