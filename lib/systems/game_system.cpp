
#include "systems/game_system.hpp"
#include "brickengine/std/random.hpp"


GameSystem::GameSystem(std::shared_ptr<EntityManager> em, GameController& gc) : entity_manager(em), game_controller(gc), System(em) {}

void GameSystem::update(double) {
    // Checking how many players are alive.
    auto players = entity_manager->getEntitiesByComponent<PlayerComponent>();
    std::set<int> dead_players;
    for (auto player : players) {
        auto health = entity_manager->getComponent<HealthComponent>(player.first);
        if (health->health <= 0 && !dead_players.count(player.first)) {
            dead_players.insert(player.first);
        }
    }

    int count =  players.size() - dead_players.size();
    if(count == 1) {
        // load next scene.
        if (completed_level_amount <= MAX_LEVELS) {
            game_controller.loadNextLevel();
            ++completed_level_amount;
        } else {
            game_controller.loadMainMenu();
            completed_level_amount = 0;
        }
    }
}