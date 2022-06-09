#ifndef MOUSECLICKEVENT_H
#define MOUSECLICKEVENT_H

#include "../EventBus/Event.h"
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

class MouseClickEvent: public Event {
  public:
    glm::vec2 location;
    SDL_Rect camera;

    MouseClickEvent(glm::vec2 location): location(location) {}
    MouseClickEvent(int x, int y, SDL_Rect& camera) {
      location.x = x;
      location.y = y;
      this->camera = camera;
    }

    ~MouseClickEvent() = default;
};

#endif
