#include "systems/movement_system.hpp"
#include "brickengine/components/transform_component.hpp"
#include "components/velocity_component.hpp"

MovementSystem::MovementSystem(std::shared_ptr<EntityManager> entityManager, std::shared_ptr<EntityFactory> ef) : BeastSystem(ef, entityManager) {}

void MovementSystem::update(double) {
    //auto entitiesWithVelocity = entityManager->getEntitiesByComponent<VelocityComponent>();

    //for(auto& [entityId, movement]: *entitiesWithVelocity){
    //    TransformComponent* transform = entityManager->getComponent<TransformComponent>(entityId);

    //    //position->setX(position->getX() + (entityWithVelocity->component->getXVelocity() * deltatime));
    //    //position->setY(position->getY() + (entityWithVelocity->component->getYVelocity() * deltatime));
    //}
}