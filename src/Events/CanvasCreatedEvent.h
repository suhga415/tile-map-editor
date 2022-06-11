#ifndef CANVASCREATEEVENT_H
#define CANVASCREATEEVENT_H

#include "../EventBus/Event.h"

class CanvasCreatedEvent: public Event {
  public:
    int tileSize;
    int tileNumX;
    int tileNumY;

    CanvasCreatedEvent(int tileSize, int tileNumX, int tileNumY) {
      this->tileSize = tileSize;
      this->tileNumX = tileNumX;
      this->tileNumY = tileNumY;
    }

    ~CanvasCreatedEvent() = default;
};

#endif
