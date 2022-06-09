#include "./ECS.h"
#include "../Logger/Logger.h"

// define static member variable
int IComponent::nextId = 0;

int Entity::getId() const {
  return id;
}

void Entity::kill() {
  registry->killEntity(*this);
}

void System::addEntityToSystem(Entity entity) {
  entities.push_back(entity);
}

void System::removeEntityFromSystem(Entity entity) {
  entities.erase(
    std::remove_if(
      entities.begin(), entities.end(), [&entity](Entity other) {
        return entity == other;
      }
    ),
    entities.end()
  );
}

std::vector<Entity> System::getSystemEntities() const {
  return entities;
}

const Signature& System::getComponentSignature() const {
  return componentSignature;
}

Entity Registry::createEntity() {
  int entityId;
  if (freeIds.empty()) {
    entityId = numEntities++;
    if (entityComponentSignatures.size() <= entityId) {
      entityComponentSignatures.resize(entityId + 1);
    }
  } else {
    entityId = freeIds.front();
    freeIds.pop_front();
  }

  Entity entity(entityId);
  entity.registry = this;
  entitiesToBeAdded.insert(entity);

  return entity;
}

void Registry::killEntity(Entity entity) {
  entitiesToBeKilled.insert(entity);
}

void Registry::addEntityToSystems(Entity entity) {
  const auto entityId = entity.getId();
  const auto &entityComponentSignature = entityComponentSignatures[entityId];
  for (auto &system: systems) {
    const auto &systemComponentSignature = system.second->getComponentSignature();
    bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;
    if (isInterested) {
      system.second->addEntityToSystem(entity);
    }
  }
}

void Registry::removeEntityFromSystems(Entity entity) {
  for (auto system: systems) {
    system.second->removeEntityFromSystem(entity);
  }
}

void Registry::update() {
  // add entities that are waiting to be added to the active system
  for (auto entity: entitiesToBeAdded) {
    addEntityToSystems(entity);
  }
  entitiesToBeAdded.clear();

  // kill entities that are waiting to be killed from the active system
  for (auto entity: entitiesToBeKilled) {
    removeEntityFromSystems(entity);
    entityComponentSignatures[entity.getId()].reset();
    freeIds.push_back(entity.getId());
  }
  entitiesToBeKilled.clear();
}
