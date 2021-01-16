#include "ECS.h"

// We need to assign an initial value for the static nextId atribute
int IComponent::nextId = 0;

int Entity::GetId() const {
	return id;
}

void Entity::Kill() {
	registry->KillEntity(*this);
}

void System::AddEntityToSystem(Entity entity){
	entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity) {
	// Could use a for instead...
	entities.erase(
		std::remove_if(
			entities.begin(),
			entities.end(),
			[&entity](Entity other) {
				return entity == other; // this operator is overloaded
			}
		), 
		entities.end()
	);
}

std::vector<Entity> System::GetSystemEntities() const {
	return entities;
}

const Signature& System::GetComponentSignature() const {
	return componentSignature;
}

Entity Registry::CreateEntity() {
	int entityId;

	if (freeIds.empty()) {
		// If there are no free ids waiting to be reused
		entityId = numEntities++;
		if (entityId >= entityComponentSignatures.size()) {
			entityComponentSignatures.resize(entityId + 1);
		}
	}
	else {
		// Reuse an id from the list of previously removed entities
		entityId = freeIds.front();
		freeIds.pop_front();
	}

	Entity entity(entityId);
	entity.registry = this;
	entitiesToBeAdded.insert(entity);
	spdlog::info("Entity created with id {0}", entityId);

	return entity;
}


void Registry::KillEntity(Entity entity) {
	entitiesToBeKilled.insert(entity);
	spdlog::info("Entity killed with id {0}", entity.GetId());
}

void Registry::AddEntityToSystems(Entity entity) {
	// This function is responsible from getting this entity, comparing the component
	// signatures of this entity with the systems and try to match all those system 
	// that require some components, if those components that the system requires
	// match the components of this entity, if the entity has all the components that
	// a system requires, then we should add this entity to the system  
	const auto entityId = entity.GetId();

	// todo: Match entityComponentSignature <---> entityComponentSignatures
	const auto& entityComponentSignature = entityComponentSignatures[entityId];
	// Loop all the systems
	for (auto& system : systems) {
		const auto& systemComponentSignature = system.second->GetComponentSignature();

		// & bitwise -> compare the items in two boolean arrays one by one doing & operation resulting in other array
		// if the result is equal to systemComponentSignature I can say that my system is indeed interested in that entity
		bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;
		if (isInterested) {
			// todo: add the entity to the system
			system.second->AddEntityToSystem(entity);
		}
	}
}

void Registry::RemoveEntityFromSystems(Entity entity) {
	for (auto system : systems) {
		system.second->RemoveEntityFromSystem(entity);
	}
}

void Registry::Update() {
	// Add the entities that are waiting to be created to the active Systems
	for (auto entity : entitiesToBeAdded) {
		AddEntityToSystems(entity);
	}
	entitiesToBeAdded.clear();

	// Process the entities that are waiting to be killed from the active Systems
	for (auto entity : entitiesToBeKilled) {
		RemoveEntityFromSystems(entity);
		entityComponentSignatures[entity.GetId()].reset();

		// Make the entity id available to be reused
		freeIds.push_back(entity.GetId());
	}
	entitiesToBeKilled.clear();
}