#include "systems/cheat_system.hpp"

#include "brickengine/input.hpp"
#include "player_input.hpp"

#include "brickengine/components/player_component.hpp"
#include "components/health_component.hpp"

CheatSystem::CheatSystem(std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef, GameController& gc) : game_controller(gc), BeastSystem(ef, em) {}

void CheatSystem::update(double deltatime) {
    auto& input = BrickInput<PlayerInput>::getInstance();
    int player_id = 0;
    int entity_id = 0;
    bool init = false;
    for (auto& player : entityManager->getEntitiesByComponent<PlayerComponent>()) {
        if(player.second->player_id < player_id && init) {
            player_id = player.second->player_id;
            entity_id = player.first;
        } else {
            player_id = player.second->player_id;
            entity_id = player.first;
            init = true;
        }
    }

    // Only the first player can cheat :)
    // Next scene
    if(input.checkInput(player_id, PlayerInput::SKIP_LEVEL)) {
        game_controller.loadNextLevel();
    }

    // Infinite Health
    if(input.checkInput(player_id, PlayerInput::INFINITE_HEALTH)) {
        auto health_component = entityManager->getComponent<HealthComponent>(entity_id);
        health_component->health = 99999999999999999;
    }

    // Random weapon drop
    if(input.checkInput(player_id, PlayerInput::RANDOM_WEAPON)) {

    }

    // Godmode laser
    if(input.checkInput(player_id, PlayerInput::LASER_WEAPON)) {

    }

    // Kill everyone except the first player
    if(input.checkInput(player_id, PlayerInput::KILL_EVERYONE_EXCEPT_YOURSELF)) {

    }
}