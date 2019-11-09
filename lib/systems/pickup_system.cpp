#include "brickengine/input.hpp"
#include "brickengine/components/player_component.hpp"

#include "systems/pickup_system.hpp"
#include "components/pickup_component.hpp"
#include "player_input.hpp"

PickupSystem::PickupSystem(std::shared_ptr<CollisionDetector> cd,
    std::shared_ptr<EntityManager> entityManager, std::shared_ptr<EntityFactory> ef)
    : BeastSystem(ef, entityManager), collisionDetector(cd) {}

void PickupSystem::update(double deltatime){    
    auto input = BrickInput<PlayerInput>::getInstance();
    auto entitiesWithPlayer = entityManager->getEntitiesByComponent<PlayerComponent>();

    for (auto& [entity_id, player]: *entitiesWithPlayer) {         
        if (input.checkInput(player->playerId, PlayerInput::GRAB)) {
            auto children = entityManager->getChildren(entity_id);
            if (!children.empty()) {
                for (const int& child : children) {
                    auto child_pickup = entityManager->getComponent<PickupComponent>(child);
                    if(child_pickup) {
                        entityManager->moveOutOfParentsHouse(child);
                    }
                }
            }
            else {
                auto trigger = collisionDetector->isInTrigger(entity_id);
                if(trigger.objectId.has_value() && trigger.isInTrigger){
                    auto collider_id = trigger.objectId.value();
                    auto collider_transform = entityManager->getComponent<TransformComponent>(collider_id);
                    auto entity_transform = entityManager->getComponent<TransformComponent>(entity_id);
                    auto [ entity_position, entity_scale ] = entityManager->getAbsoluteTransform(entity_id);
                    if (entity_transform->xDirection == Direction::NEGATIVE) {
                        collider_transform->xPos = -25;
                    } else {
                        collider_transform->xPos = 25;
                    }
                    collider_transform->xDirection = entity_transform->xDirection;
                    collider_transform->yDirection = entity_transform->yDirection;
                    collider_transform->yPos = 0;
                    collider_transform->xScale /= entity_scale.x;
                    collider_transform->yScale /= entity_scale.y;
                    entityManager->setParent(collider_id, entity_id, true);
                }
            }
        }
    }
}