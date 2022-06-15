#ifndef CURSORPOSCOMPONENT_H
#define CURSORPOSCOMPONENT_H

#include <glm/glm.hpp>

struct CursorPosComponent {
  glm::vec2 position;
  int tileSize;
  bool snap;
    
  CursorPosComponent(
    glm::vec2 position = glm::vec2(0.0, 0.0),
    int tileSize = 0,
    bool snap = false
  ) {
    this->position = position;
    this->tileSize = tileSize;
    this->snap = snap;
  }
};

#endif
