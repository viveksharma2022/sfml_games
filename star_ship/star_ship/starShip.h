#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <list>
#include <cstdlib>
#include <SFML\graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/String.hpp>
#include "definitions.h"
#include "utility.h"

class Game;
const std::string playerImage = "resources\\starShip.jpg";
constexpr float enemyVelocities[] = { 50.f,100.f,150.f,200.f,250.f,300.f }; // all possible velocities for enemy movement, unit: pixels/second
static sf::Clock globalClock; 

// utility to get array size
template <typename T, size_t N>
constexpr size_t GetArraySize(T(&)[N]) {
	return N;
}

enum GameStatus {
	GAME_OVER,
	GAME_RUNNING,
	GAME_PAUSED
};

inline GameStatus ExecuteSpaceKeyEvent(GameStatus status){
	return (status == GAME_PAUSED) ? GAME_RUNNING : GAME_PAUSED;
} 

struct Bullet {
	sf::RectangleShape		host;
	sf::Vector2f			position;
	bool					isExist; 
	Bullet() :
		host(sf::Vector2f(BULLET_WIDTH, BULLET_HEIGHT)),
		isExist(true)// default existance
	{}
	~Bullet() {
		std::cout << "Bullet removed" << std::endl;
	}
};

class Enemy {
public:
	sf::RectangleShape host;
	sf::Vector2f position;
	uint16_t velocity;
	bool isExist;

	Enemy() :
		host(sf::Vector2f(ENEMY_WIDTH, ENEMY_HEIGHT)),
		position({static_cast<float>(rand() % WINDOW_WIDTH - ENEMY_WIDTH - 1), 0.0f}), // randomly positions the player at start
		velocity(enemyVelocities[rand() % GetArraySize(enemyVelocities)]),
		isExist(true)
	{
		host.setPosition(position); // set enemy initial position
		std::cout << "Enemy created with velocity: " << velocity << std::endl;
	}
	~Enemy() {
		std::cout << "Enemy destroyed" << std::endl;
	}
	const sf::RectangleShape& GetHost() const { return host; }
};

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
	virtual void RenderPause(Game* currentGame);
	virtual void RenderBullets(sf::RenderWindow& mWindow, std::list<Bullet>& bulletList);
	virtual void RenderEnemies(sf::RenderWindow& mWindow, std::list<Enemy>& enemies);
};

class Game {
private:
	sf::RenderWindow				mWindow;
	std::shared_ptr<Render_API>		renderAPI;
	std::unique_ptr<Player>			starShip;
	GameStatus						gameStates;
public:
	Game(std::shared_ptr<Render_API> renderAPI):
		mWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_LENGTH), "Star Ship game"),
		renderAPI(renderAPI),
		starShip(std::make_unique<Player>()),
		gameStates(GAME_RUNNING)
	{}
	void								PollEvents();
	void								PollPauseEvent();
	void								Run();
	sf::RenderWindow&					GetWindow() { return this->mWindow; }
	std::unique_ptr<Player>&			GetPlayer() { return this->starShip; }
	void								HandlePlayerInputs(sf::Keyboard::Key key);
	std::list<Bullet>					bullets;
	std::list<Enemy>					enemies;
	void								UpdateBullets();
	void								CreateEnemies();
	void								UpdateEnemies();
};





