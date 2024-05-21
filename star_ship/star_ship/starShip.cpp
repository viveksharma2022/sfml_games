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
    else if (key == sf::Keyboard::LShift) {
        Bullet blt;
        blt.position = playerCurrentPosition + sf::Vector2f{PLAYER_WIDTH/2, -1.0f};
        blt.host.setPosition(blt.position);
        bulletS.push_back(blt);
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
        UpdateBullets();
        this->renderAPI->Render(this);
    }
}

void Render_API::Render(Game* currentGame) {
    currentGame->GetWindow().clear();
    currentGame->GetWindow().draw(currentGame->GetPlayer()->GetPlayerHost());
    RenderBullets(currentGame->GetWindow(), currentGame->bulletS);
    currentGame->GetWindow().display();
}

void Game::UpdateBullets() {
    for (auto& b : bulletS) {
        Utility::BorderCheck(b);
        Utility::CollisionCheck(b);
        if (b.isExist) {
            // update position if bullet exists
            b.position += sf::Vector2f{0.0f, -1.0f};
            b.host.setPosition(b.position);
        }
    }
    // clear bullet if it does not exist
    bulletS.remove_if(Utility::CheckNotExists);
}

void Render_API::RenderBullets(sf::RenderWindow& mWindow, std::list<Bullet>& bulletList) {
    for (auto& b : bulletList) {
        mWindow.draw(b.host);
    }
}

void Utility::BorderCheck(Bullet& b) {
    // check if position of bullets exceeds boundaries
    if (b.position.x < 0 || b.position.x > WINDOW_WIDTH
        || b.position.y < 0 || b.position.y > WINDOW_LENGTH) {
        b.isExist = false;
    }
}

void Utility::CollisionCheck(Bullet& b){
    // TO BE ADDED
};

bool Utility::CheckNotExists(Bullet& b) { 
    return !b.isExist; 
}

