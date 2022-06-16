#ifndef CURSORMOVEMENTSYSTEM_H
#define CURSORMOVEMENTSYSTEM_H

#include "../Constants.h"
#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "../Components/CursorTileComponent.h"
#include "../Components/SelectedTileComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CanvasCreatedEvent.h"
#include "../Events/CanvasOpenedEvent.h"
#include <math.h>

class CursorPropertiesSystem: public System {
  public:
    CursorPropertiesSystem() {
      requireComponent<CursorTileComponent>();
      requireComponent<SelectedTileComponent>();
    }

    void subscribeToEvents(std::shared_ptr<EventBus>& eventBus) {
      eventBus->subscribeToEvent<CanvasCreatedEvent>(this, &CursorPropertiesSystem::onCanvasCreated);
      eventBus->subscribeToEvent<CanvasOpenedEvent>(this, &CursorPropertiesSystem::onCanvasOpened);
    }

    void update(int posX, int posY, std::shared_ptr<SDL_Rect>& camera) {
      for (auto entity: getSystemEntities()) { // should be singleton actually
        auto& cursorPos = entity.getComponent<CursorTileComponent>();
        if (cursorPos.snap) {
          snap(posX, posY, camera);
        } else {
          cursorPos.position.x = posX;
          cursorPos.position.y = posY;
        }
      }
    }

    void snap(int posX, int posY, std::shared_ptr<SDL_Rect>& camera) {
      for (auto entity: getSystemEntities()) { // should be singleton actually
        auto& cursorPos = entity.getComponent<CursorTileComponent>();
        int tileSize = entity.getComponent<CursorTileComponent>().tileSize;
        cursorPos.position.x = floor(float(posX + camera->x)/float(tileSize)) * tileSize - camera->x;
        cursorPos.position.y = floor(float(posY + camera->y)/float(tileSize)) * tileSize - camera->y + WINDOW_MENUBAR_HEIGHT;
      }
    }

    void onCanvasCreated(CanvasCreatedEvent& event) {
      for (auto entity: getSystemEntities()) { // should be singleton actually
        auto& cursorPos = entity.getComponent<CursorTileComponent>();
        cursorPos.tileSize = event.tileSize;
      }
    }

    void onCanvasOpened(CanvasOpenedEvent& event) {
      for (auto entity: getSystemEntities()) { // should be singleton actually
        auto& cursorPos = entity.getComponent<CursorTileComponent>();
        cursorPos.tileSize = event.tileSize;
      }
    }

    void onSnapToggled() {
      // TODO ... SnapToggledEvent& event
      // for (auto entity: getSystemEntities()) { // should be singleton actually
      //   auto& cursorPos = entity.getComponent<CursorTileComponent>();
      //   cursorPos.snap = event.snap;
      // }
    }
};

#endif
