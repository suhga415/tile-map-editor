#ifndef TILESETCHANGEDEVENT_H
#define TILESETCHANGEDEVENT_H

#include "../EventBus/Event.h"
#include <SDL2/SDL.h>

class TileSetChangedEvent: public Event {
  public:
    std::string assetId;

    TileSetChangedEvent(std::string assetId) {
      this->assetId = assetId;
    }

    ~TileSetChangedEvent() = default;
};

#endif
