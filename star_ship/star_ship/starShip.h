#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <SFML\graphics.hpp>

class Player {
private:


public:

};

class Render_API {
public:
	virtual void Render(sf::RenderWindow& mWindow);
};

class Game {
private:
	sf::RenderWindow		mWindow;
	Render_API 				renderAPI;
public:
	Game(Render_API&& renderAPI):
		mWindow(sf::VideoMode(800, 600), "My window"),
		renderAPI(std::move(renderAPI))
	{}
	void PollEvents();
	void Run();
};