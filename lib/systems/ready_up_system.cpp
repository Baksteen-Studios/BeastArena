#include "brickengine/input.hpp"
#include "brickengine/components/player_component.hpp"
#include "components/ready_component.hpp"
#include "components/stats_component.hpp"

#include "systems/ready_up_system.hpp"
#include "player_input.hpp"

ReadyUpSystem::ReadyUpSystem(std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef,
                            std::function<void()> ready_function)
    : BeastSystem(ef, em), ready_function(ready_function){}

void ReadyUpSystem::update(double delta_time){
    auto entities_with_player = entityManager->getEntitiesByComponent<PlayerComponent>();

    if(player_ready.size() == entities_with_player.size())
        everyone_is_ready = true;
    else {
        everyone_is_ready = false;
        timer = 0;
    }

    auto& input = BrickInput<PlayerInput>::getInstance();

    for (auto& [entity_id, player]: entities_with_player) {
        if (player->disabled) continue;

        auto ready_component = entityManager->getComponent<ReadyComponent>(entity_id);
        auto children = entityManager->getChildren(entity_id);
        for(int id : children) {
            if(entityManager->hasTag(id, "Ready")) {
                if(!ready_component->readied_up) {
                    ready_component->readied_up = true;
                    auto stats_component = entityManager->getComponent<StatsComponent>(entity_id);
                    // Adding a new texture that shows the user has readied up in the endgame screen. Where the image is placed is hardcoded, When scaling is added to the game, 
                    // this code should be taken into consideration for changing when the 
                    auto comps = entity_factory->createImage("menu/check.png", (1600 * 0.75) + 20, 200 + ((stats_component->last_game_result - 1) * 50), 56, 44, 1, Layers::UI, 255);
                    comps.tags.push_back("player" + std::to_string(player->player_id) + "ready");
                    entity_factory->addToEntityManager(std::move(comps), "EndScene");
                    player_ready.insert(player->player_id);
                }
            }
        }
        if(!children.size()) {
            if(player_ready.count(player->player_id)) {
                player_ready.erase(player->player_id);
                ready_component->readied_up = false;
                entityManager->removeEntitiesWithTag("player" + std::to_string(player->player_id) + "ready");
            }
        }
    }
    if(everyone_is_ready) {
        timer += delta_time;

        if(timer > 1){
            this->ready_function();
        }
    }
}

void ReadyUpSystem::reset() {
    this->timer = 0;
    this->everyone_is_ready = false;
    player_ready.clear();
}