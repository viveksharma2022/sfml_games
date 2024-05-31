#include "starShip.h"

void Game::Run() {
    while (mWindow.isOpen()) {
        this->gameState->Render();
        this->gameState->RunState();
    }
}

void Game::TransitionTo(std::shared_ptr<GameState> newState) {
    if (this->gameState) {
        gameState.reset();
    }
    this->gameState = newState;
    this->gameState->SetContext(this);
}

void GameRunning::RunState() {
    this->gameContext->CheckGameOver();
    this->gameContext->PollEvents();
    Utility::CollisionCheck(this->gameContext->enemies, this->gameContext->bullets);
    this->gameContext->UpdateScoreBoard();
    this->gameContext->UpdateBullets();
    this->gameContext->CreateEnemies();
    this->gameContext->UpdateEnemies();
}

void GamePaused::RunState() {
    this->gameContext->PollPauseEvent();
    globalClock.restart(); // reset clock (dt) otherwise it impacts enemy movement, enemy distance = dt * velocity
}

void GameOver::RunState() {
    Utility::CheckWindowClosed(this->gameContext);
}

void GameRunning::Render() {
    this->gameContext->renderAPI->Render(this->gameContext);
}

void GamePaused::Render() {
    this->gameContext->renderAPI->Render(this->gameContext);
    this->gameContext->renderAPI->RenderPause(this->gameContext);
}

void GameOver::Render() {
    this->gameContext->renderAPI->Render(this->gameContext);
    this->gameContext->renderAPI->RenderGameOver(this->gameContext);
}

void Game::PollEvents() {
	sf::Event event;
    while (mWindow.pollEvent(event))
    {
        // check the type of the event...
        switch (event.type)
        {
            // window closed
        case sf::Event::Closed:
            mWindow.close();
            break;
        case sf::Event::KeyPressed:
            HandlePlayerInputs(event.key.code);
            break;
            // we don't process other types of events
        default:
            break;
        }
    }
}

void Game::PollPauseEvent() {
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        // check the type of the event...Handle only pause event
        switch (event.type)
        {
            // window closed
        case sf::Event::Closed:
            mWindow.close();
            break;
        case (sf::Event::KeyPressed):
            if (event.key.code == sf::Keyboard::Space) { // handle only space button
                ExecuteSpaceKeyEvent(this);
            }
            break;
            // we don't process other types of events
        default:
            break;
        }
    }
}

void Game::HandlePlayerInputs(sf::Keyboard::Key key) {
    sf::Vector2f playerCurrentPosition = starShip->GetPlayerPosition();
    if (key == sf::Keyboard::Left) {
         starShip->SetPlayerPosition({Utility::BoundCheck<float>(playerCurrentPosition.x, playerCurrentPosition.x - 1.0f * MOV_RESOLUTION, static_cast<float>(WINDOW_WIDTH - MOV_RESOLUTION - 1)),
            playerCurrentPosition.y });
    }
    else if (key == sf::Keyboard::Right) {
        starShip->SetPlayerPosition({ Utility::BoundCheck<float>(playerCurrentPosition.x, playerCurrentPosition.x + 1.0f * MOV_RESOLUTION, static_cast<float>(WINDOW_WIDTH - MOV_RESOLUTION - 1)),
            playerCurrentPosition.y });
    }
    else if (key == sf::Keyboard::LShift) {
        Bullet blt;
        blt.position = playerCurrentPosition + sf::Vector2f{PLAYER_WIDTH/2, -1.0f};
        blt.host.setPosition(blt.position);
        bullets.push_back(blt);
    }
    else if (key == sf::Keyboard::Space) {
        ExecuteSpaceKeyEvent(this);
    }
}

void Player::SetPlayerPosition(sf::Vector2f newPosition) {
    this->position = newPosition;
    this->playerHost.setPosition(this->position);
}

const sf::Vector2f& Player::GetPlayerPosition() {
    return this->position;
}

void Game::UpdateScoreBoard() {
    // add to the score if an enemy is killed
    for (auto& e : enemies) {
        if (!e.isNotCollided) {
            scoreBoard.AddScore(e.killScore);
        }
    }
}

