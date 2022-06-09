#ifndef SELECTEDTIMECHANGEDEVENT_H
#define SELECTEDTIMECHANGEDEVENT_H

#include "../EventBus/Event.h"
#include <SDL2/SDL.h>

class SelectedTileChangedEvent: public Event {
  public:
    int rowIdx;
    int colIdx;

    SelectedTileChangedEvent(int rowIdx, int colIdx) {
      this->rowIdx = rowIdx;
      this->colIdx = colIdx;
    }

    ~SelectedTileChangedEvent() = default;
};

#endif
