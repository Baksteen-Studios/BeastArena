#include "brickengine/input.hpp"
#include "brickengine/components/player_component.hpp"

#include "systems/pickup_system.hpp"
#include "components/pickup_component.hpp"
#include "player_input.hpp"

PickupSystem::PickupSystem(std::shared_ptr<CollisionDetector> cd,
    std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef)
    : BeastSystem(ef, em), collision_detector(cd) {}

void PickupSystem::update(double){    
    auto input = BrickInput<PlayerInput>::getInstance();
    auto entities_with_player = entityManager->getEntitiesByComponent<PlayerComponent>();

    for (auto& [entity_id, player]: entities_with_player) {
        if (player->disabled) continue;

        if (input.checkInput(player->player_id, PlayerInput::GRAB)) {
            std::cout << "grab inputed" << std::endl;
            auto children = entityManager->getChildren(entity_id);
            if (!children.empty()) {
                std::cout << "trying to drop children" << std::endl;
                for (const int& child : children) {
                    auto child_pickup = entityManager->getComponent<PickupComponent>(child);
                    if(child_pickup) {
                        entityManager->moveOutOfParentsHouse(child);
                    }
                }
            }
            else {
                std::cout << "trying to pickup" << std::endl;
                auto trigger = collision_detector->isInTrigger(entity_id);
                if (!trigger.object_id || !trigger.is_in_trigger) continue;

                auto trigger_pickup = entityManager->getComponent<PickupComponent>(*trigger.object_id);
                if(trigger_pickup){
                    std::cout << "initiating pickup" << std::endl;
                    auto collider_id = trigger.object_id.value();
                    auto collider_transform = entityManager->getComponent<TransformComponent>(collider_id);
                    auto entity_transform = entityManager->getComponent<TransformComponent>(entity_id);
                    auto [ entity_position, entity_scale ] = entityManager->getAbsoluteTransform(entity_id);
                    if (entity_transform->x_direction == Direction::NEGATIVE) {
                        collider_transform->x_pos = entity_transform->x_scale / 2 * -1;
                    } else {
                        collider_transform->x_pos = entity_transform->x_scale / 2;
                    }
                    if (trigger_pickup->match_parent_x_direction)
                        collider_transform->x_direction = entity_transform->x_direction;
                    if (trigger_pickup->match_parent_y_direction)
                        collider_transform->y_direction = entity_transform->y_direction;
                    collider_transform->y_pos = 0;
                    collider_transform->x_scale /= entity_scale.x;
                    collider_transform->y_scale /= entity_scale.y;
                    entityManager->setParent(collider_id, entity_id, true);
                }
            }
        }
    }
}