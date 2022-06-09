#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/CursorPosComponent.h"
#include "../Components/SelectedTileComponent.h"
#include "../AssetStore/AssetStore.h"
#include <SDL2/SDL.h>
#include <imgui/imgui.h>

class RenderSystem: public System {
  private:
  public:
    RenderSystem() {
      requireComponent<TransformComponent>();
      requireComponent<SpriteComponent>();
    }

    void update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore) {
      // Sort entities by z-index <-- this might be a red flag in terms of performance later...
      auto entities = getSystemEntities();
      std::sort(entities.begin(), entities.end(), [](const Entity& a, const Entity& b) {
        return a.getComponent<SpriteComponent>().zIndex < b.getComponent<SpriteComponent>().zIndex;
      });

      // Render the sorted entities
      for (auto entity: entities) {
        const auto transform = entity.getComponent<TransformComponent>();
        const auto sprite = entity.getComponent<SpriteComponent>();
        
        SDL_Rect sourceRect = sprite.srcRect;
        SDL_Rect destinationRect = {
          static_cast<int>(transform.position.x),
          static_cast<int>(transform.position.y),
          static_cast<int>(sprite.width * transform.scale.x),
          static_cast<int>(sprite.height * transform.scale.y)
        };

        if (entity.hasComponent<CursorPosComponent>() && entity.hasComponent<SelectedTileComponent>()) {
          // it's mouse cursor.
          ImGuiIO& io = ImGui::GetIO();
          if (!io.WantCaptureMouse) {
            renderMouseCursor(entity, sourceRect, destinationRect);
          } else {
            continue;
          }
        }

      	SDL_RenderCopyEx(
          renderer,
          assetStore->getTexture(sprite.assetId),
          &sourceRect,
          &destinationRect,
          transform.rotation,
          NULL,
          SDL_FLIP_NONE
        );
      }
    }

  private:
    void renderMouseCursor(Entity& entity, SDL_Rect& sourceRect, SDL_Rect& destinationRect) {
      const auto cursorPos = entity.getComponent<CursorPosComponent>();
      const auto selectedTile = entity.getComponent<SelectedTileComponent>();
      const int tileSize = selectedTile.tileSize * selectedTile.scale;

      sourceRect = {
        static_cast<int>(selectedTile.rowIdx * tileSize),
        static_cast<int>(selectedTile.colIdx * tileSize),
        static_cast<int>(tileSize),
        static_cast<int>(tileSize)
      };
      destinationRect = {
        static_cast<int>(cursorPos.position.x - tileSize / 2),
        static_cast<int>(cursorPos.position.y - tileSize / 2),
        static_cast<int>(tileSize),
        static_cast<int>(tileSize)
      };
    }
};

#endif
