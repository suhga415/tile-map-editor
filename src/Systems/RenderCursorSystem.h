#ifndef RENDERCURSORSYSTEM_H
#define RENDERCURSORSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/CursorTileComponent.h"
#include "../Components/SelectedTileComponent.h"
#include "../AssetStore/AssetStore.h"
#include <SDL2/SDL.h>
#include <imgui/imgui.h>

class RenderCursorSystem: public System {
  private:
  public:
    RenderCursorSystem() {
      requireComponent<CursorTileComponent>();
      requireComponent<SelectedTileComponent>();
    }

    void update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore) {
      auto entity = getSystemEntities()[0]; // singleton
      const auto cursorPos = entity.getComponent<CursorTileComponent>();
      const auto selectedTile = entity.getComponent<SelectedTileComponent>();

      ImGuiIO& io = ImGui::GetIO();
      if (!io.WantCaptureMouse) {
        SDL_Rect sourceRect = {
          static_cast<int>(selectedTile.rowIdx * selectedTile.tileSize),
          static_cast<int>(selectedTile.colIdx * selectedTile.tileSize),
          static_cast<int>(selectedTile.tileSize),
          static_cast<int>(selectedTile.tileSize)
        };
        SDL_Rect destinationRect = {
          static_cast<int>(cursorPos.position.x),
          static_cast<int>(cursorPos.position.y),
          static_cast<int>(cursorPos.tileSize),
          static_cast<int>(cursorPos.tileSize)
        };
        SDL_RenderCopyEx(
          renderer,
          assetStore->getTexture(selectedTile.assetId),
          &sourceRect,
          &destinationRect,
          0.0, // rotation
          NULL,
          SDL_FLIP_NONE
        );
        if (!selectedTile.assetId.empty()) {
          SDL_SetRenderDrawColor(renderer, 200, 50, 50, 150); // Red
          SDL_RenderDrawRect(renderer, &destinationRect);
        }
      }
    }
};

#endif
