#pragma once

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include <spdlog/spdlog.h>

class CollisionSystem: public System {
    public:
        CollisionSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<BoxColliderComponent>();
        }

        void Update() {
            auto entities = GetSystemEntities();

            // Loop all the entities that the system is interested in
            for (auto i = entities.begin(); i != entities.end(); i++) {
                Entity a = *i;
                auto aTransform = a.GetComponent<TransformComponent>();
                auto aCollider = a.GetComponent<BoxColliderComponent>();

                // Loop all the entities that still need to be checked (to the right of i)
                for (auto j = i; j != entities.end(); j++) {
                    Entity b = *j;

                    // Bypass if we are trying to test the same entity
                    if (a == b) {
                        continue;
                    }

                    auto bTransform = b.GetComponent<TransformComponent>();
                    auto bCollider = b.GetComponent<BoxColliderComponent>();
                 
                    // Perform the AABB collision check between entities a and b
                    bool collisionHappened = CheckAABBCollision(
                        aTransform.position.x + aCollider.offset.x,
                        aTransform.position.y + aCollider.offset.y,
                        aCollider.width,
                        aCollider.height,
                        bTransform.position.x + bCollider.offset.x,
                        bTransform.position.y + bCollider.offset.y,
                        bCollider.width,
                        bCollider.height
                    );

                    if (collisionHappened) {
                        spdlog::info("EntityId = {} is colliding with entity {}", a.GetId(), b.GetId());
                        // TODO: emit an event...
                    }
                }
            }
        }

        bool CheckAABBCollision(double aX, double aY, double aW, double aH, double bX, double bY, double bW, double bH) {
            return (
                aX < bX + bW &&
                aX + aW > bX &&
                aY < bY + bH &&
                aY + aH > bY
            );
        }
};