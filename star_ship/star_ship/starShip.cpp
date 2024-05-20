#include "starShip.h"

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

void Game::HandlePlayerInputs(sf::Keyboard::Key key) {
    sf::Vector2f playerCurrentPosition = starShip->GetPlayerPosition();
    if (key == sf::Keyboard::Left) {
        starShip->SetPlayerPosition({playerCurrentPosition.x - 1.0f * MOV_RESOLUTION, playerCurrentPosition.y});
    }
    else if (key == sf::Keyboard::Right) {
        starShip->SetPlayerPosition({ playerCurrentPosition.x + 1.0f * MOV_RESOLUTION, playerCurrentPosition.y });
    }
}

void Player::SetPlayerPosition(sf::Vector2f newPosition) {
    this->position = newPosition;
    this->playerHost.setPosition(this->position);
}

const sf::Vector2f& Player::GetPlayerPosition() {
    return this->position;
}

void Game:: Run() {
    while (mWindow.isOpen()) {
        PollEvents();
        this->renderAPI->Render(this);
    }
}

void Render_API::Render(Game* currentGame) {
    currentGame->GetWindow().clear();
    currentGame->GetWindow().draw(currentGame->GetPlayer()->GetPlayerHost());
    currentGame->GetWindow().display();
}

