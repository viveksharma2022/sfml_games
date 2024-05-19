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
            // we don't process other types of events
        default:
            break;
        }
    }
}

void Game:: Run() {
    while (mWindow.isOpen()) {
        PollEvents();
        renderAPI.Render(mWindow);
    }
}

void Render_API::Render(sf::RenderWindow& mWindow) {
    mWindow.display();
}