void Game::CheckGameOver() {
    if (std::any_of(enemies.begin(), enemies.end(),
        [](const Enemy& enemyPosition) {
            return (enemyPosition.GetEnemyPosition().y >= (WINDOW_LENGTH - SCORE_BOARD_HEIGHT - PLAYER_HEIGHT - 1)) ? true : false;
        })) {
        TransitionTo(std::make_shared<GameOver>());
    }
}

void Game::CreateEnemies() {
    while (enemies.size() < MAX_ENEMIES) {
        enemies.push_back(Enemy());
    }
}

void Game::UpdateBullets() {
    for (auto& b : bullets) {
        Utility::BorderCheck(b);
        if (b.isExist) {
            // update position if bullet exists
            b.position += sf::Vector2f{0.0f, -1.0f};
            b.host.setPosition(b.position);
        }
    }
    // clear bullet if it does not exist
    bullets.remove_if(Utility::CheckNotExists<Bullet>);
}

void Game::UpdateEnemies() {
    float deltaTime = globalClock.restart().asSeconds(); // get delta time
        for (auto& e : enemies) {
            Utility::BorderCheck(e);
            if (e.isExist) {
                //update position of the enemy based on it's velocity  
                e.position += {0.0f, e.velocity * deltaTime};
                e.host.setPosition(e.position);
            }        
        }
    // clear enemy if it does not exist
    enemies.remove_if(Utility::CheckNotExists<Enemy>);
}

void Render_API::Render(Game* currentGame) {
    currentGame->GetWindow().clear();
    currentGame->GetWindow().draw(currentGame->GetPlayer()->GetPlayerHost()); // render player
    RenderBullets(currentGame->GetWindow(), currentGame->bullets);
    RenderEnemies(currentGame->GetWindow(), currentGame->enemies);
    RenderScoreBoard(currentGame);
    currentGame->GetWindow().display();
}

void Render_API::RenderPause(Game* currentGame) {
    currentGame->GetWindow().clear();
    static sf::Text text("Pause!", currentGame->gameTextFont, 50);
    // set the text style
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    currentGame->GetWindow().draw(text);
    currentGame->GetWindow().display();
}

void Render_API::RenderGameOver(Game* currentGame) {
    currentGame->GetWindow().clear();
    static sf::Text text("GAME OVER!", currentGame->gameTextFont, 50);
    // set the text style
    text.setStyle(sf::Text::Bold);
    text.setPosition(sf::Vector2f{ 10.0f, static_cast<float>(WINDOW_LENGTH / 2) });
    text.setStyle(sf::Text::Bold);
    currentGame->GetWindow().draw(text);
    currentGame->GetWindow().display();
}

void Render_API::RenderBullets(sf::RenderWindow& mWindow, std::list<Bullet>& bulletList) {
    for (auto& b : bulletList) {
        mWindow.draw(b.host);
    }
}

void Render_API::RenderEnemies(sf::RenderWindow& mWindow, std::list<Enemy>& enemies) {
    for (auto& e : enemies) {
        mWindow.draw(e.host);
    }
}

void Render_API::RenderScoreBoard(Game* currentGame) {
    sf::Text text("SCORE: " + std::to_string(currentGame->scoreBoard.GetScore()), currentGame->gameTextFont, 30);
    text.setPosition(currentGame->scoreBoard.GetScoreBoardPosition() + sf::Vector2f{0.0f, 20.0f});
    text.setStyle(sf::Text::Bold);
    currentGame->GetWindow().draw(text);
}

void Utility::CollisionCheck(std::list<Enemy>& enemies, std::list<Bullet>& bullets) {
    // check collision of enemies with bullets
    for (auto& e : enemies) {
        // fine optimization may be needed, as early stopping is not enabled in std::any_of, it goes through all the list of bullets,
        // a handwriiten function may be necessary
        std::any_of(bullets.begin(), bullets.end(), [&e](Bullet& b) {
            if (e.GetHost().getGlobalBounds().intersects(b.host.getGlobalBounds())) {
                // destroy bullet and enemy
                e.isNotCollided = false;
                b.isNotCollided = false;
                return true;
            }
            else {
                return false;
            }
            });
    }
}

inline void Utility::CheckWindowClosed(Game* currentGame) {
    // poll only for window close event
    sf::Event event;
    while (currentGame->GetWindow().pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            currentGame->GetWindow().close();
        }
    }
}
