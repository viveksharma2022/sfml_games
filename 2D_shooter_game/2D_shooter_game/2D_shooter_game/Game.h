#pragma once
#include <iostream>
#include "sfml\Graphics.hpp"
#include "Map.h"
#include "Utility.h"

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

class Game {
private:
	std::shared_ptr<GameState> gameState;
public:
	App* appReference; // a reference of app is always provided to all submodules
	std::vector <MapTile> tiles;
	Game() :
		appReference(nullptr),
		gameState(std::make_shared<GameRunning>()) // TODO: set the game state to DO_NOTHING
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
