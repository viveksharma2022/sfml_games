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
	///////////////// movement parts ////////////////////
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

	///////////////shooting part ///////////////
	if (key == sf::Keyboard::LShift) {
		this->bullets.emplace_back(sf::Vector2f({ this->GetPosition().x,this->GetPosition().y + PLAYER_HEIGHT / 2 }),
			this->GetPlayerOrientation(), 
			sf::Vector2f({ PLAYER_BULLET_VELOCITY ,PLAYER_BULLET_VELOCITY }));
	}
}

void GameRunning::RenderGame() {
	this->gameContext->appReference->mWindow.clear();
	Utility::RenderMap(this->gameContext->appReference->mWindow,
		this->gameContext->tiles);
	Utility::RenderPlayer(this->gameContext->appReference->mWindow,
		this->gameContext->GetPlayer());
	Utility::RenderBullets(this->gameContext);
	this->gameContext->appReference->mWindow.display();
}

void GameRunning::RunGame() {
	PollEvents();
	if (!this->state == GAME_RUNNING) { return; }
	this->gameContext->CheckBoundaryConditionsForPlayer();
	this->gameContext->GetPlayer()->PlayerUpdatePosition();
	this->gameContext->GetPlayer()->PlayerDampenVelocity();
	Utility::UpdateBullets(this->gameContext);
	// boundary conditions currently checks for crossing the game window
	// or collding with opaque objects
	Utility::ApplyBoundaryConditionsToBullets(this->gameContext); 
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
	if (Utility::IsExceedingBoundary({PLAYER_WIDTH,PLAYER_HEIGHT}, this->player->GetPosition() + this->player->velocity)
		|| Utility::IsTouchingOpaques(sf::FloatRect(this->player->GetPosition() + this->player->velocity, 
			this->player->GetPlayerHost().getSize()), opaqueTiles)) {
		//reset velocity if it hits image boundary or hits opaque objects,
		// avoids velocity increment and therefore 
		// also avoids lag in movement after boundary hit
		this->player->velocity = { 0.0f,0.0f };
	}
}

void Game::GetAllOpaqueObjects() {
	std::for_each(tiles.begin(), tiles.end(), [&](const MapTile& tile) {
		if (tile.GetObjectPenetrationType() == OPAQUE) {
			opaqueTiles.emplace_back(tile);
		}
		});
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

bool Utility::IsExceedingBoundary(const sf::Vector2f& entityDimension, const sf::Vector2f& newValue) {
	// restrict values within the game windowss
	bool val =  (newValue.x >= 1.0f && newValue.x <= (MAP_WIDTH - entityDimension.x -1.0f)
		&& newValue.y >= 1.0f && newValue.y <= (MAP_HEIGHT - entityDimension.y - 1.0f)) ? false : true;

	if (val) { std::cout << "Exceeding border" << "\n"; }
	return val;
}

bool Utility::IsTouchingOpaques(const sf::FloatRect& hostBox, const std::vector<MapTile>& opaqueTiles) {
	// checks if a given host-box intersects with any of the opaque tiles
	bool val = std::any_of(opaqueTiles.begin(), opaqueTiles.end(), [&](const MapTile& opaqueTile) {
		return opaqueTile.GetHost().getGlobalBounds().intersects(hostBox) ? true : false;
		});

	if (val) { std::cout << "Touching Opaque" << "\n"; }
	return val;
}

void Utility::RenderBullets(Game* game) {
	// Render player bullets
	for (const auto& pB : game->GetPlayer()->bullets) {
		game->appReference->mWindow.draw(pB.host);
	}
}

void Utility::UpdateBullets(Game* game) {
	float deltaTime			 = globalClock.restart().asSeconds(); // get delta time
	// update player bullets
	for (auto& pB : game->GetPlayer()->bullets) {
		// update position
		pB.position.x = pB.position.x - pB.velocity.x * deltaTime;	// bullet only moves horizontally
		pB.host.setPosition(pB.position);
	}
}

void Utility::ApplyBoundaryConditionsToBullets(Game* game) {
	// checks for boundary conditions and destroys bullets if satisfied
	// first check player boundary conditions
	for (auto& pB : game->GetPlayer()->bullets) {
		if (IsExceedingBoundary(sf::Vector2f({ BULLET_WIDTH,BULLET_HEIGHT }), pB.position)
			|| IsTouchingOpaques(pB.host.getGlobalBounds(), game->opaqueTiles)) {
			pB.isExist = false;
		}	
	}
	game->GetPlayer()->bullets.erase(
		std::remove_if(game->GetPlayer()->bullets.begin(), game->GetPlayer()->bullets.end(), 
			[](const Bullet& b) { return !b.isExist; }),
		game->GetPlayer()->bullets.end()); // remove player bullets if not exist
}