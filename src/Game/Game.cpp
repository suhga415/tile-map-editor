#include "./Game.h"
#include "../Constants.h"
#include "../Logger/Logger.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TileSetComponent.h"
#include "../Components/SelectedTileComponent.h"
#include "../Components/CanvasComponent.h"
#include "../Components/LoadedTileSetsComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/KeyboardControlSystem.h"
#include "../Systems/GUISystem.h"
#include "../Systems/ChangeTileSystem.h"
#include "../Systems/EditCanvasSystem.h"
#include "../Systems/CursorMovementSystem.h"
#include "../Systems/RenderCursorSystem.h"
#include "../Events/KeyPressedEvent.h"
#include <fstream>
#include <glm/glm.hpp>

Game::Game() {
  this->isRunning = false;
  registry = std::make_unique<Registry>();
  assetStore = std::make_unique<AssetStore>();
  eventBus = std::make_shared<EventBus>();
  camera = std::make_shared<SDL_Rect>();
  Logger::Info("Game constructor called.");
}

Game::~Game() {
  Logger::Info("Game destructor called.");
}

void Game::initialize() {
  if (SDL_Init( SDL_INIT_EVERYTHING ) != 0) {
		Logger::Error("Error initializing SDL.");
		return;
	}
  if (TTF_Init() != 0) {
		Logger::Error("Error initializing SDL TTF.");
		return;
	}
  if (Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0) {
		Logger::Error("Error initializing SDL Mixer.");
		return;
	}

  this->window = SDL_CreateWindow(
		NULL, 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_BORDERLESS
	);
	if (!window) {
		Logger::Error("Error creating SDL window.");
		return;
	}

	this->renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		Logger::Error("Error creating SDL renderer.");
		return;
	}

  // initialize ImGui context
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();
  // setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer_Init(renderer);

  // initialize the camera view with the screen area
  camera->x = 0;
  camera->y = 0;
  camera->w = WINDOW_WIDTH;
  camera->h = WINDOW_HEIGHT - WINDOW_MENUBAR_HEIGHT;
  
  this->isRunning = true;
  return;
}

void Game::run() {
  setup();
  while(this->isRunning) {
    processInput();
    update();
    render();
  }
}

void Game::setup() {
  loadLevel(1);
}

void Game::loadLevel(int level) {
  // add systems
  registry->addSystem<MovementSystem>();
  registry->addSystem<RenderSystem>();
  registry->addSystem<KeyboardControlSystem>();
  registry->addSystem<GUISystem>();
  registry->addSystem<ChangeTileSystem>();
  registry->addSystem<EditCanvasSystem>();
  registry->addSystem<CursorMovementSystem>();
  registry->addSystem<RenderCursorSystem>();

  // add textures
  assetStore->addTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
  assetStore->addTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
  // Some textures for tile map
  // ./assets/tilemaps/field.png
  // ./assets/tilemaps/jungle.png

  // tileset GUI (palette)
  Entity gui = registry->createEntity();
  gui.addComponent<TileSetComponent>(); // "tilemap-image", 32, 10, 3, 1.5
  gui.addComponent<LoadedTileSetsComponent>();

  // selected tile (mouse cursor)
  Entity selectedTile = registry->createEntity();
  selectedTile.addComponent<CursorPosComponent>(glm::vec2(0, 0), 0, true);
  selectedTile.addComponent<SelectedTileComponent>();

  // canvas
  Entity canvasEntity = registry->createEntity();
  canvasEntity.addComponent<CanvasComponent>(CANVAS_X, CANVAS_Y); // 62, 20, 10, 1
  canvasEntity.addComponent<SelectedTileComponent>();
}

void Game::processInput() {
  SDL_Event event;
  
  while (SDL_PollEvent(&event)) {
    // handle ImGui SDL input
    ImGui_ImplSDL2_ProcessEvent(&event);
    static int mouseX, mouseY;
    static bool rightMouseButtonDown = false;
    // ImGuiIO& io = ImGui::GetIO();
    // const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
    // io.MousePos = ImVec2(mouseX, mouseY);
    // io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
    // io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

    // handle core SDL events
    switch (event.type) {
      case SDL_QUIT: 
        isRunning = false;
        break;
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          isRunning = false;
        }
        eventBus->emitEvent<KeyPressedEvent>(event.key.keysym.sym);
        break;
      case SDL_MOUSEBUTTONDOWN:
        switch (event.button.button) {
          case SDL_BUTTON_LEFT:
            SDL_GetMouseState(&mouseX, &mouseY);
            eventBus->emitEvent<MouseClickEvent>(mouseX, mouseY, camera);
            break;
          case SDL_BUTTON_RIGHT:
            rightMouseButtonDown = true; // to detect drag motion
            SDL_GetMouseState(&mouseX, &mouseY);
            break;
          default:
            break;
        }
        break;
      case SDL_MOUSEBUTTONUP:
        if (event.button.button == SDL_BUTTON_RIGHT) {
          rightMouseButtonDown = false;
        }
        break;
      case SDL_MOUSEMOTION:
        if (rightMouseButtonDown) {
          eventBus->emitEvent<MouseDragEvent>(mouseX - event.motion.x, mouseY - event.motion.y, camera);
          SDL_GetMouseState(&mouseX, &mouseY);
        }
        break;
      default:
        break;
    }
  }
}

void Game::update() {
  int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - ticksLastFrame);
	if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME)
		SDL_Delay( timeToWait );

	// deltaTime: the difference in ticks from the last frame (converted to seconds)
	float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;
	
	// clamp deltaTime to a maximum value
	deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

	// set the new ticks for the current frame to be used in the next pass
	ticksLastFrame = SDL_GetTicks(); // current real time in milliseconds

  // systems subscribe to events (every frame)
  eventBus->resetSubscribers();
  registry->getSystem<KeyboardControlSystem>().subscribeToEvents(eventBus);
  registry->getSystem<CursorMovementSystem>().subscribeToEvents(eventBus);
  registry->getSystem<ChangeTileSystem>().subscribeToEvents(eventBus);
  registry->getSystem<EditCanvasSystem>().subscribeToEvents(eventBus);

  // update registry to process adding / killing entities
  registry->update();

	// systems update
  registry->getSystem<MovementSystem>().update(deltaTime);
  registry->getSystem<CursorMovementSystem>().update(ImGui::GetMousePos().x, ImGui::GetMousePos().y, camera);
  // registry->getSystem<CollideSystem>().update(deltaTime);

}

void Game::render() {
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

  // systems render
  registry->getSystem<EditCanvasSystem>().update(renderer, assetStore, camera);
  registry->getSystem<GUISystem>().update(renderer, assetStore, eventBus);
  registry->getSystem<RenderSystem>().update(renderer, assetStore);
  registry->getSystem<RenderCursorSystem>().update(renderer, assetStore);

	SDL_RenderPresent(renderer); // swap the buffer
}

void Game::destroy() {
  // ImGui cleanup
  ImGui_ImplSDLRenderer_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
