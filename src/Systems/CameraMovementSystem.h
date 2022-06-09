#ifndef CAMERAMOVEMENTSYSTEM_H
#define CAMERAMOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/MouseDragEvent.h"

class CameraMovementSystem: public System {
  private:
    std::shared_ptr<EventBus> eventBus;

  public:
    CameraMovementSystem() = default;

    void subscribeToEvents(std::shared_ptr<EventBus>& eventBus) {
      this->eventBus = eventBus;
      eventBus->subscribeToEvent<MouseDragEvent>(this, &CameraMovementSystem::onMouseDragged);
    }

    void onMouseDragged(MouseDragEvent& event) {
      // move camera view
    }

    void update() {
      // ...
    }
};

#endif
