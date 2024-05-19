#include "starShip.h"

int main() {

	Game game(std::move(Render_API()));
	game.Run();
	return 0;
}