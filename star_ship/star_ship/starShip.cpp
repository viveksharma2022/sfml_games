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
        CreateEnemies();
        UpdateEnemies();
        this->renderAPI->Render(this);
    }
}

void Game::CreateEnemies() {
    while (enemies.size() < MAX_ENEMIES) {
        enemies.push_back(std::make_unique<Enemy>());
    }
}

void Render_API::Render(Game* currentGame) {
    currentGame->GetWindow().clear();
    currentGame->GetWindow().draw(currentGame->GetPlayer()->GetPlayerHost()); // render player
    RenderBullets(currentGame->GetWindow(), currentGame->bulletS);
    RenderEnemies(currentGame->GetWindow(), currentGame->enemies);
    currentGame->GetWindow().display();
}

void Game::UpdateBullets() {
    for (auto& b : bulletS) {
        Utility::BorderCheck(b);
        if (b.isExist) {
            // update position if bullet exists
            b.position += sf::Vector2f{0.0f, -1.0f};
            b.host.setPosition(b.position);
        }
    }
    // clear bullet if it does not exist
    bulletS.remove_if(Utility::CheckNotExists);
}

void Game::UpdateEnemies() {
    float deltaTime = globalClock.restart().asSeconds(); // get delta time
        for (auto& e : enemies) {
            Utility::BorderCheck(e);
            if (e->isExist) {
                //update position of the enemy based on it's velocity  
                e->position += {0.0f, e->velocity * deltaTime};
                e->host.setPosition(e->position);
            }        
        }
    // clear enemy if it does not exist
        enemies.remove_if([](const std::unique_ptr<Enemy>& e) {return !e->isExist; });
}

void Utility::BorderCheck(Bullet& b) {
    // check if position exceeds boundaries
    // host reference point is top left corner
    const static sf::Vector2f hostSize = b.host.getSize();
    if (b.position.x < 0 || (b.position.x + hostSize.y) > WINDOW_WIDTH
        || (b.position.y) < 0 || (b.position.y + hostSize.x) > WINDOW_LENGTH) {
        b.isExist = false;
    }
}

// TODO: template out border checks, unnecessary code duplication
void Utility::BorderCheck(std::unique_ptr<Enemy>& e) {
    // check if position exceeds boundaries
    // host reference point is top left corner
    const static sf::Vector2f hostSize = e->host.getSize();
    if (e->position.x < 0 || (e->position.x + hostSize.y) > WINDOW_WIDTH
        || e->position.y < 0 || (e->position.y + hostSize.x) > WINDOW_LENGTH) {
        e->isExist = false;
    }
}

void Render_API::RenderBullets(sf::RenderWindow& mWindow, std::list<Bullet>& bulletList) {
    for (auto& b : bulletList) {
        mWindow.draw(b.host);
    }
}

void Render_API::RenderEnemies(sf::RenderWindow& mWindow, std::list<std::unique_ptr<Enemy>>& enemies) {
    for (auto& e : enemies) {
        mWindow.draw(e->host);
    }
}

void Utility::CollisionCheck(Bullet& b){
    // TO BE ADDED
};

bool Utility::CheckNotExists(Bullet& b) { 
    return !b.isExist; 
}

