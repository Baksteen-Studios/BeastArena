#include "brickengine/input.hpp"
#include "brickengine/components/player_component.hpp"

#include "systems/pickup_system.hpp"
#include "components/pickup_component.hpp"
#include "player_input.hpp"

PickupSystem::PickupSystem(CollisionDetector2& cd,
    std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef)
    : BeastSystem(ef, em), collision_detector(cd) {}

void PickupSystem::update(double){    
    auto input = BrickInput<PlayerInput>::getInstance();
    auto entities_with_player = entityManager->getEntitiesByComponent<PlayerComponent>();

    for (auto& [entity_id, player]: entities_with_player) {
        if (player->disabled) continue;

        if (input.checkInput(player->player_id, PlayerInput::GRAB)) {
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
                auto collisions = collision_detector.detectDiscreteCollision(entity_id);
                auto is_trigger_collision = std::find_if(collisions.begin(), collisions.end(),
                [](DiscreteCollision& collision) {
                    return collision.opposite.is_trigger;
                });
                if (is_trigger_collision != collisions.end()) continue;

                int pickup_entity_id = is_trigger_collision->opposite.id;
                auto pickup = entityManager->getComponent<PickupComponent>(pickup_entity_id);
                if (pickup) {
                    auto pickup_transform = entityManager->getComponent<TransformComponent>(pickup_entity_id);
                    auto entity_transform = entityManager->getComponent<TransformComponent>(entity_id);
                    auto [ entity_position, entity_scale ] = entityManager->getAbsoluteTransform(entity_id);
                    if (entity_transform->x_direction == Direction::NEGATIVE) {
                        pickup_transform->x_pos = entity_transform->x_scale / 2 * -1;
                    } else {
                        pickup_transform->x_pos = entity_transform->x_scale / 2;
                    }
                    if (pickup->match_parent_x_direction)
                        pickup_transform->x_direction = entity_transform->x_direction;
                    if (pickup->match_parent_y_direction)
                        pickup_transform->y_direction = entity_transform->y_direction;
                    pickup_transform->y_pos = 0;
                    pickup_transform->x_scale /= entity_scale.x;
                    pickup_transform->y_scale /= entity_scale.y;
                    entityManager->setParent(pickup_entity_id, entity_id, true);
                }
            }
        }
    }
}
