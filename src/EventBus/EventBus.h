#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <map>
#include <typeindex>
#include <list>
#include <memory>
#include <functional>
#include "./Event.h"

class IEventCallback {
  private:
    virtual void call(Event& e) = 0;

  public:
    virtual ~IEventCallback() = default;

    void execute(Event& e) {
      call(e);
    }
};

// basically a wrapper around a function pointer
template <typename TOwner, typename TEvent>
class EventCallback: public IEventCallback {
  private:
    typedef void (TOwner::*CallbackFunction)(TEvent&);

    // the callback function pointer that needs to be invoked
    TOwner* owner;
    CallbackFunction callbackFunction;

    virtual void call(Event& e) override {
      std::invoke(callbackFunction, owner, static_cast<TEvent&>(e));
    }
  
  public:
    EventCallback(TOwner* owner, CallbackFunction callbackFunction) {
      this->owner = owner;
      this->callbackFunction = callbackFunction;
    }
    virtual ~EventCallback() override = default;
};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventBus {
  private:
    std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;

  public:
    EventBus() {}
    ~EventBus() {}

    void resetSubscribers() {
      subscribers.clear();
    }

    // example: eventBus->subscribeToEvent<CollisionEvent>(this, &Game::onCollision);
    template <typename TEvent, typename TOwner>
    void subscribeToEvent(TOwner* owner, void (TOwner::*callbackFunction)(TEvent&)) {
      if (!subscribers[typeid(TEvent)].get()) {
        subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
      }
      auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(owner, callbackFunction);
      subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
    }

    // example: eventBus->emitEvent<CollisionEvent>(player, enemy);
    template <typename TEvent, typename ...TArgs>
    void emitEvent(TArgs&& ...args) {
      auto handlers = subscribers[typeid(TEvent)].get();
      if (handlers) {
        for (auto iter = handlers->begin(); iter != handlers->end(); iter++) {
          auto handler = iter->get();
          TEvent event(std::forward<TArgs>(args)...);
          handler->execute(event);
        }
        // for (auto handler: handlers) {
        //   TEvent event(std::forward<TArgs>(args)...);
        //   handler->execute(event);
        // }
      }
    }
};

#endif
