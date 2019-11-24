#include "brickengine/input.hpp"

#include "systems/gadget_spawn_system.hpp"
#include "components/spawn_component.hpp"
#include "components/weapon_component.hpp"
#include "enums/gadget_spawn_type.hpp"
#include "brickengine/std/random.hpp"


GadgetSpawnSystem::GadgetSpawnSystem(std::shared_ptr<CollisionDetector> cd,
    std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef)
    : BeastSystem(ef, em), collision_detector(cd) {}

void GadgetSpawnSystem::update(double deltatime) {
    auto& r = Random::getInstance(); 
    auto entities_with_spawn = entityManager->getEntitiesByComponent<SpawnComponent>();
    int size = entities_with_spawn.size();
    
    // If there are the same amount of weapons as spawners in the game : continue
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
            int random_gadget = r.getRandomInt(1, spawn->available_spawns.size());
            auto gadget = spawn->available_spawns[random_gadget - 1];

            if(gadget == "pistol") {
                entity_factory->createPistol(spawn->x, spawn->y - 25, true, entity_id);
            } else if(gadget == "rifle") {
                entity_factory->createRifle(spawn->x, spawn->y - 25, true, entity_id);
            } else if(gadget == "sniper") {
                entity_factory->createSniper(spawn->x, spawn->y - 25, true, entity_id);
            }
            spawn->time_elapsed = 0;
        } 
    }
}
