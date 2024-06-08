#pragma once
#define TILE_WIDTH 50
#define TILE_HEIGHT 50
#define MAP_GRIDS_X 10
#define MAP_GRIDS_Y 10 
#define MAP_WIDTH MAP_GRIDS_X*TILE_WIDTH
#define MAP_HEIGHT MAP_GRIDS_Y*TILE_HEIGHT
#define PLAYER_WIDTH (TILE_WIDTH-20)
#define PLAYER_HEIGHT (TILE_HEIGHT-20)
#define PLAYER_STEP_ACCELERATION 0.1f // acceleration-decelration step for each movement button-pressed
#define PLAYER_DAMPENING_COEFFICIENT 0.99f //velocity dampening factor for each non-movement button-pressed-iteration
#define BULLET_WIDTH 10
#define BULLET_HEIGHT 5
#define PLAYER_BULLET_VELOCITY 500 // pixels per sec
