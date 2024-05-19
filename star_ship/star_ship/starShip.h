#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <SFML\graphics.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_LENGTH 600

class Player {
private:
	sf::Vector2i position;
public:
	Player():
		position({ WINDOW_WIDTH / 2, WINDOW_LENGTH / 2 }) {}
};

class Render_API {	
public:
	Render_API() = default;
	virtual void Render(sf::RenderWindow& mWindow);
};

class Game {
private:
	sf::RenderWindow			mWindow;
	Render_API&&				renderAPI;
	std::unique_ptr<Player>		starShip;
public:
	Game(Render_API&& renderAPI):
		mWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_LENGTH), "Star Ship game"),
		renderAPI(std::move(renderAPI)),
		starShip(std::make_unique<Player>())
	{}
	void PollEvents();
	void Run();
};