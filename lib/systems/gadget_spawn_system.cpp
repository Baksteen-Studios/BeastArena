#include "brickengine/input.hpp"

#include "systems/gadget_spawn_system.hpp"
#include "components/spawn_component.hpp"
#include "components/weapon_component.hpp"
#include "brickengine/std/random.hpp"

GadgetSpawnSystem::GadgetSpawnSystem(std::shared_ptr<CollisionDetector> cd,
    std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef)
    : BeastSystem(ef, em), collision_detector(cd) {}

void GadgetSpawnSystem::update(double deltatime) {
    auto& r = Random::getInstance(); 
    auto entities_with_spawn = entityManager->getEntitiesByComponent<SpawnComponent>();
    int size = entities_with_spawn.size();

    // If there are the same amount of weapons as spawners in the game
    if (entityManager->getEntitiesByComponent<WeaponComponent>().size() == size) return;

    for (auto& [entity_id, spawn]: entities_with_spawn) {
        // If spawner has a child (gadget spawned) : continue
        if (entityManager->getChildren(entity_id).size() > 0) continue;        

        // Wait for respawn time
        if (spawn->time_elapsed < spawn->respawn_timer) {
            spawn->time_elapsed += deltatime;
        }
        // Spawn a random gadget (weapon for now)
        else {
            int random_gadget = r.getRandomInt(0, spawn->gadget_fns.size() - 1);
            auto gadget_comps = spawn->gadget_fns[random_gadget]();

            int spawned_gadget_id = entityManager->createEntity(std::move(gadget_comps), std::make_pair(entity_id, false));

            auto transform = entityManager->getComponent<TransformComponent>(spawned_gadget_id);
            transform->x_pos = 0;
            transform->y_pos = -25;

            spawn->time_elapsed = 0;
        } 
    }
}