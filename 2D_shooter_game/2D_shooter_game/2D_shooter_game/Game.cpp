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
				case(BLANK):
					this->tiles.push_back(Blank({ static_cast<float>(y * TILE_HEIGHT),static_cast<float>(x * TILE_WIDTH) }));
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
	// player movements are dependent on the velocity component
	// velocity is proportional to the acceleration provided, i.e.,
	// every movement-button pressed, adds accelaration vector and 
	// which is accumulated to the velocity. A no button press would 
	// normally dampen the velocity and is implemented in anoher
	// following function call
	//
	sf::Vector2f acceleration;
	// movement parts
	if (key == sf::Keyboard::Left) {
		acceleration.x -= PLAYER_STEP_ACCELERATION;
	}
	else if (key == sf::Keyboard::Right) {
		acceleration.x += PLAYER_STEP_ACCELERATION;
	}
	else if (key == sf::Keyboard::Up) {
		acceleration.y -= PLAYER_STEP_ACCELERATION;
	}
	else if (key == sf::Keyboard::Down) {
		acceleration.y += PLAYER_STEP_ACCELERATION;
	}
	// accumulate to velocity
	velocity += acceleration;
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
	this->gameContext->CheckBoundaryConditionsForPlayer();
	this->gameContext->GetPlayer()->PlayerUpdatePosition();
	this->gameContext->GetPlayer()->PlayerDampenVelocity();
	RenderGame();
}


void Player::PlayerUpdatePosition() {
	sf::Vector2f playerCurrentPosition = GetPosition();
	//add velocity to the current position
	this->SetPlayerPosition(playerCurrentPosition + velocity);
}

void Player::PlayerDampenVelocity() {
	// dampen the velocity if nothing is pressed, normal act of 
	// decelerations
	velocity.x *= PLAYER_DAMPENING_COEFFICIENT; 
	velocity.y *= PLAYER_DAMPENING_COEFFICIENT;
}

void Game::CheckBoundaryConditionsForPlayer() {
	if (Utility::IsExceedingBoundary(this->player->GetPosition(), this->player->GetPosition() + this->player->velocity)
		|| Utility::IsTouchingOpaques(this->player->GetPosition() + this->player->velocity, tiles)) {
		//reset velocity if it hits image boundary or hits opaque objects,
		// avoids velocity increment and therefore 
		// also avoids lag in movement after boundary hit
		this->player->velocity = { 0.0f,0.0f };
	}
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

bool Utility::IsExceedingBoundary(const sf::Vector2f& currentValue, const sf::Vector2f& newValue) {
	// restrict values within the game windowss
	return  (newValue.x >= 1.0f && newValue.x <= MAP_WIDTH - TILE_WIDTH - 1
		&& newValue.y >= 1.0f && newValue.y <= MAP_HEIGHT - TILE_HEIGHT - 1) ? false : true;
}

bool Utility::IsTouchingOpaques(const sf::Vector2f& position, const std::vector<MapTile>& tiles) {
	// TODO: does not work, change it with intersects with all opaque objects
	std::vector<sf::Vector2i> localTiles;
	sf::Vector2f tilePosition = { position.x / TILE_WIDTH, position.y / TILE_HEIGHT };
	sf::Vector2i tileCoordinate = sf::Vector2i({ static_cast<int>(std::floor(tilePosition.x)),
		static_cast<int>(std::floor(tilePosition.y)) });
	localTiles.emplace_back(tileCoordinate);

	//if (std::round(tilePosition.x - 0.5) != tilePosition.x) {
	//	localTiles.emplace_back(tileCoordinate + sf::Vector2i({ 1,0 }));
	//}
	//if (std::round(tilePosition.y) != tilePosition.y) {
	//	localTiles.emplace_back(tileCoordinate + sf::Vector2i({ 0,1 }));
	//}
	//if (std::round(tilePosition.x) != tilePosition.x && std::round(tilePosition.y) != tilePosition.y) {
	//	localTiles.emplace_back(tileCoordinate + sf::Vector2i({ 1,1 }));
	//}

	localTiles.erase(
		std::remove_if(
			localTiles.begin(), localTiles.end(),
			[](const sf::Vector2i& tile) {
				return (tile.x >= 0 && tile.x < MAP_GRIDS_X && tile.y >= 0 && tile.y < MAP_GRIDS_Y) ? false : true; }),
				localTiles.end());

	bool val = std::any_of(localTiles.begin(), localTiles.end(), [&](const sf::Vector2i& tile) {
		return tiles[tile.y * MAP_GRIDS_Y + tile.x].GetObjectPenetrationType() == OPAQUE;
		});

	return val;
}