#include "brickengine/input.hpp"
#include "brickengine/components/player_component.hpp"
#include "components/ready_component.hpp"

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
        }

        if(ready_component->readied_up){
            ++readied_up_counter;
        }
    }

    if(readied_up_counter == entities_with_player.size()){
        this->ready_function();
    }

}