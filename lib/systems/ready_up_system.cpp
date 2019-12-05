#include "brickengine/input.hpp"
#include "brickengine/components/player_component.hpp"
#include "components/ready_component.hpp"
#include "components/stats_component.hpp"

#include "systems/ready_up_system.hpp"
#include "player_input.hpp"

ReadyUpSystem::ReadyUpSystem(std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef,
                            std::function<void()> ready_function)
    : BeastSystem(ef, em), ready_function(ready_function){}

void ReadyUpSystem::update(double){    
    auto& input = BrickInput<PlayerInput>::getInstance();
    int readied_up_counter = 0;

    auto entities_with_player = entityManager->getEntitiesByComponent<PlayerComponent>();

    for (auto& [entity_id, player]: entities_with_player) {
        if (player->disabled) continue;

        auto ready_component = entityManager->getComponent<ReadyComponent>(entity_id);

        if (input.checkInput(player->player_id, PlayerInput::SHOOT)) {
            ready_component->readied_up = true;

            auto stats_component = entityManager->getComponent<StatsComponent>(entity_id);

            // Adding a new texture that shows the user has readied up in the endgame screen. Where the image is placed is hardcoded, When scaling is added to the game, 
            // this code should be taken into consideration for changing when the 
            auto comps = entity_factory->createImage("menu/check.png", (1600 * 0.75) + 20, 200 + ((stats_component->last_game_result - 1) * 50), 56, 44, 1, Layers::UI, 255);
            entity_factory->addToEntityManager(std::move(comps));
        }

        if(ready_component->readied_up){
            ++readied_up_counter;
        }
    }

    if(readied_up_counter == entities_with_player.size()){
        this->ready_function();
    }

}