#ifndef TILESETGUISYSTEM_H
#define TILESETGUISYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TileSetComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/SelectedTileChangedEvent.h"
#include "../Events/TileSetChangedEvent.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_sdlrenderer.h>
#include <SDL2/SDL.h>
#include <string>
#include <vector>

struct TileSet {
  int tileSize;
  int tileNumX;
  int tileNumY;
  float scale;
};

class TileSetGUISystem: public System {
  private:
    std::map<std::string, TileSet*> loadedTileSets;

  public:
    TileSetGUISystem() {
      requireComponent<TileSetComponent>();
      TileSet jungle = {32, 10, 3, 1.5};
      loadedTileSets.emplace("jungle-image", &jungle);
    }

    void update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, std::shared_ptr<EventBus>& eventBus) {
      ImGui_ImplSDLRenderer_NewFrame();
      ImGui_ImplSDL2_NewFrame();
      ImGui::NewFrame();
      ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
      
      // ImGui::ShowDemoWindow();
      
      auto entity = getSystemEntities()[0]; // singleton?
      auto& tileSet = entity.getComponent<TileSetComponent>();

      if (ImGui::Begin("Tileset", NULL, window_flags)) {
        float width = tileSet.width * tileSet.scale;
        float height = tileSet.height * tileSet.scale;
        ImGui::Image(assetStore->getTexture(tileSet.assetId), ImVec2(width, height));

        // Handle mouse (left) click event
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
          int mouseX = ImGui::GetMousePos().x - ImGui::GetWindowPos().x + ImGui::GetScrollX() - 10; // a little offset
          int mouseY = ImGui::GetMousePos().y - ImGui::GetWindowPos().y + ImGui::GetScrollY() - 20 - 10; // (TITLE_BAR_SIZE + a little offset)
          int row = mouseX / (static_cast<float>(tileSet.tileSize) * tileSet.scale);
          int col = mouseY / (static_cast<float>(tileSet.tileSize) * tileSet.scale);
          Logger::Info("click** row = " + std::to_string(row) + ", col = " + std::to_string(col));
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
          if (ImGui::Button("Load")) {
            // tileSet.assetId = sprites[selectedSpriteIndex];
            // tileSet.tileSize = loadedTileSets[tileSet.assetId].tileSize;
            // tileSet.tileNumX = loadedTileSets[tileSet.assetId].tileNumX;
            // tileSet.tileNumY = loadedTileSets[tileSet.assetId].tileNumY;
            // tileSet.scale = loadedTileSets[tileSet.assetId].scale;
            // eventBus->emitEvent<TileSetChangedEvent>(tileSet.assetId);
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
            spritesV.push_back(tileSetIdStr);
            Logger::Info("tile id: " + tileSetIdStr + ", file path: " + textureFilePath);
            tileSetId[0] = '\0';
            textureFilePath[0] = '\0';
            assetStore->addTexture(renderer, tileSetIdStr, textureFilePathStr);
            tileSet.assetId = tileSetIdStr;
            tileSet.tileSize = tileSize;
            tileSet.tileNumX = tileNumX;
            tileSet.tileNumY = tileNumY;
            tileSet.scale = scale;
            tileSet.width = tileSize * tileNumX;
            tileSet.height = tileSize * tileNumY;
            eventBus->emitEvent<TileSetChangedEvent>(tileSetIdStr);
            // TODO: canvas에 있는 정보들도 바뀌어야 - scale 이랑 tileSize. assignedTiles는 초기화되어야할지도..
            // TODO: add this new tile set to the loadedTileSets
          }
        }
        
        ImGui::End();
      }
      ImGui::Render();
      ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    }

    void update0(std::unique_ptr<AssetStore>& assetStore) {
      bool show_another_window = false;

      ImGui_ImplSDLRenderer_NewFrame();
      ImGui_ImplSDL2_NewFrame();
      ImGui::NewFrame();


      // ImGui::ShowDemoWindow();
      
      ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;

      if (ImGui::Begin("Texture", NULL, window_flags)) {
        // The image might be larger than the ImGui window, so we want to add the current scroll value to the 
        // mouse position for the selected image 
        auto scrollY = ImGui::GetScrollY();
        auto scrollX = ImGui::GetScrollX();

        // ImGui::BeginChild("child", ImVec2(300, 200), true, ImGuiWindowFlags_HorizontalScrollbar);

        // Normalized coordinates of pixel (10,10) in a 320x96 texture.
        // ImVec2 uv0 = ImVec2(10.0f/320.0f, 10.0f/96.0f);
        // Normalized coordinates of pixel (110,60) in a 320x96 texture.
        // ImVec2 uv1 = ImVec2((10.0f+100.0f)/320.0f, (10.0f+50.0f)/96.0f);        
        // ImGui::Image(assetStore->getTexture("tilemap-image"), ImVec2(100.0f, 50.0f), uv0, uv1);

        ImGui::Image(assetStore->getTexture("tilemap-image"), ImVec2(320, 96));

        // int mousePosX = static_cast<int>(ImGui::GetMousePos().x - ImGui::GetWindowPos().x + scrollX);
        // int mousePosY = static_cast<int>(ImGui::GetMousePos().y - ImGui::GetWindowPos().y - TITLE_BAR_SIZE + scrollY);
        // int rows = imageHeight / (mouseRect.y * 2);
        // int cols = imageWidth / (mouseRect.x * 2);

        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
          int mouseX = ImGui::GetMousePos().x - ImGui::GetWindowPos().x + scrollX;
          int mouseY = ImGui::GetMousePos().y - ImGui::GetWindowPos().y + scrollY - 26; // TITLE_BAR_SIZE
          int row = mouseX / (32 * 1);
          int col = mouseY / (32 * 1);
          Logger::Info("click** row = " + std::to_string(row) + ", col = " + std::to_string(col));

          // change selected tile...
          // eventBus->emitEvent<SelectedTileChangedEvent>(row, col);
        }

        // for (int i = 0; i < cols; i++) {
        //   for (int j = 0; j < rows; j++) {
        //     auto drawList = ImGui::GetWindowDrawList();
        //     // Check to see if we are in the area of the desired 2D tile
        //     if ((mousePosX >= (imageWidth / cols) * i && mousePosX <= (imageWidth / cols) + ((imageWidth / cols) * i))
        //       && (mousePosY >= (imageHeight / rows) * j && mousePosY <= (imageHeight / rows) + ((imageHeight / rows) * j)))
        //     {
        //       if (ImGui::IsItemHovered()) {
        //         if (ImGui::IsMouseClicked(0)) {
        //           mSrcRectX = i * mouseRect.x;
        //           mSrcRectY = j * mouseRect.y;
        //         }
        //       }
        //     }
        //   }
        // }

        // ImGui::EndChild();

        ImGui::End();
      }

      // Display a small overlay window to display the map position using the mouse
      if (show_another_window) {
        if (ImGui::Begin("Test")) {
          ImGui::Text("Hello! This is a new window.");
        }
        ImGui::End();

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav;
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always, ImVec2(0, 0));
        ImGui::SetNextWindowBgAlpha(0.9f);
        if (ImGui::Begin("Map coordinates", NULL, windowFlags)) {
          ImGui::Text(
            "Map coordinates (x=%.1f, y=%.1f)",
            ImGui::GetIO().MousePos.x,
            ImGui::GetIO().MousePos.y
          );
        }
        ImGui::End();
      }

      ImGui::Render();
      ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    }

};

#endif
