#ifndef CANVASPROPERTIESCHANGEDEVENT_H
#define CANVASPROPERTIESCHANGEDEVENT_H

#include "../EventBus/Event.h"

class CanvasPropertiesChangedEvent: public Event {
  public:
    int tileSize;
    int tileNumX;
    int tileNumY;

    CanvasPropertiesChangedEvent(int tileSize, int tileNumX, int tileNumY) {
      this->tileSize = tileSize;
      this->tileNumX = tileNumX;
      this->tileNumY = tileNumY;
    }

    ~CanvasPropertiesChangedEvent() = default;
};

#endif
