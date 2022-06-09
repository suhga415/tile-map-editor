#ifndef TILESETGUISYSTEM_H
#define TILESETGUISYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TileSetComponent.h"
#include "../Components/SpriteComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/SelectedTileChangedEvent.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_sdlrenderer.h>
#include <SDL2/SDL.h>

class TileSetGUISystem: public System {
  public:
    TileSetGUISystem() {
      requireComponent<TileSetComponent>();
      requireComponent<SpriteComponent>();
    }

    void update(std::unique_ptr<AssetStore>& assetStore, std::shared_ptr<EventBus>& eventBus) {
      ImGui_ImplSDLRenderer_NewFrame();
      ImGui_ImplSDL2_NewFrame();
      ImGui::NewFrame();
      ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
      
      // ImGui::ShowDemoWindow();
      
      auto entity = getSystemEntities()[0]; // singleton?
      auto tileSet = entity.getComponent<TileSetComponent>();
      auto sprite = entity.getComponent<SpriteComponent>();

      if (ImGui::Begin("Tileset", NULL, window_flags)) {
        float width = sprite.width * tileSet.scale;
        float height = sprite.height * tileSet.scale;
        ImGui::Image(assetStore->getTexture(sprite.assetId), ImVec2(width, height));
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
          int mouseX = ImGui::GetMousePos().x - ImGui::GetWindowPos().x + ImGui::GetScrollX() - 10; // a little offset
          int mouseY = ImGui::GetMousePos().y - ImGui::GetWindowPos().y + ImGui::GetScrollY() - 20 - 10; // (TITLE_BAR_SIZE + a little offset)
          int row = mouseX / (static_cast<float>(tileSet.tileSize) * tileSet.scale);
          int col = mouseY / (static_cast<float>(tileSet.tileSize) * tileSet.scale);
          Logger::Info("click** row = " + std::to_string(row) + ", col = " + std::to_string(col));
          // change selected tile...
          eventBus->emitEvent<SelectedTileChangedEvent>(row, col);
        }

        // Draw grid in the canvas
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
