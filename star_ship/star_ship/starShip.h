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
#include <SFML/Graphics/Color.hpp>
#include "definitions.h"
#include "utility.h"

class Game;
const std::string playerImage = "resources\\starShip.jpg";
constexpr float enemyVelocities[] = { 50.f,100.f,150.f,200.f,250.f,300.f }; // all possible velocities for enemy movement, unit: pixels/second
constexpr uint16_t enemykillScore[] = { 100,200,300,400,700,1000 };
static std::vector<sf::Color> enemyColors = { sf::Color(sf::Color::White),
												sf::Color(sf::Color::Green),
												sf::Color(sf::Color::Blue),
												sf::Color(sf::Color::Yellow),
												sf::Color(sf::Color::Cyan),
												sf::Color(sf::Color::Magenta)
											};
static sf::Clock globalClock; 

// utility to get array size
template <typename T, size_t N>
constexpr size_t GetArraySize(T(&)[N]) {
	return N;
}

struct Bullet {
	sf::RectangleShape	host;
	sf::Vector2f	position;
	bool	isExist; 
	bool	isNotCollided;
	Bullet() :
		host(sf::Vector2f(BULLET_WIDTH, BULLET_HEIGHT)),
		isExist(true),// default existance
		isNotCollided(true)
	{}
	~Bullet() {
		std::cout << "Bullet removed" << std::endl;
	}
};

class Enemy {
public:
	sf::RectangleShape host;
	sf::Vector2f position;
	uint8_t id;
	uint16_t velocity;
	uint16_t killScore;
	bool isExist; // to check if it exists within the game window
	bool isNotCollided; // to check collision with bullets

	Enemy() :
		host(sf::Vector2f(ENEMY_WIDTH, ENEMY_HEIGHT)),
		position({static_cast<float>(rand() % WINDOW_WIDTH - ENEMY_WIDTH - 1), 0.1f}), // randomly positions the player at start
		id(rand() % GetArraySize(enemyVelocities)),
		isExist(true),
		isNotCollided(true),
		velocity(enemyVelocities[id]),
		killScore(enemykillScore[id])
	{

		host.setPosition(position); // set enemy initial position
		host.setFillColor(enemyColors[id]); // set color to the enemy
		std::cout << "Enemy created with velocity: " << velocity << std::endl;
	}
	~Enemy() {
		std::cout << "Enemy destroyed" << std::endl;
	}
	const sf::RectangleShape& GetHost() const { return host; }
	const sf::Vector2f& GetEnemyPosition() const{ return position; }
};

class Player {
private:
	sf::RectangleShape playerHost; // host position of the player on the screen
	sf::Vector2f position;
	sf::Texture playerTexture;
public:
	Player() :
		position({ WINDOW_WIDTH / 2, WINDOW_LENGTH - SCORE_BOARD_HEIGHT - PLAYER_HEIGHT-1}),
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

// Game rendering: it is made external and aggregated into the game..
// offers easy extensivity if required
class Render_API {	
public:
	Render_API() = default;
	~Render_API() { std::cout << "Deleted Render API" << std::endl; }
	void Render(Game* currentGame);
	void RenderPause(Game* currentGame);
	void RenderGameOver(Game* currentGame);
	void RenderBullets(sf::RenderWindow& mWindow, std::list<Bullet>& bulletList);
	void RenderEnemies(sf::RenderWindow& mWindow, std::list<Enemy>& enemies);
	void RenderScoreBoard(Game* currentGame);
};

// Game states and state design pattern below
enum State {
	GAME_OVER,
	GAME_RUNNING,
	GAME_PAUSED
};

class GameState {
protected:
	Game* gameContext;
public:
	State state;
	GameState(State state) :
		gameContext(nullptr),
		state(state) {}
	virtual void RunState() = 0;
	virtual void Render() = 0;
	void SetContext(Game* gameContext) {
		this->gameContext = gameContext;
	}
};

class GameRunning : public GameState {
public:
	GameRunning() : GameState(GAME_RUNNING) {};
	~GameRunning() { std::cout << "Game-running state destroyed" << std::endl; }
	void RunState() override;
	void Render() override;
};

class GamePaused : public GameState {
public:
	GamePaused() : GameState(GAME_PAUSED) {};
	~GamePaused() { std::cout << "Game-paused state destroyed" << std::endl; }
	void RunState() override;
	void Render() override;
};

class GameOver : public GameState {
public:
	GameOver() : GameState(GAME_OVER) {};
	~GameOver() { std::cout << "Game-over state destroyed" << std::endl; }
	void RunState() override;
	void Render() override;
};

class Game {
private:
	sf::RenderWindow	mWindow;
	std::unique_ptr<Player>	starShip;
	std::shared_ptr<GameState> gameState;
public:
	std::shared_ptr<Render_API>	renderAPI;
	Utility::ScoreBoard& scoreBoard;
	sf::Font gameTextFont; // font for text display
	Game(std::shared_ptr<Render_API> renderAPI,
		Utility::ScoreBoard& scoreBoard) :
		mWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_LENGTH), "Star Ship game"),
		renderAPI(renderAPI),
		scoreBoard(scoreBoard),
		starShip(std::make_unique<Player>()),
		gameState(std::make_shared<GameRunning>())
	{
		if (!gameTextFont.loadFromFile("resources\\arial.ttf")) {} // Load font
		this->gameState->SetContext(this); // the context of the state has to be set in the beginning
	}
	const std::shared_ptr<GameState>& GetGameState() const { return gameState; }
	void	TransitionTo(std::shared_ptr<GameState> state);
	void	PollEvents();
	void	PollPauseEvent();
	void	Run();
	sf::RenderWindow& GetWindow() { return this->mWindow; }
	std::unique_ptr<Player>& GetPlayer() { return this->starShip; }
	void	HandlePlayerInputs(sf::Keyboard::Key key);
	std::list<Bullet>	bullets;
	std::list<Enemy>	enemies;
	void	UpdateBullets();
	void	CreateEnemies();
	void	UpdateEnemies();
	void	UpdateScoreBoard();
	void	CheckGameOver();
};

// A single function to toggle between pause and run
inline void ExecuteSpaceKeyEvent(Game* currentGame) {
	(currentGame->GetGameState()->state == GAME_PAUSED) ?
		currentGame->TransitionTo(std::make_shared<GameRunning>()) :
		currentGame->TransitionTo(std::make_shared<GamePaused>());
}