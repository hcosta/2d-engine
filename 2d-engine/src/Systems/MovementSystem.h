#pragma once

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include <spdlog/spdlog.h>

class MovementSystem : public System {
public:
	MovementSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();
	}

	void Update(double deltaTime) {
		// Loop all entities that the system is interested in
		for (auto entity : GetSystemEntities()) {
			// Update entity position based on its velocity every frame of the game loop.
			auto& transform = entity.GetComponent<TransformComponent>();
			const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

			transform.position.x += rigidbody.velocity.x * static_cast<float>(deltaTime);
			transform.position.y += rigidbody.velocity.y * static_cast<float>(deltaTime);

			// spdlog::info("EntityId = {0} position is now ({1}, {2})", entity.GetId(), transform.position.x, transform.position.y);
		}
	}
};