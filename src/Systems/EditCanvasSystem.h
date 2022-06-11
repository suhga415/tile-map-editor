#ifndef EDITCANVASSYSTEM_H
#define EDITCANVASSYSTEM_H

#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "../Components/CanvasComponent.h"
#include "../Components/SelectedTileComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/MouseClickEvent.h"
#include "../Events/KeyPressedEvent.h"
#include "../Events/TileSetChangedEvent.h"
#include "../Events/CanvasCreatedEvent.h"
#include "../Events/CanvasPropertiesChangedEvent.h"
#include "../Utilities/MapFileWriter.h"
#include "../Constants.h"
#include <memory>
#include <SDL2/SDL.h>
#include <imgui/imgui.h>

class EditCanvasSystem: public System {
  private:
    std::shared_ptr<EventBus> eventBus;

  public:
    EditCanvasSystem() {
      requireComponent<CanvasComponent>();
      requireComponent<SelectedTileComponent>();
    }

    void subscribeToEvents(std::shared_ptr<EventBus>& eventBus) {
      this->eventBus = eventBus;
      eventBus->subscribeToEvent<MouseClickEvent>(this, &EditCanvasSystem::onMouseClicked);
      eventBus->subscribeToEvent<KeyPressedEvent>(this, &EditCanvasSystem::onSaveKeyPressed);
      eventBus->subscribeToEvent<TileSetChangedEvent>(this, &EditCanvasSystem::onTileSetChanged);
      eventBus->subscribeToEvent<CanvasCreatedEvent>(this, &EditCanvasSystem::onCanvasCreated);
      eventBus->subscribeToEvent<CanvasPropertiesChangedEvent>(this, &EditCanvasSystem::onCanvasPropertiesChanged);
    }

    void onMouseClicked(MouseClickEvent& event) {
      ImGuiIO& io = ImGui::GetIO();
      if (!io.WantCaptureMouse) {
        int mouseX = static_cast<int>(event.location.x);
        int mouseY = static_cast<int>(event.location.y);
        auto entity = getSystemEntities()[0]; // singleton?
        auto& canvas = entity.getComponent<CanvasComponent>();
        auto selectedTile = entity.getComponent<SelectedTileComponent>();
        bool withinCanvasX = true;
        bool withinCanvasY = true;
        if (event.camera.w > canvas.tileSize * canvas.tileNumX * canvas.scale) { // if canvas (width) is smaller than camera view
          withinCanvasX = (mouseX >= CANVAS_X - event.camera.x) && (mouseX <= CANVAS_X + canvas.tileSize * canvas.tileNumX * canvas.scale - event.camera.x);
        }
        if (event.camera.h > canvas.tileSize * canvas.tileNumY * canvas.scale) { // if canvas (height) is smaller than camera view
          withinCanvasY = (mouseY >= CANVAS_Y - event.camera.y) && (mouseY <= CANVAS_Y + canvas.tileSize * canvas.tileNumY * canvas.scale - event.camera.y);
        }
        if (withinCanvasX && withinCanvasY) {
          int row = (mouseX - CANVAS_X + event.camera.x) / (canvas.tileSize * canvas.scale);
          int col = (mouseY - CANVAS_Y + event.camera.y) / (canvas.tileSize * canvas.scale);
          Logger::Info("[EditCanvasSystem] row = " + std::to_string(row) + ", col = " + std::to_string(col));
          canvas.assignedTiles[col * canvas.tileNumX + row].rowIdx = selectedTile.rowIdx;
          canvas.assignedTiles[col * canvas.tileNumX + row].colIdx = selectedTile.colIdx;
        }
      }
    }

    void onSaveKeyPressed(KeyPressedEvent& event) {
      std::string keyCode = std::to_string(event.symbol);
      std::string keySymbol(1, event.symbol);
      if (keySymbol.compare("w") == 0) {
        for (auto entity: getSystemEntities()) {
          const auto canvas = entity.getComponent<CanvasComponent>();
          const auto selectedTile = entity.getComponent<SelectedTileComponent>();
          MapFileWriter::write(canvas, selectedTile, "./out.map");
        }
      }
    }

    void onTileSetChanged(TileSetChangedEvent& event) {
      for (auto entity: getSystemEntities()) {
        auto& canvas = entity.getComponent<CanvasComponent>();
        // Initialize the canvas...?
      }
    }

    void onCanvasCreated(CanvasCreatedEvent& event) {
      for (auto entity: getSystemEntities()) {
        auto& canvas = entity.getComponent<CanvasComponent>();
        canvas.tileSize = event.tileSize;
        canvas.tileNumX = event.tileNumX;
        canvas.tileNumY = event.tileNumY;
        canvas.initialize();
      }
    }

    void onCanvasPropertiesChanged(CanvasPropertiesChangedEvent& event) {
      for (auto entity: getSystemEntities()) {
        auto& canvas = entity.getComponent<CanvasComponent>();
        int prevNumX = canvas.tileNumX;
        int prevNumY = canvas.tileNumY;
        canvas.tileSize = event.tileSize;
        canvas.tileNumX = event.tileNumX;
        canvas.tileNumY = event.tileNumY;
        canvas.reassignTilesByNumChange(prevNumX, prevNumY);
      }
    }

    // Render the canvas
    void update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, SDL_Rect& camera) {
      for (auto entity: getSystemEntities()) {
        const auto canvas = entity.getComponent<CanvasComponent>();
        const auto selectedTile = entity.getComponent<SelectedTileComponent>();
        for (int j = 0; j < canvas.tileNumY; j++) {
          for (int i = 0; i < canvas.tileNumX; i++) {
            int sourceRowIdx = canvas.assignedTiles[j * canvas.tileNumX + i].rowIdx;
            int sourceColIdx = canvas.assignedTiles[j * canvas.tileNumX + i].colIdx;
            if (sourceRowIdx == -1 && sourceColIdx == -1) {
              SDL_Rect emptyTile = {
                static_cast<int>(canvas.locationX + i * canvas.tileSize * canvas.scale - camera.x),
                static_cast<int>(canvas.locationY + j * canvas.tileSize * canvas.scale - camera.y),
                static_cast<int>(canvas.tileSize * canvas.scale),
                static_cast<int>(canvas.tileSize * canvas.scale)
              };
              if ((i + j)%2 == 0) {
                SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
              } else {
                SDL_SetRenderDrawColor(renderer, 155, 155, 155, 255);
              }
              SDL_RenderFillRect(renderer, &emptyTile);
            } else {
              // render the assigned tile
              SDL_Rect sourceRect = {
                static_cast<int>(sourceRowIdx * selectedTile.tileSize * selectedTile.scale),
                static_cast<int>(sourceColIdx * selectedTile.tileSize * selectedTile.scale),
                static_cast<int>(selectedTile.tileSize * selectedTile.scale),
                static_cast<int>(selectedTile.tileSize * selectedTile.scale)
              };
              SDL_Rect destinationRect = {
                static_cast<int>(canvas.locationX + i * canvas.tileSize * canvas.scale - camera.x),
                static_cast<int>(canvas.locationY + j * canvas.tileSize * canvas.scale - camera.y),
                static_cast<int>(canvas.tileSize * canvas.scale),
                static_cast<int>(canvas.tileSize * canvas.scale)
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
        }
      }
    }

};

#endif
