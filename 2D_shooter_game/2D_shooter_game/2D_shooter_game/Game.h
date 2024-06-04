#pragma once
#include <iostream>
#include "sfml\Graphics.hpp"
#include "Map.h"
#include "Utility.h"

const std::string playerTexFile = "assets\\player.png";
static sf::Texture playerTexture;

class Game;
class App;

enum GameStates {
	GAME_RUNNING,
	GAME_PAUSED,
	GAME_OVER,
	DO_NOTHING
};

class GameState {
protected:
	Game* gameContext;
public:
	GameStates state;
	GameState(GameStates state) :
		gameContext(nullptr),
		state(state) {}
	~GameState() = default;
	virtual void RunGame() = 0;
	virtual void RenderGame() = 0;
	virtual void PollEvents() = 0;
	void SetContext(Game* gameContext) {
		this->gameContext = gameContext;
	}
};

class GameRunning : public GameState {
public:
	GameRunning() :GameState(GAME_RUNNING) {}
	~GameRunning() { std::cout << "Game-Running state destroyed" << "\n"; }
	void RunGame();
	void RenderGame();
	void PollEvents();
};

class Player {
private:
	sf::RectangleShape host; // host position of the player on the screen
	sf::Vector2f position;
public:
	Player() :
		// x-position is initialized to TILE_WIDTH, because the 
		// texture is flipped and the origin is now at top right instead
		// of top left
		position({TILE_WIDTH, MAP_HEIGHT-2*TILE_HEIGHT }), 
		host(sf::Vector2f(PLAYER_WIDTH, PLAYER_HEIGHT))
	{
		host.setPosition(position); // set the initial position of the player
		if (playerTexture.loadFromFile(playerTexFile)) {
			host.setTexture(&playerTexture);// set the texture for the player
		}
		// flip texture horizontally, as player image is facing left
		// origin is now at top-right
		host.setScale(-1, 1);  
	}
	~Player() {
		std::cout << "Player destructed" << std::endl;
	}
	// utlity functions for player
	const sf::RectangleShape& GetPlayerHost() const { return host; }
	void SetPlayerPosition(sf::Vector2f newPosition);
	const sf::Vector2f& GetPlayerPosition() const { return position; };
};

class Game {
private:
	std::shared_ptr<GameState> gameState;
	std::unique_ptr<Player> player;
public:
	App* appReference; // a reference of app is always provided to all submodules
	std::vector <MapTile> tiles;
	Game() :
		appReference(nullptr),
		gameState(std::make_shared<GameRunning>()), // TODO: set the game state to DO_NOTHING
		player(std::make_unique<Player>())
	{
		this->gameState->SetContext(this); // the context of the state has to be set in the beginning
		InitializeMap();
	}
	~Game() { std::cout << "Game destroyed" << "\n"; }
	void InitializeMap();
	void TransitionTo(std::unique_ptr<GameState>&& state);
	const std::shared_ptr<GameState>& GetGameState() const { return gameState; }
	void SetAppReference(App* appReference) {
		this->appReference = appReference;
	}
	const std::unique_ptr<Player>& GetPlayer() const { return player; }
};

class App {
public:
	sf::RenderWindow	mWindow;
	std::shared_ptr<Game> game;
	App():
		game(std::make_shared<Game>()),
		mWindow(sf::VideoMode(MAP_WIDTH, MAP_HEIGHT), "2D-shooter Game")
	{
		this->game->SetAppReference(this);
	}
	~App() { std::cout << "App destroyed" << "\n"; }
	void RunApp();

};
