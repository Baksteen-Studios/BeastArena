#include "systems/game_speed_system.hpp"

#include <utility>

#include "brickengine/input.hpp"

#include "player_input.hpp"

GameSpeedSystem::GameSpeedSystem(std::shared_ptr<EntityManager> em, double *delta_time_modifier) : System(em), delta_time_modifier(delta_time_modifier) {};

void GameSpeedSystem::update(double delta_time) {
    std::ignore = delta_time;

    auto input = BrickInput<PlayerInput>::getInstance();
    double step_size = 0.2;
    bool modifier_changed = false;

    if(input.checkInput(1, PlayerInput::PAGE_DOWN)) {
        // Decrease game speed
        if(*delta_time_modifier - step_size >= 0.2) {
            *delta_time_modifier -= 0.2;
            modifier_changed = true;
        }
    } else if (input.checkInput(1, PlayerInput::PAGE_UP)) {
        // Increase game speed
        if(*delta_time_modifier + step_size <= 3) {
            *delta_time_modifier += 0.2;
            modifier_changed = true;
        }
    }

    if(modifier_changed)
        std::cout << "New game loop speed modifier = " << *delta_time_modifier << std::endl;
}