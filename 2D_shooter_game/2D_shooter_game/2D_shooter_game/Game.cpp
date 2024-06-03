#include "Game.h"

void Game::InitializeMap() {
	for (int x = 0; x < MAP_WIDTH; x++) {
		for (int y = 0; y < MAP_HEIGHT; y++) {
			switch (map[x][y]) {
				case(GRASS):
					this->tiles.push_back(Grass({ static_cast<float>(x * TILE_WIDTH),static_cast<float>(y * TILE_HEIGHT) },
						""));
					break;
			}
		}
	}
}