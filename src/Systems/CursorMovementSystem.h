#ifndef CURSORMOVEMENTSYSTEM_H
#define CURSORMOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "../Components/CursorPosComponent.h"
#include "../Components/SelectedTileComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CanvasCreatedEvent.h"
#include "../Events/CanvasOpenedEvent.h"

class CursorMovementSystem: public System {
  public:
    CursorMovementSystem() {
      requireComponent<CursorPosComponent>();
      requireComponent<SelectedTileComponent>();
    }

    void subscribeToEvents(std::shared_ptr<EventBus>& eventBus) {
      eventBus->subscribeToEvent<CanvasCreatedEvent>(this, &CursorMovementSystem::onCanvasCreated);
      eventBus->subscribeToEvent<CanvasOpenedEvent>(this, &CursorMovementSystem::onCanvasOpened);
    }

    void update(int posX, int posY, std::shared_ptr<SDL_Rect>& camera) {
      for (auto entity: getSystemEntities()) { // should be singleton actually
        auto& cursorPos = entity.getComponent<CursorPosComponent>();
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
        auto& cursorPos = entity.getComponent<CursorPosComponent>();
        int tileSize = entity.getComponent<CursorPosComponent>().tileSize;
        cursorPos.position.x = ((posX + camera->x)/tileSize) * tileSize - camera->x;
        cursorPos.position.y = ((posY + camera->y)/tileSize) * tileSize - camera->y;
      }
    }

    void onCanvasCreated(CanvasCreatedEvent& event) {
      for (auto entity: getSystemEntities()) { // should be singleton actually
        auto& cursorPos = entity.getComponent<CursorPosComponent>();
        cursorPos.tileSize = event.tileSize;
      }
    }

    void onCanvasOpened(CanvasOpenedEvent& event) {
      for (auto entity: getSystemEntities()) { // should be singleton actually
        auto& cursorPos = entity.getComponent<CursorPosComponent>();
        cursorPos.tileSize = event.tileSize;
      }
    }

    void onSnapToggled() {
      // TODO ... SnapToggledEvent& event
      // for (auto entity: getSystemEntities()) { // should be singleton actually
      //   auto& cursorPos = entity.getComponent<CursorPosComponent>();
      //   cursorPos.snap = event.snap;
      // }
    }
};

#endif
