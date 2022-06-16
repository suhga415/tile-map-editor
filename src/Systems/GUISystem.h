#ifndef GUISYSTEM_H
#define GUISYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TileSetComponent.h"
#include "../Components/LoadedTileSetsComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/SelectedTileChangedEvent.h"
#include "../Events/TileSetChangedEvent.h"
#include "../Events/CanvasCreatedEvent.h"
#include "../Events/CanvasOpenedEvent.h"
#include "../Events/CanvasPropertiesChangedEvent.h"
#include "../Utilities/MapFileLoader.h"
#include "../Structs/Tile.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_sdlrenderer.h>
#include <SDL2/SDL.h>
#include <string>
#include <vector>

class GUISystem: public System {
  private:
    bool showNewCanvasWindow;
    bool showOpenCanvasWindow;
    bool showCanvasPropertiesWindow;
    bool showTileSetWindow;
    bool showEntityWindow;

  public:
    GUISystem() {
      requireComponent<TileSetComponent>();
      requireComponent<LoadedTileSetsComponent>();
      showNewCanvasWindow = false;
      showOpenCanvasWindow = false;
      showCanvasPropertiesWindow = false;
      showTileSetWindow = false;
      showEntityWindow = false;
    }

    void update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, std::shared_ptr<EventBus>& eventBus) {
      ImGui_ImplSDLRenderer_NewFrame();
      ImGui_ImplSDL2_NewFrame();
      ImGui::NewFrame();

      if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Canvas")) {
          ImGui::MenuItem("Create New", NULL, &showNewCanvasWindow); ImGui::Spacing();
          ImGui::MenuItem("Open File", NULL, &showOpenCanvasWindow); ImGui::Spacing();
          ImGui::MenuItem("Properties", NULL, &showCanvasPropertiesWindow); ImGui::Spacing();
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tile Set")) {
          ImGui::MenuItem("Tile Set Window", NULL, &showTileSetWindow); ImGui::Spacing();
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Entity")) {
          ImGui::MenuItem("Entity Window", NULL, &showEntityWindow); ImGui::Spacing();
          ImGui::EndMenu();
        }
		    ImGui::EndMainMenuBar();
      }
      
      if (showNewCanvasWindow) {
        renderNewCanvasWindow(showNewCanvasWindow, renderer, assetStore, eventBus);
      }

      if (showOpenCanvasWindow) {
        renderOpenCanvasWindow(showOpenCanvasWindow, renderer, assetStore, eventBus);
      }

      if (showCanvasPropertiesWindow) {
        renderCanvasPropertiesWindow(showCanvasPropertiesWindow, renderer, assetStore, eventBus);
      }

      if (showTileSetWindow) {
        renderTileSetWindow(showTileSetWindow, renderer, assetStore, eventBus);
      }

      if (showEntityWindow) {
        renderEntityWindow(showEntityWindow, renderer, assetStore, eventBus);
      }

      ImGui::Render();
      ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    }

    void renderNewCanvasWindow(bool& open, SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, std::shared_ptr<EventBus>& eventBus) {
      if (ImGui::Begin("Create Canvas", NULL, ImGuiWindowFlags_HorizontalScrollbar)) {
        // Static variables to hold input values
        static char filePath[64] = "";
        static int tileSize = 0;
        static int tileNumX = 0;
        static int tileNumY = 0;

        if (ImGui::CollapsingHeader("New Canvas", ImGuiTreeNodeFlags_DefaultOpen)) {
          ImGui::InputText("New File Name", filePath, 64);
          ImGui::InputInt("Tile size (square)", &tileSize);
          ImGui::InputInt("The number of columns", &tileNumX);
          ImGui::InputInt("The number of rows", &tileNumY);
          if (ImGui::Button("Create")) {
            std::string filePathStr(filePath);
            eventBus->emitEvent<CanvasCreatedEvent>(filePathStr, tileSize, tileNumX, tileNumY);
            tileSize = 0;
            tileNumX = 0;
            tileNumY = 0;
            open = false;
          }
        }
        ImGui::End();
      }
    }

    void renderOpenCanvasWindow(bool& open, SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, std::shared_ptr<EventBus>& eventBus) {
      // Static variables to hold input values
      static char mapFilePath[64] = "";

      if (ImGui::CollapsingHeader("Open Canvas", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::InputText("File Path", mapFilePath, 64);
        if (ImGui::Button("Open")) {
          std::string mapFilePathStr(mapFilePath);
          mapFilePath[0] = '\0';
          // Load the file
          std::string assetId;
          int tileSize, tileNumX, tileNumY;
          float scale;
          std::vector<Tile> assignedTiles;
          MapFileLoader::load(mapFilePathStr, assetId, tileSize, tileNumX, tileNumY, scale, assignedTiles);
          eventBus->emitEvent<CanvasOpenedEvent>(mapFilePathStr, assetId, tileSize, tileNumX, tileNumY, scale, assignedTiles);
          open = false;
        }
      }
    }

    void renderCanvasPropertiesWindow(bool& open, SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, std::shared_ptr<EventBus>& eventBus) {
      if (ImGui::Begin("Canvas", NULL, ImGuiWindowFlags_HorizontalScrollbar)) {
        // Static variables to hold input values
        static int tileSize = 0;
        static int tileNumX = 0;
        static int tileNumY = 0;

        if (ImGui::CollapsingHeader("Edit Properties", ImGuiTreeNodeFlags_DefaultOpen)) {
          ImGui::InputInt("Tile size (square)", &tileSize);
          ImGui::InputInt("The number of columns", &tileNumX);
          ImGui::InputInt("The number of rows", &tileNumY);
          if (ImGui::Button("Apply")) {
            eventBus->emitEvent<CanvasPropertiesChangedEvent>(tileSize, tileNumX, tileNumY);
            tileSize = 0;
            tileNumX = 0;
            tileNumY = 0;
            open = false;
          }
        }
        ImGui::End();
      }
    }

    void renderEntityWindow(bool& open, SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, std::shared_ptr<EventBus>& eventBus) {
      if (ImGui::Begin("Entity", NULL, ImGuiWindowFlags_HorizontalScrollbar)) {
        // TODO

        ImGui::End();
      }
    }

    void renderTileSetWindow(bool& open, SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, std::shared_ptr<EventBus>& eventBus) {
      auto entity = getSystemEntities()[0]; // singleton?
      auto& tileSet = entity.getComponent<TileSetComponent>();
      auto& loadedTileSets = entity.getComponent<LoadedTileSetsComponent>().tileSets;

      if (ImGui::Begin("Tileset", NULL, ImGuiWindowFlags_HorizontalScrollbar)) {
        float width = tileSet.width * tileSet.scale;
        float height = tileSet.height * tileSet.scale;
        ImGui::Image(assetStore->getTexture(tileSet.assetId), ImVec2(width, height));

        // Handle mouse (left) click event
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
          int mouseX = ImGui::GetMousePos().x - ImGui::GetWindowPos().x + ImGui::GetScrollX() - 10; // a little offset
          int mouseY = ImGui::GetMousePos().y - ImGui::GetWindowPos().y + ImGui::GetScrollY() - 20 - 10; // (TITLE_BAR_SIZE + a little offset)
          int row = mouseX / (static_cast<float>(tileSet.tileSize) * tileSet.scale); // x-dir index of selected tile
          int col = mouseY / (static_cast<float>(tileSet.tileSize) * tileSet.scale); // y-dir index of selected tile
          // Emit event to change selected tile
          eventBus->emitEvent<SelectedTileChangedEvent>(row, col);
        }

        // Draw boarder & grid in the canvas
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        static ImVec2 scrolling(0.0f, 0.0f);
        ImVec2 leftTop = ImGui::GetCursorScreenPos();
        leftTop = ImVec2(leftTop.x, leftTop.y - height - 5);
        ImVec2 rightBottom = ImVec2(leftTop.x + width, leftTop.y + height);
        draw_list->AddRect(leftTop, rightBottom, IM_COL32(255, 255, 255, 100));

        draw_list->PushClipRect(leftTop, rightBottom, true);
        const float GRID_STEP = static_cast<float>(tileSet.tileSize) * tileSet.scale;
        for (float x = fmodf(scrolling.x, GRID_STEP); x < width; x += GRID_STEP)
          draw_list->AddLine(ImVec2(leftTop.x + x, leftTop.y), ImVec2(leftTop.x + x, rightBottom.y), IM_COL32(200, 200, 200, 60));
          for (float y = fmodf(scrolling.y, GRID_STEP); y < height; y += GRID_STEP)
            draw_list->AddLine(ImVec2(leftTop.x, leftTop.y + y), ImVec2(rightBottom.x, leftTop.y + y), IM_COL32(200, 200, 200, 60));
        draw_list->PopClipRect();

        // Help Messages
        ImGui::Text("Click the tile to select");
        ImGui::Text("Save: Hit [w] | Pan: Mouse right click + Drag");

        // Static variables to hold input values
        static char tileSetId[64] = "";
        static char textureFilePath[64] = "";
        static int tileNumX = 0;
        static int tileNumY = 0;
        static int tileSize = 0;
        static int scale = 0;
        static int selectedSpriteIndex = 0;
        std::vector<std::string> spritesV = {};
        for (auto it = loadedTileSets.begin(); it != loadedTileSets.end(); ++it) {
          spritesV.push_back(it->first);
        }
        const char* sprites[spritesV.size()];
        for (int i = 0; i < spritesV.size(); i++) {
          sprites[i] = const_cast<char*>(spritesV[i].c_str());
        }

        // Section to select tileset
        if (ImGui::CollapsingHeader("Load Tileset", ImGuiTreeNodeFlags_DefaultOpen)) {
          ImGui::Combo("Sprite", &selectedSpriteIndex, sprites, IM_ARRAYSIZE(sprites));
          ImGui::Text("*** WARNING: Changing the tile set will initialize the canvas!");
          if (ImGui::Button("Load")) {
            std::string tileSetIdStr(sprites[selectedSpriteIndex]);
            tileSet.assetId = tileSetIdStr;
            tileSet.tileSize = loadedTileSets[tileSetIdStr]->tileSize;
            tileSet.tileNumX = loadedTileSets[tileSetIdStr]->tileNumX;
            tileSet.tileNumY = loadedTileSets[tileSetIdStr]->tileNumY;
            tileSet.scale = loadedTileSets[tileSetIdStr]->scale;
            tileSet.width = tileSet.tileSize * tileSet.tileNumX;
            tileSet.height = tileSet.tileSize * tileSet.tileNumY;
            TileSet selectedTileSet(tileSet.tileSize, tileSet.tileNumX, tileSet.tileNumY, tileSet.scale);
            eventBus->emitEvent<TileSetChangedEvent>(tileSetIdStr, selectedTileSet);
          }
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Section to add new tileset
        if (ImGui::CollapsingHeader("Add Tileset", ImGuiTreeNodeFlags_DefaultOpen)) {
          ImGui::InputText("Texture ID", tileSetId, 64);
          ImGui::InputText("Texture File Path", textureFilePath, 64);
          ImGui::InputInt("The number of columns", &tileNumX);
          ImGui::InputInt("The number of rows", &tileNumY);
          ImGui::InputInt("Tile size (sqaure)", &tileSize);
          ImGui::InputInt("Scale", &scale);
          if (ImGui::Button("Add")) {
            // add new texture to sprites list and assetStore
            std::string tileSetIdStr(tileSetId);
            std::string textureFilePathStr(textureFilePath);
            TileSet* newTileSet = new TileSet(tileSize, tileNumX, tileNumY, scale); // dynamic memory allocation
            loadedTileSets.emplace(tileSetIdStr, newTileSet);
            assetStore->addTexture(renderer, tileSetIdStr, textureFilePathStr);
            tileSetId[0] = '\0';
            textureFilePath[0] = '\0';
            tileSize = 0;
            tileNumX = 0;
            tileNumY = 0;
            scale = 0;
          }
        }

        ImGui::End();
      }
    }
};

#endif
