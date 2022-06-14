#ifndef EDITCANVASSYSTEM_H
#define EDITCANVASSYSTEM_H

#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "../Components/CanvasComponent.h"
#include "../Components/SelectedTileComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/MouseClickEvent.h"
#include "../Events/MouseDragEvent.h"
#include "../Events/KeyPressedEvent.h"
#include "../Events/TileSetChangedEvent.h"
#include "../Events/CanvasCreatedEvent.h"
#include "../Events/CanvasOpenedEvent.h"
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
      eventBus->subscribeToEvent<MouseDragEvent>(this, &EditCanvasSystem::onMouseDrag);
      eventBus->subscribeToEvent<KeyPressedEvent>(this, &EditCanvasSystem::onSaveKeyPressed);
      eventBus->subscribeToEvent<TileSetChangedEvent>(this, &EditCanvasSystem::onTileSetChanged);
      eventBus->subscribeToEvent<CanvasCreatedEvent>(this, &EditCanvasSystem::onCanvasCreated);
      eventBus->subscribeToEvent<CanvasOpenedEvent>(this, &EditCanvasSystem::onCanvasOpened);
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
        if (event.camera->w > canvas.tileSize * canvas.tileNumX) { // if canvas (width) is smaller than camera view
          withinCanvasX = (mouseX >= canvas.locationX - event.camera->x) && (mouseX <= canvas.locationX + canvas.tileSize * canvas.tileNumX - event.camera->x);
        }
        if (event.camera->h > canvas.tileSize * canvas.tileNumY) { // if canvas (height) is smaller than camera view
          withinCanvasY = (mouseY >= CANVAS_Y - event.camera->y) && (mouseY <= CANVAS_Y + canvas.tileSize * canvas.tileNumY - event.camera->y);
        }
        if (withinCanvasX && withinCanvasY) {
          int row = (mouseX - canvas.locationX + event.camera->x) / (canvas.tileSize);
          int col = (mouseY - CANVAS_Y + event.camera->y) / (canvas.tileSize);
          Logger::Info("[EditCanvasSystem] row = " + std::to_string(row) + ", col = " + std::to_string(col));
          canvas.assignedTiles[col * canvas.tileNumX + row].rowIdx = selectedTile.rowIdx;
          canvas.assignedTiles[col * canvas.tileNumX + row].colIdx = selectedTile.colIdx;
        }
      }
    }

    void onMouseDrag(MouseDragEvent& event) {
      auto entity = getSystemEntities()[0]; // singleton?
      auto canvas = entity.getComponent<CanvasComponent>();
      auto& camera = event.camera;
      SDL_Rect canvasRect = {canvas.locationX, canvas.locationY, canvas.tileSize * canvas.tileNumX, canvas.tileSize * canvas.tileNumY};
      cameraPan(event.dragDist.x, event.dragDist.y, camera, canvasRect);
    }

    void cameraPan(int dragDistX, int dragDistY, std::shared_ptr<SDL_Rect>& camera, SDL_Rect& canvas) {
      camera->x += dragDistX;
      camera->y += dragDistY;
      // update camera x-position
      if (camera->w >= canvas.w) { // the canvas is smaller than camera
        if (camera->x >= 0) {
          camera->x = 0;
        } else if (camera->x <= canvas.w - camera->w) {
          camera->x = canvas.w - camera->w;
        }
      } else { // the canvas is bigger than camera
        if (camera->x <= 0) {
          camera->x = 0;
        } else if (camera->x + camera->w >= canvas.x + canvas.w) {
          camera->x = canvas.x + canvas.w - camera->w;
        }
      }
      // update camera y-position
      if (camera->h >= canvas.h) {
        if (camera->y >= 0) {
          camera->y = 0;
        } else if (camera->y <= canvas.h - camera->h) {
          camera->y = canvas.h - camera->h;
        }
      } else {
        if (camera->y <= 0) {
          camera->y = 0;
        } else if (camera->y + camera->h >= canvas.y + canvas.h) {
          camera->y = canvas.y + canvas.h - camera->h;
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
          MapFileWriter::write(canvas, selectedTile, canvas.filePath);
        }
      }
    }

    void onTileSetChanged(TileSetChangedEvent& event) {
      for (auto entity: getSystemEntities()) {
        auto& canvas = entity.getComponent<CanvasComponent>();
        canvas.scale = static_cast<float>(canvas.tileSize) / static_cast<float>(event.selectedTileSet.tileSize);
        canvas.initialize();
      }
    }

    void onCanvasCreated(CanvasCreatedEvent& event) {
      for (auto entity: getSystemEntities()) {
        auto& canvas = entity.getComponent<CanvasComponent>();
        auto& selectedTile = entity.getComponent<SelectedTileComponent>();
        canvas.filePath = event.filePath;
        canvas.tileSize = event.tileSize;
        canvas.tileNumX = event.tileNumX;
        canvas.tileNumY = event.tileNumY;
        if (selectedTile.tileSize != 0) { // if any tile set is currently loaded
          canvas.scale = static_cast<float>(canvas.tileSize) / static_cast<float>(selectedTile.tileSize);
        }
        canvas.initialize();
      }
    }

    void onCanvasOpened(CanvasOpenedEvent& event) {
      for (auto entity: getSystemEntities()) {
        auto& canvas = entity.getComponent<CanvasComponent>();
        auto& selectedTile = entity.getComponent<SelectedTileComponent>();
        selectedTile.assetId = event.assetId;
        canvas.filePath = event.filePath;
        canvas.tileSize = event.tileSize;
        canvas.tileNumX = event.tileNumX;
        canvas.tileNumY = event.tileNumY;
        canvas.scale = event.scale;
        canvas.assignedTiles = event.assignedTiles;
        // selectedTile의 tile size???
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
    void update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, std::shared_ptr<SDL_Rect>& camera) {
      for (auto entity: getSystemEntities()) {
        const auto canvas = entity.getComponent<CanvasComponent>();
        const auto selectedTile = entity.getComponent<SelectedTileComponent>();
        for (int j = 0; j < canvas.tileNumY; j++) {
          for (int i = 0; i < canvas.tileNumX; i++) {
            int sourceRowIdx = canvas.assignedTiles[j * canvas.tileNumX + i].rowIdx;
            int sourceColIdx = canvas.assignedTiles[j * canvas.tileNumX + i].colIdx;
            if (sourceRowIdx == -1 && sourceColIdx == -1) {
              SDL_Rect emptyTile = {
                static_cast<int>(canvas.locationX + i * canvas.tileSize - camera->x),
                static_cast<int>(canvas.locationY + j * canvas.tileSize - camera->y),
                static_cast<int>(canvas.tileSize),
                static_cast<int>(canvas.tileSize)
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
                static_cast<int>(sourceRowIdx * selectedTile.tileSize),
                static_cast<int>(sourceColIdx * selectedTile.tileSize),
                static_cast<int>(selectedTile.tileSize),
                static_cast<int>(selectedTile.tileSize)
              };
              SDL_Rect destinationRect = {
                static_cast<int>(canvas.locationX + i * canvas.tileSize - camera->x),
                static_cast<int>(canvas.locationY + j * canvas.tileSize - camera->y),
                static_cast<int>(canvas.tileSize),
                static_cast<int>(canvas.tileSize)
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
