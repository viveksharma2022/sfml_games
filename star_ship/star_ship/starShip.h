#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <list>
#include <cstdlib>
#include <SFML\graphics.hpp>

#define WINDOW_WIDTH 400
#define WINDOW_LENGTH 600
#define PLAYER_WIDTH 40
#define PLAYER_HEIGHT 50
#define ENEMY_WIDTH PLAYER_WIDTH*1.2
#define ENEMY_HEIGHT PLAYER_HEIGHT*0.5
#define MOV_RESOLUTION PLAYER_WIDTH/2 // movement resolution on the screen
#define BULLET_WIDTH 2
#define BULLET_HEIGHT 5
#define MAX_ENEMIES 2

class Game;
const std::string playerImage = "resources\\starShip.png";
constexpr float enemyVelocities[] = { 50.f,100.f,150.f,200.f,250.f,300.f }; // all possible velocities for enemy movement, unit: pixels/second
static sf::Clock globalClock; 

// utility to get array size
template <typename T, size_t N>
constexpr size_t GetArraySize(T(&)[N]) {
	return N;
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
		position({static_cast<float>(rand() % WINDOW_WIDTH), 0.0f}),
		velocity(enemyVelocities[rand() % GetArraySize(enemyVelocities)]),
		isExist(true)
	{
		host.setPosition(position); // set enemy initial position
		std::cout << "Enemy created with velocity: " << velocity << std::endl;
	}
	~Enemy() {
		std::cout << "Enemy destroyed" << std::endl;
	}
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
	virtual void RenderBullets(sf::RenderWindow& mWindow, std::list<Bullet>& bulletList);
	virtual void RenderEnemies(sf::RenderWindow& mWindow, std::list<std::unique_ptr<Enemy>>& enemies);
};

class Game {
private:
	sf::RenderWindow				mWindow;
	std::shared_ptr<Render_API>		renderAPI;
	std::unique_ptr<Player>			starShip;
public:
	Game(std::shared_ptr<Render_API> renderAPI):
		mWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_LENGTH), "Star Ship game"),
		renderAPI(renderAPI),
		starShip(std::make_unique<Player>())
	{}
	void								PollEvents();
	void								Run();
	sf::RenderWindow&					GetWindow() { return this->mWindow; }
	std::unique_ptr<Player>&			GetPlayer() { return this->starShip; }
	void								HandlePlayerInputs(sf::Keyboard::Key key);
	std::list<Bullet>					bulletS;
	std::list<std::unique_ptr<Enemy>>	enemies;
	void								UpdateBullets();
	void								CreateEnemies();
	void								UpdateEnemies();
};

namespace Utility {
	// TODO: change bullet from stack memory to unique pointer. Impact on speed to be taken into account
	void BorderCheck(Bullet& b);
	void BorderCheck(std::unique_ptr<Enemy>& e);
	void CollisionCheck(Bullet& b); 
	bool CheckNotExists(Bullet& b);
}


