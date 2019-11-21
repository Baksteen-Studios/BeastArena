#include <iostream>
#include <tuple>

#include "brickengine/components/transform_component.hpp"
#include "brickengine/components/physics_component.hpp"
#include "brickengine/components/player_component.hpp"
#include "brickengine/input.hpp"

#include "systems/movement_system.hpp"
#include "player_input.hpp"

MovementSystem::MovementSystem(std::shared_ptr<CollisionDetector> cd,
    std::shared_ptr<EntityManager> entityManager, std::shared_ptr<EntityFactory> ef)
    : BeastSystem(ef, entityManager), collision_detector(cd) {}

void MovementSystem::update(double) {
    auto input = BrickInput<PlayerInput>::getInstance();
    auto entitiesWithPlayer = entityManager->getEntitiesByComponent<PlayerComponent>();

     for (auto& [entity_id, player]: entitiesWithPlayer) {
        if (player->disabled) continue;

        auto physics = entityManager->getComponent<PhysicsComponent>(entity_id);
        if (!physics) continue;

        double vx = physics->vx;
        double vy = physics->vy;
        double mass = physics->mass;

        // Moving left or right
        // TODO add the modifier to the calculation
        auto x_movement = input.checkInput(player->player_id, PlayerInput::X_AXIS);
        std::cout << x_movement << std::endl;
        if (x_movement < 0) {
            if (vx > 0) vx = 0;
            vx += -1 * TERMINAL_VELOCITY * MOVEMENT_FORCE / mass;
            if (vx < (TERMINAL_VELOCITY * -1) / mass) {
                vx = (TERMINAL_VELOCITY * -1) / mass;
            }
        }
        else if (x_movement > 0) {
            if (vx < 0) 
                vx = 0;
            vx += TERMINAL_VELOCITY * MOVEMENT_FORCE / mass;
            if (vx > TERMINAL_VELOCITY / mass) {
                vx = TERMINAL_VELOCITY / mass;
            }
        } else {
            vx = 0;
        }
        // Jumping
        // TODO check if it is positive
        if (input.checkInput(player->player_id, PlayerInput::Y_AXIS)) {
            bool standsOnPlatform = collision_detector->spaceLeft(entity_id, Axis::Y, Direction::POSITIVE).space_left == 0;

            if (standsOnPlatform) {
                vy = -1 * (JUMP_FORCE / mass);
            }
        }

        physics->vx = vx;
        physics->vy = vy;
    }
}