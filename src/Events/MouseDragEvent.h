#ifndef MOUSEDRAGEVENT_H
#define MOUSEDRAGEVENT_H

#include "../EventBus/Event.h"
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

class MouseDragEvent: public Event {
  public:
    // mouse click-drag from right to left --> positive value (+)
    glm::vec2 dragDist;
    std::shared_ptr<SDL_Rect> camera;

    MouseDragEvent(int x, int y, std::shared_ptr<SDL_Rect>& camera) {
      dragDist.x = x;
      dragDist.y = y;
      this->camera = camera;
    }

    ~MouseDragEvent() = default;
};

#endif
