#include "ECS.h"

int Entity::GetId() const {
	return id;
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
