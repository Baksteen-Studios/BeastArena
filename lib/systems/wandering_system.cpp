#include "systems/wandering_system.hpp"
#include "brickengine/components/player_component.hpp"
#include "brickengine/components/colliders/rectangle_collider_component.hpp"
#include <tuple>

WanderingSystem::WanderingSystem(std::shared_ptr<CollisionDetector> cd, std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef)
    : BeastSystem(ef, em), collision_detector(cd) {}

void WanderingSystem::update(double deltatime){
    
}
