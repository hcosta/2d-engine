#pragma once

class MovementSystem : public System {
public:
	MovementSystem() {
		// RequireComponent<TransformComponent>();
		// RequireComponent<...>();
	}

	void Update() {
		// Loop all entities that the system is interested in
		for (auto entity : GetEntities()) {
			// Update entity position based on its velocity
			// every frame of the game loop.
		}
	}
};