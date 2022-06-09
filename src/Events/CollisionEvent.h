#ifndef COLLISIONEVENT_H
#define COLLISIONEVENT_H

class CollisionEvent : public Event {
  public:
    Entity a;
    Entity b;
    CollisionEvent(Entity a, Entity b): a(a), b(b) {}
    ~CollisionEvent() = default;

};

#endif
