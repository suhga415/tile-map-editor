#ifndef RENDERCURSORSYSTEM_H
#define RENDERCURSORSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/CursorPosComponent.h"
#include "../Components/SelectedTileComponent.h"
#include "../AssetStore/AssetStore.h"
#include <SDL2/SDL.h>
#include <imgui/imgui.h>

class RenderCursorSystem: public System {
  private:
  public:
    RenderCursorSystem() {
      requireComponent<CursorPosComponent>();
      requireComponent<SelectedTileComponent>();
    }

    void update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore) {
      auto entity = getSystemEntities()[0]; // singleton
      const auto cursorPos = entity.getComponent<CursorPosComponent>();
      const auto selectedTile = entity.getComponent<SelectedTileComponent>();
      const int tileSize = selectedTile.tileSize * selectedTile.scale;

      ImGuiIO& io = ImGui::GetIO();
      if (!io.WantCaptureMouse) {
        SDL_Rect sourceRect = {
          static_cast<int>(selectedTile.rowIdx * tileSize),
          static_cast<int>(selectedTile.colIdx * tileSize),
          static_cast<int>(tileSize),
          static_cast<int>(tileSize)
        };
        SDL_Rect destinationRect = {
          static_cast<int>(cursorPos.position.x - tileSize / 2),
          static_cast<int>(cursorPos.position.y - tileSize / 2),
          static_cast<int>(tileSize),
          static_cast<int>(tileSize)
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
      }
    }
};

#endif
