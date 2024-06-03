#pragma once
#include <iostream>
#include "sfml\Graphics.hpp"
#include "Map.h"

class Game;
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
	std::vector <MapTile> tiles;
	std::unique_ptr<GameState> gameState;
public:
	Game() :
		gameState(std::move(std::make_unique<GameRunning>())) {
		InitializeMap();
	}
	void InitializeMap();
};
