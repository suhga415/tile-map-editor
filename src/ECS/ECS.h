#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <set>
#include <deque>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include "../Logger/Logger.h"

const unsigned int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent {
  protected:
    static int nextId;
};

template <typename T>
class Component: public IComponent {
  public:
    static int getId() {
      static auto id = nextId++;
      return id;
    }
};

class Entity {
  private:
    int id;

  public:
    Entity(int id): id(id) {};
    Entity(const Entity& entity) = default;

    int getId() const;
    void kill();

    // operator overloading
    bool operator == (const Entity& other) const { return this->id == other.id; }
    bool operator != (const Entity& other) const { return this->id != other.id; }
    bool operator > (const Entity& other) const { return this->id > other.id; }
    bool operator < (const Entity& other) const { return this->id < other.id; }

    // Component management
    template <typename TComponent, typename ...TArgs> void addComponent(TArgs&& ...args);
    template <typename TComponent> void removeComponent();
    template <typename TComponent> bool hasComponent() const;
    template <typename TComponent> TComponent& getComponent() const;

    // a pointer to the entity's owner registry (Be careful! - cyclic dependency)
    class Registry *registry;
};

class System {
  private:
    Signature componentSignature;
    std::vector<Entity> entities;

  public:
    System() = default;
    ~System() = default;

    void addEntityToSystem(Entity entity);
    void removeEntityFromSystem(Entity entity);
    std::vector<Entity> getSystemEntities() const;
    const Signature& getComponentSignature() const;
    
    template <typename TComponent> void requireComponent();
};

class IPool {
  public:
    virtual ~IPool() {}
};

template <typename T>
class Pool: public IPool {
  private:
    std::vector<T> data;

  public:
    Pool(int size = 100) {
      data.resize(size);
    }

    virtual ~Pool() = default;

    bool isEmpty() const {
      return data.empty();
    }

    int getSize() const {
      return data.size();
    }

    void resize(int size) {
      data.resize(size);
    }

    void clear() {
      data.clear();
    }

    void add(T object) {
      data.push_back(object);
    }

    void set(int index, T object) {
      data[index] = object;
    }

    T& get(int index) {
      return static_cast<T&>(data[index]);
    }

    T& operator [] (unsigned int index) {
      return data[index];
    }
};

class Registry {
  private:
    int numEntities = 0;
    std::vector<std::shared_ptr<IPool>> componentPools;
    std::vector<Signature> entityComponentSignatures;
    std::unordered_map<std::type_index, std::shared_ptr<System>> systems;
    std::set<Entity> entitiesToBeAdded;
    std::set<Entity> entitiesToBeKilled;
    std::deque<int> freeIds;

  public:
    Registry() {
      Logger::Info("Registry constructor called!");
    }

    ~Registry() {
      Logger::Info("Registry destructor called!");
    }
    // Entity management
    Entity createEntity();
    void killEntity(Entity entity);

    // Component management
    template <typename TComponent, typename ...TArgs> void addComponent(Entity entity, TArgs&& ...args);
    template <typename TComponent> void removeComponent(Entity entity);
    template <typename TComponent> bool hasComponent(Entity entity) const;
    template <typename TComponent> TComponent& getComponent(Entity entity) const;
    template <typename TComponent> TComponent& getComponentByEntityId(int entityId) const;

    // System management
    template <typename TSystem, typename ...TArgs> void addSystem(TArgs&& ...args);
    template <typename TSystem> void removeSystem();
    template <typename TSystem> bool hasSystem() const;
    template <typename TSystem> TSystem& getSystem() const;
    
    void addEntityToSystems(Entity entity);
    void removeEntityFromSystems(Entity entity);
    
    void update();
};

template <typename TComponent>
void System::requireComponent() {
  const auto componentId = Component<TComponent>::getId();
  componentSignature.set(componentId);
}


template <typename TComponent, typename ...TArgs>
void Registry::addComponent(Entity entity, TArgs&& ...args) {
  // get the component id and entity id
  const auto componentId = Component<TComponent>::getId();
  const auto entityId = entity.getId();

  // resize the componentPools if needed
  if (componentPools.size() <= componentId) {
    componentPools.resize(componentId + 1, nullptr);
  }

  // if we don't have a Pool for that component type (first time creating a pool for that component)
  if (!componentPools[componentId]) {
    std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
    componentPools[componentId] = newComponentPool;
  }

  // get the pool for that component type
  std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

  // resize the component pool if needed
  if (componentPool->getSize() <= entityId) {
    componentPool->resize(numEntities);
  }

  // create a new component object of the type T, and forward the parameters
  TComponent newComponent(std::forward<TArgs>(args)...); // 근데 왜 new를 쓰지 않는 거야???

  // add the new component to the component pool list, using the entity id as index
  componentPool->set(entityId, newComponent);

  // enable the component signature of the entity
  entityComponentSignatures[entityId].set(componentId);

  // Logger::Info("Component id: " + std::to_string(componentId) + " has been added to entity id: " + std::to_string(entityId));
}

template <typename TComponent>
void Registry::removeComponent(Entity entity) {
  const auto componentId = Component<TComponent>::getId();
  const auto entityId = entity.getId();
  // Pool<TComponent> *componentPool = componentPools[componentId];
  // componentPool->set(entityId, nullptr);
  entityComponentSignatures[entityId].set(componentId, false);

  // Logger::Info("Component id: " + std::to_string(componentId) + " has been removed from entity id: " + std::to_string(entityId));
}

template <typename TComponent>
bool Registry::hasComponent(Entity entity) const {
  const auto componentId = Component<TComponent>::getId();
  const auto entityId = entity.getId();
  return entityComponentSignatures[entityId].test(componentId);
}

template <typename TComponent>
TComponent& Registry::getComponent(Entity entity) const {
  const auto componentId = Component<TComponent>::getId();
  const auto entityId = entity.getId();
  // Pool<TComponent> *componentPool = componentPools[componentId];
  auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
  return componentPool->get(entityId);
}

template <typename TComponent>
TComponent& Registry::getComponentByEntityId(int entityId) const {
  const auto componentId = Component<TComponent>::getId();
  auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
  return componentPool->get(entityId);
}

template <typename TSystem, typename ...TArgs>
void Registry::addSystem(TArgs&& ...args) {
  std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
  systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template <typename TSystem>
void Registry::removeSystem() {
  auto system = systems.find(std::type_index(typeid(TSystem))); // returns iterator pointer
  systems.erase(system);
}

template <typename TSystem>
bool Registry::hasSystem() const {
  return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template <typename TSystem>
TSystem& Registry::getSystem() const {
  auto system = systems.find(std::type_index(typeid(TSystem))); // returns iterator pointer
  return *(std::static_pointer_cast<TSystem>(system->second));
}

template <typename TComponent, typename ...TArgs>
void Entity::addComponent(TArgs&& ...args) {
  this->registry->addComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template <typename TComponent>
void Entity::removeComponent() {
  this->registry->removeComponent<TComponent>(*this);
}

template <typename TComponent>
bool Entity::hasComponent() const {
  return this->registry->hasComponent<TComponent>(*this);
}

template <typename TComponent>
TComponent& Entity::getComponent() const {
  return this->registry->getComponent<TComponent>(*this);
}

#endif
