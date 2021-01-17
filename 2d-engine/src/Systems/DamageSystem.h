#pragma once

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include <spdlog/spdlog.h>

class DamageSystem: public System {
    public:
        DamageSystem() {
            RequireComponent<BoxColliderComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::onCollision);
        }

        void onCollision(CollisionEvent& event) {
            spdlog::info("The Damage system received an event collision between entities {} and {}", event.a.GetId(), event.b.GetId());
            event.a.Kill();
            event.b.Kill();
        }

        void Update() {
            // TODO:...
        }
};