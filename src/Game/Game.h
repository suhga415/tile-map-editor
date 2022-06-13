#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <memory>
#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../EventBus/EventBus.h"

class Game {
  private:
    bool isRunning;
		int ticksLastFrame;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Rect camera;
    SDL_Rect canvas;
    std::unique_ptr<Registry> registry;
    std::unique_ptr<AssetStore> assetStore;
    std::shared_ptr<EventBus> eventBus;

  public:
    Game();
    ~Game();
    void initialize();
    void run();
    void loadLevel(int level);
    void setup();
    void processInput();
    void update();
    void render();
    void destroy();
};

#endif
