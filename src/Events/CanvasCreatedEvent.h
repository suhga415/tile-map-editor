#ifndef CANVASCREATEEVENT_H
#define CANVASCREATEEVENT_H

#include "../EventBus/Event.h"

class CanvasCreatedEvent: public Event {
  public:
    std::string filePath;
    int tileSize;
    int tileNumX;
    int tileNumY;

    CanvasCreatedEvent(std::string filePath, int tileSize, int tileNumX, int tileNumY) {
      this->filePath = filePath;
      this->tileSize = tileSize;
      this->tileNumX = tileNumX;
      this->tileNumY = tileNumY;
    }

    ~CanvasCreatedEvent() = default;
};

#endif
