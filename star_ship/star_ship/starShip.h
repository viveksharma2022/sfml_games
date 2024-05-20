#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <SFML\graphics.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_LENGTH 600

class Game;

class Player {
private:
	sf::RectangleShape playerPlace;
	sf::Vector2i position;
public:
	Player():
		position({ WINDOW_WIDTH / 2, WINDOW_LENGTH / 2 }),
		playerPlace(sf::Vector2f(50.f, 120.f)) {}
	sf::RectangleShape& GetPlayerPlace() { return playerPlace; }
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
	void PollEvents();
	void Run();
	sf::RenderWindow& GetWindow() { return this->mWindow; }
	std::unique_ptr<Player>& GetPlayer() { return this->starShip; }
};