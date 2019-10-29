#include <iostream>
#include <tuple>

#include "brickengine/components/transform_component.hpp"
#include "brickengine/components/player_component.hpp"
#include "components/weapon_component.hpp"
#include "brickengine/input.hpp"

#include "systems/weapon_system.hpp"
#include "player_input.hpp"

WeaponSystem::WeaponSystem(std::shared_ptr<CollisionDetector> cd,
    std::shared_ptr<EntityManager> entityManager, std::shared_ptr<EntityFactory> ef)
    : BeastSystem(ef, entityManager), collisionDetector(cd) {}

void WeaponSystem::update(double deltatime) {
    auto input = BrickInput<PlayerInput>::getInstance();
    auto entitiesWithWeapon = entityManager->getEntitiesByComponent<WeaponComponent>();

     for (auto& [entityId, weapon]: *entitiesWithWeapon) {
        
        // Picking up / dropping weapon
        auto collisionRight = collisionDetector->spaceLeft(entityId, Axis::X, Direction::POSITIVE);
        auto collisionLeft = collisionDetector->spaceLeft(entityId, Axis::X, Direction::NEGATIVE);
        auto test = entityManager->getComponent<PlayerComponent>(collisionRight.second);
        //std::cout << dynamic_cast<PlayerComponent*>(test) << std::endl;
        std::cout << "RIGHT : " << collisionRight.first << std::endl;
        std::cout << "LEFT : " << collisionLeft.first << std::endl;
        if (collisionRight.first < 1 && dynamic_cast<PlayerComponent*>(test) != 0)
        {
            std::cout << "Player is in weapon" << std::endl;
        }
    }
}