#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

class MovementSystem: public System {
  public:
    MovementSystem() {
      requireComponent<TransformComponent>();
      requireComponent<RigidBodyComponent>();
    }

    void update(float deltaTime) {
      for (auto entity: getSystemEntities()) {
        auto& transform = entity.getComponent<TransformComponent>();
        const auto rigidBody = entity.getComponent<RigidBodyComponent>();
        transform.position.x += rigidBody.velocity.x * deltaTime;
        transform.position.y += rigidBody.velocity.y * deltaTime;
      }
    }
};

#endif
