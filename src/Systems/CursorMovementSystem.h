#ifndef CURSORMOVEMENTSYSTEM_H
#define CURSORMOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/CursorPosComponent.h"

class CursorMovementSystem: public System {
  public:
    CursorMovementSystem() {
      requireComponent<CursorPosComponent>();
    }

    void update(int posX, int posY) {
      for (auto entity: getSystemEntities()) {
        auto& cursorPos = entity.getComponent<CursorPosComponent>();
        cursorPos.position.x = posX;
        cursorPos.position.y = posY;
      }
    }
};

#endif
