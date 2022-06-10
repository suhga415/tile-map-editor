#ifndef MOUSEDRAGEVENT_H
#define MOUSEDRAGEVENT_H

#include "../EventBus/Event.h"
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

class MouseDragEvent: public Event {
  public:
    // mouse click-drag from right to left --> positive value (+)
    glm::vec2 dragDist;
    SDL_Rect camera;
    SDL_Rect canvas;

    MouseDragEvent(int x, int y, SDL_Rect& camera, SDL_Rect& canvas) {
      dragDist.x = x;
      dragDist.y = y;
      this->camera = camera;
      this->canvas = canvas;
    }

    ~MouseDragEvent() = default;
};

#endif
