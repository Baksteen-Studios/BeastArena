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

        if(elapsed_time < wander->duration && can_move){
            elapsed_time += deltatime;
            switch (random){
                case 0:{
                    if (vx > 0) vx = 0;
                    vx += -1 * TERMINAL_VELOCITY * MOVEMENT_FORCE / mass;
                    if (vx < (TERMINAL_VELOCITY * -1) / mass) {
                        vx = (TERMINAL_VELOCITY * -1) / mass;
                    }
                    auto right = collision_detector->spaceLeft(entity_id, Axis::X, Direction::NEGATIVE);
                    if (right.space_left <= 0) {
                        bool on_platform = collision_detector->spaceLeft(entity_id, Axis::Y, Direction::POSITIVE).space_left == 0;

                        if (on_platform) {
                            vy = -1 * (JUMP_FORCE / mass);
                        }                    
                    }}
                break;
                case 1:
                    if (vx < 0) vx = 0;
                    vx += TERMINAL_VELOCITY * MOVEMENT_FORCE / mass;
                    if (vx > TERMINAL_VELOCITY / mass) {
                        vx = TERMINAL_VELOCITY / mass;
                    }
                    auto left = collision_detector->spaceLeft(entity_id, Axis::X, Direction::POSITIVE);
                    if (left.space_left <= 0) {
                        bool on_platform = collision_detector->spaceLeft(entity_id, Axis::Y, Direction::POSITIVE).space_left == 0;

                        if (on_platform) {
                            vy = -1 * (JUMP_FORCE / mass);
                        }                    
                    }
                break;
            }
        } else if (can_move) {
            can_move = false;
            int wait = r.getRandomInt(5, 10);
            waited_for = 0;
        }

        if (waited_for < 1 && !can_move) {
            waited_for += deltatime;
        }
        else if (!can_move){
            can_move = true;
            elapsed_time = 0;
            waited_for = 0;
            random = r.getRandomInt(0, 1);
            if (random == 0) {
                auto right = collision_detector->spaceLeft(entity_id, Axis::X, Direction::NEGATIVE);
                if (right.space_left * -1 <= 0) {
                    random = 1;
                }
            } else {
                auto left = collision_detector->spaceLeft(entity_id, Axis::X, Direction::POSITIVE);
                if(left.space_left <= 0){
                    random = 0;
                }
            }
        }

        physics->vx = vx;
        physics->vy = vy;
    }
}