#ifndef MOUSEDRAGEVENT_H
#define MOUSEDRAGEVENT_H

#include "../EventBus/Event.h"
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

class MouseDragEvent: public Event {
  public:
    glm::vec2 location;
    MouseDragEvent(glm::vec2 location): location(location) {}
    MouseDragEvent(int x, int y) {
      location.x = x;
      location.y = y;
    }

    ~MouseDragEvent() = default;
};

#endif
