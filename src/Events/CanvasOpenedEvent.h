#ifndef CANVASOPENEDEVENT_H
#define CANVASOPENEDEVENT_H

#include "../EventBus/Event.h"
#include "../Structs/Tile.h"
#include <string>
#include <vector>

class CanvasOpenedEvent: public Event {
  public:
    std::string assetId;
    int tileSize;
    int tileNumX;
    int tileNumY;
    float scale;
    std::vector<Tile> assignedTiles; // a 2d tile-index data

    CanvasOpenedEvent(std::string assetId, int tileSize, int tileNumX, int tileNumY, float scale, std::vector<Tile> assignedTiles) {
      this->assetId = assetId;
      this->tileSize = tileSize;
      this->tileNumX = tileNumX;
      this->tileNumY = tileNumY;
      this->scale = scale;
      this->assignedTiles = assignedTiles;
    }

    ~CanvasOpenedEvent() = default;
};

#endif
