#include "Game.h"

void Game::InitializeMap() {
	for (int x = 0; x < MAP_GRIDS_X; x++) {
		for (int y = 0; y < MAP_GRIDS_Y; y++) {
			switch (map[x][y]) {
				case(GRASS):
					this->tiles.push_back(Grass({ static_cast<float>(y * TILE_HEIGHT),static_cast<float>(x * TILE_WIDTH) }));
					break;
				case(BRICK):
					this->tiles.push_back(Brick({ static_cast<float>(y * TILE_HEIGHT),static_cast<float>(x * TILE_WIDTH) }));
					break;
			}
		}
	}
}

void Player::SetPlayerPosition(sf::Vector2f newPosition) {
	this->position = newPosition;
}

void Game::TransitionTo(std::unique_ptr<GameState>&& newState) {
	if (this->gameState) {
		gameState.reset();
	}
	this->gameState = std::move(newState);
	this->gameState->SetContext(this);
}


void GameRunning::PollEvents() {
	sf::Event event;
	while (this->gameContext->appReference->mWindow.pollEvent(event))
	{ 
		// check the type of the event...Handle only pause event
		switch (event.type)
		{
			// window closed
		case sf::Event::Closed:
			this->gameContext->appReference->mWindow.close();
			break;
		}
	}

}

void GameRunning::RenderGame() {
	this->gameContext->appReference->mWindow.clear();
	Utility::RenderMap(this->gameContext->appReference->mWindow,
		this->gameContext->tiles);
	Utility::RenderPlayer(this->gameContext->appReference->mWindow,
		this->gameContext->GetPlayer());
	this->gameContext->appReference->mWindow.display();
}

void GameRunning::RunGame() {
	PollEvents();
	if (!this->state == GAME_RUNNING) { return; }
	RenderGame();
}

void App::RunApp() {
	while (mWindow.isOpen()) {
		this->game->GetGameState()->RunGame();
		// ADD Menu run
	}
}

void Utility::RenderPlayer(sf::RenderWindow& mWindow, const std::unique_ptr<Player>& player) {
	mWindow.draw(player->GetPlayerHost());
}