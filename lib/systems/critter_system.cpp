#include "systems/critter_system.hpp"
#include "components/wandering_component.hpp"
#include "brickengine/components/player_component.hpp"
#include "brickengine/components/colliders/rectangle_collider_component.hpp"
#include "brickengine/std/random.hpp"
#include <tuple>

CritterSystem::CritterSystem(std::shared_ptr<CollisionDetector> cd, std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef)
    : BeastSystem(ef, em), collision_detector(cd) {}

void CritterSystem::update(double deltatime){
    auto& r = Random::getInstance();
    auto entities_with_wandering = entityManager->getEntitiesByComponent<WanderingComponent>();

    for (auto& [entity_id, wander]: entities_with_wandering) {
        auto physics = entityManager->getComponent<PhysicsComponent>(entity_id);
        if (!physics) continue;

        double vx = physics->vx;
        double vy = physics->vy;
        double mass = physics->mass;

        if (wander->waited_for < 1 && !wander->can_move) {
            wander->waited_for += deltatime;
        }
        // Reset variables
        else if (!wander->can_move){
            wander->can_move = true;
            wander->elapsed_time = 0;
            wander->waited_for = 0;
            wander->random = r.getRandomInt(0, 1);
            // If critter is in front of a wall and is trying to go that direction use opposite direction
            if (wander->random == 0) {
                auto right = collision_detector->spaceLeft(entity_id, Axis::X, Direction::NEGATIVE);
                if (right.space_left * -1 <= 0) {
                    wander->random = 1;
                }
            } else {
                auto left = collision_detector->spaceLeft(entity_id, Axis::X, Direction::POSITIVE);
                if(left.space_left <= 0){
                    wander->random = 0;
                }
            }
            wander->duration = r.getRandomInt(0, 20);
        }

        if(wander->elapsed_time < wander->duration / 10 && wander->can_move){
            wander->elapsed_time += deltatime;
            switch (wander->random) {
                // Right
                case 0:{
                    if (vx > 0) vx = 0;
                    vx += -1 * TERMINAL_VELOCITY * MOVEMENT_FORCE / mass;
                    if (vx < (TERMINAL_VELOCITY * -1) / mass) {
                        vx = TERMINAL_VELOCITY * -1 / mass;
                    }
                    // If there is a entity in front of the critter start jumping
                    auto right = collision_detector->spaceLeft(entity_id, Axis::X, Direction::NEGATIVE);
                    if (right.space_left <= 0) {
                        bool on_platform = collision_detector->spaceLeft(entity_id, Axis::Y, Direction::POSITIVE).space_left == 0;

                        if (on_platform) {
                            vy = -1 * (JUMP_FORCE / mass);
                        }                    
                    }}
                break;
                // Left
                case 1:
                    if (vx < 0) vx = 0;
                    vx += TERMINAL_VELOCITY * MOVEMENT_FORCE / mass;
                    if (vx > TERMINAL_VELOCITY / mass) {
                        vx = TERMINAL_VELOCITY / mass;
                    }
                    // If there is a entity in front of the critter start jumping
                    auto left = collision_detector->spaceLeft(entity_id, Axis::X, Direction::POSITIVE);
                    if (left.space_left <= 0) {
                        bool on_platform = collision_detector->spaceLeft(entity_id, Axis::Y, Direction::POSITIVE).space_left == 0;

                        if (on_platform) {
                            vy = -1 * (JUMP_FORCE / mass);
                        }                    
                    }
                break;
            }                    
            physics->vx = vx + r.getRandomInt(1, 5);
            physics->vy = vy;
        } else if (wander->can_move) {
            wander->can_move = false;
            int wait = r.getRandomInt(5, 10);
            wander->waited_for = 0;
        }
    }
}