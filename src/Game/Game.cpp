#include "./Game.h"
#include "../Constants.h"
#include "../Logger/Logger.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TileSetComponent.h"
#include "../Components/SelectedTileComponent.h"
#include "../Components/CanvasComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/KeyboardControlSystem.h"
#include "../Systems/TileSetGUISystem.h"
#include "../Systems/ChangeTileSystem.h"
#include "../Systems/EditCanvasSystem.h"
#include "../Systems/CursorMovementSystem.h"
#include "../Systems/RenderCursorSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Events/KeyPressedEvent.h"
#include <fstream>
#include <glm/glm.hpp>

Game::Game() {
  this->isRunning = false;
  registry = std::make_unique<Registry>();
  assetStore = std::make_unique<AssetStore>();
  eventBus = std::make_shared<EventBus>();
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
  camera.x = 0;
  camera.y = 0;
  camera.w = WINDOW_WIDTH;
  camera.h = WINDOW_HEIGHT;
  
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
  registry->addSystem<TileSetGUISystem>();
  registry->addSystem<ChangeTileSystem>();
  registry->addSystem<EditCanvasSystem>();
  registry->addSystem<CursorMovementSystem>();
  registry->addSystem<CameraMovementSystem>();
  registry->addSystem<RenderCursorSystem>();

  // add textures
  assetStore->addTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
  assetStore->addTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
  assetStore->addTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");
// field
// ./assets/tilemaps/field.png

  // tileset (palette)
  Entity tileSet = registry->createEntity();
  tileSet.addComponent<TileSetComponent>("tilemap-image", 32, 10, 3, 1.5);

  // selected tile (mouse cursor)
  Entity selectedTile = registry->createEntity();
  selectedTile.addComponent<CursorPosComponent>(glm::vec2(0, 0));
  selectedTile.addComponent<SelectedTileComponent>("tilemap-image", -1, -1, 32, 1);

  // canvas
  Entity canvasEntity = registry->createEntity();
  canvasEntity.addComponent<CanvasComponent>(CANVAS_X, CANVAS_Y, 32, 25, 23, 2);
  canvasEntity.addComponent<SelectedTileComponent>("tilemap-image", -1, -1, 32, 1);
  canvas.x = CANVAS_X;
  canvas.y = CANVAS_Y;
  canvas.w = 32 * 25 * 2; // tileSize * tilNumX * scale
  canvas.h = 32 * 23 * 2; // tileSize * tilNumY * scale

  // load and draw a map of tiles
  // loadMap("./assets/tilemaps/jungle.map", 32, 25, 20, 2.0);
}

void Game::loadMap(std::string filePath, int tileSize, int mapNumCols, int mapNumRows, double tileScale) {
	// Read the map tile definitions from the .map file
	std::fstream mapFile;
	mapFile.open(filePath);
	char ch;
	for (int y = 0; y < mapNumRows; y++) {
		for (int x = 0; x < mapNumCols; x++) {
			mapFile.get(ch);
			int srcRectY = std::atoi(&ch) * tileSize;
			mapFile.get(ch);
			int srcRectX = std::atoi(&ch) * tileSize;
			mapFile.ignore(); // ignore comma (,)
			
      // add new tile entity
      Entity tile = registry->createEntity();
      tile.addComponent<TransformComponent>(glm::vec2(x*tileSize*tileScale, y*tileSize*tileScale), glm::vec2(tileScale, tileScale), 0.0);
      tile.addComponent<SpriteComponent>("tilemap-image", 0, tileSize, tileSize, srcRectX, srcRectY);
		}
	}
	mapFile.close();
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
          registry->getSystem<CameraMovementSystem>().update(mouseX - event.motion.x, mouseY - event.motion.y, camera, canvas);
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
  registry->getSystem<ChangeTileSystem>().subscribeToEvents(eventBus);
  registry->getSystem<EditCanvasSystem>().subscribeToEvents(eventBus);

  // update registry to process adding / killing entities
  registry->update();

	// systems update
  registry->getSystem<MovementSystem>().update(deltaTime);
  registry->getSystem<CursorMovementSystem>().update(ImGui::GetMousePos().x, ImGui::GetMousePos().y);
  // registry->getSystem<CollideSystem>().update(deltaTime);

}

void Game::render() {
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

  // systems render
  registry->getSystem<EditCanvasSystem>().update(renderer, assetStore, camera);
  registry->getSystem<TileSetGUISystem>().update(renderer, assetStore, eventBus);
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
