#include "starShip.h"

int main() {
	srand((unsigned)time(NULL)); // seed for random number generation
	std::shared_ptr<Render_API> renderAPI = std::make_shared<Render_API>();
	std::unique_ptr<Game> game = std::make_unique<Game>(renderAPI);
	game->Run();
	return 0;
}