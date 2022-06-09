#ifndef DRAWSELECTEDTILESYSTE_H
#define DRAWSELECTEDTILESYSTE_H

#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "../Components/SelectedTileComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/SelectedTileChangedEvent.h"
#include "../Constants.h"

class ChangeTileSystem: public System {
  public:
    ChangeTileSystem() {
      requireComponent<SelectedTileComponent>();
    }

    void subscribeToEvents(std::shared_ptr<EventBus>& eventBus) {
      eventBus->subscribeToEvent<SelectedTileChangedEvent>(this, &ChangeTileSystem::onTileChanged);
    }

    void onTileChanged(SelectedTileChangedEvent& event) {
      Logger::Info("[SelectedTileChangedEvent] rowIdx = " + std::to_string(event.rowIdx) + ", colIdx = " + std::to_string(event.colIdx));

      for (auto entity: getSystemEntities()) {
        auto& selectedTile = entity.getComponent<SelectedTileComponent>();
        selectedTile.rowIdx = event.rowIdx;
        selectedTile.colIdx = event.colIdx;
        if (entity.hasComponent<SpriteComponent>()) {
          auto& sprite = entity.getComponent<SpriteComponent>();
          sprite.srcRect.x = event.rowIdx * selectedTile.tileSize * selectedTile.scale;
          sprite.srcRect.y = event.colIdx * selectedTile.tileSize * selectedTile.scale;
        }
      }
    }
};

#endif
