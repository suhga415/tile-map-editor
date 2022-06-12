#ifndef TILESETCHANGEDEVENT_H
#define TILESETCHANGEDEVENT_H

#include "../EventBus/Event.h"
#include "../Structs/TileSet.h"
#include <SDL2/SDL.h>

class TileSetChangedEvent: public Event {
  public:
    std::string assetId;
    TileSet selectedTileSet;

    TileSetChangedEvent(std::string assetId, TileSet selectedTileSet) {
      this->assetId = assetId;
      this->selectedTileSet = selectedTileSet;
    }

    ~TileSetChangedEvent() = default;
};

#endif
