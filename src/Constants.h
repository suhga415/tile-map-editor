#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SDL2/SDL.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 500;
const int WINDOW_MENUBAR_HEIGHT = 20;

const unsigned int FPS = 60;
const unsigned int FRAME_TARGET_TIME = 1000 / FPS; // time[ms] per one frame transition

const int CAMERA_X = 0;
const int CAMERA_Y = 0;
const int CAMERA_WIDTH = WINDOW_WIDTH;
const int CAMERA_HEIGHT = WINDOW_HEIGHT;

const int CANVAS_X = 0;
const int CANVAS_Y = WINDOW_MENUBAR_HEIGHT;

enum CollisionType {
	NO_COLLISION,
	PLAYER_ENEMY_COLLISION,
	PLAYER_PROJECTILE_COLLISION,
	ENEMY_PROJECTILE_COLLISION,
	PLAYER_VEGETATION_COLLISION,
	PLAYER_LEVEL_COMPLETE_COLLISION
};

enum LayerType {
	TILEMAP_LAYER = 0,
	VEGETATION_LAYER = 1,
	ENEMY_LAYER = 2,
	OBSTACLE_LAYER = 3,
	PLAYER_LAYER = 4,
	PROJECTILE_LAYER = 5,
	UI_LAYER = 6
};

const unsigned int NUM_LAYERS = 7;

const SDL_Color WHITE_COLOR = {255, 255, 255, 255};
const SDL_Color GREEN_COLOR = {0, 200, 100, 255};

#endif
