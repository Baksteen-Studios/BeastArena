
#include "systems/game_system.hpp"
#include "brickengine/std/random.hpp"


GameSystem::GameSystem(std::shared_ptr<EntityManager> em, GameController& gc) : entity_manager(em), game_controller(gc), System(em) {}

void GameSystem::update(double) {
    // Checking how many players are alive.
    auto players = entity_manager->getEntitiesByComponent<PlayerComponent>();
    for (auto player : players) {
        auto health = entity_manager->getComponent<HealthComponent>(player.first);
        if (health->health <= 0 && !dead_players.count(player.first)) {
            dead_players.insert(player.first);
        }
    }

    if(!dead_players.empty()) {
        int count = players.size() - dead_players.size();
        if(count <= 1) {
            // load next scene.
            dead_players.clear();
            ++completed_level_amount;
            if (completed_level_amount < GameController::MAX_LEVELS) {
                game_controller.loadNextLevel();
            } else {
                game_controller.loadMainMenu();
                completed_level_amount = 0;
            }
        }
    }
}
