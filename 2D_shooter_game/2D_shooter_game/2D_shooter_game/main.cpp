#include "map.h"
#include "Game.h"

sf::Clock globalClock;

int main() {

	// To be modified
	std::shared_ptr<App> app = std::make_shared<App>();
	app->RunApp();
	return 0;
}