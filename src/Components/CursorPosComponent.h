#ifndef CURSORPOSCOMPONENT_H
#define CURSORPOSCOMPONENT_H

#include <glm/glm.hpp>

struct CursorPosComponent {
  glm::vec2 position;
    
  CursorPosComponent(
    glm::vec2 position = glm::vec2(0.0, 0.0)
  ) {
    this->position = position;
  }
};

#endif
