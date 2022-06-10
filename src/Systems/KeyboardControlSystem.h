#ifndef KEYBOARDMOVEMENTSYSTEM_H
#define KEYBOARDMOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "../Components/KeyboardComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"

class KeyboardControlSystem: public System {
  public:
    KeyboardControlSystem() {
      requireComponent<KeyboardComponent>();
    }

    void subscribeToEvents(std::shared_ptr<EventBus>& eventBus) {
      eventBus->subscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::onKeyPressed);
    }

    void onKeyPressed(KeyPressedEvent& event) {
      std::string keyCode = std::to_string(event.symbol);
      std::string keySymbol(1, event.symbol);
      // Logger::Info("Key pressed event emitted: [" + keyCode + "] " + keySymbol);

    }

    void update() {
      // ...
    }
};

#endif
