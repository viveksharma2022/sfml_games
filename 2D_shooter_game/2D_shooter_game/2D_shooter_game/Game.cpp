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
	this->GetPlayerHost().setPosition(newPosition);
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
		case sf::Event::KeyPressed:
			this->gameContext->GetPlayer()->HandlePlayerInputs(event.key.code);
			break;
			// we don't process other types of events
		default:
			break;
		}

	}

}

void Player::HandlePlayerInputs(sf::Keyboard::Key key) {
	sf::Vector2f playerCurrentPosition = GetPlayerPosition();
	if (key == sf::Keyboard::Left) {
		this->SetPlayerPosition({ Utility::BoundCheck(playerCurrentPosition,
			{playerCurrentPosition.x - 1.0f,playerCurrentPosition.y}) });
	}
	else if (key == sf::Keyboard::Right) {
		this->SetPlayerPosition({ Utility::BoundCheck(playerCurrentPosition,
					{playerCurrentPosition.x + 1.0f,playerCurrentPosition.y }) });
	}
	else if (key == sf::Keyboard::Up) {
		this->SetPlayerPosition({ Utility::BoundCheck(playerCurrentPosition,
					{playerCurrentPosition.x,playerCurrentPosition.y - 1.0f }) });
	}
	else if (key == sf::Keyboard::Down) {
		this->SetPlayerPosition({ Utility::BoundCheck(playerCurrentPosition,
					{playerCurrentPosition.x,playerCurrentPosition.y + 1.0f }) });
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

Orientation Utility::SwitchOrientation(Orientation currentOrientation) {
	return currentOrientation == LEFT ? RIGHT : LEFT;
}

const sf::Vector2f& Utility::BoundCheck(const sf::Vector2f& currentValue, const sf::Vector2f& newValue) {
	// restrict values within the game windowss
	return  (newValue.x >= 0.0f && newValue.x <= MAP_WIDTH - TILE_WIDTH - 1
		&& newValue.y >= 0.0f && newValue.y <= MAP_HEIGHT - TILE_HEIGHT - 1) ? newValue : currentValue;
}