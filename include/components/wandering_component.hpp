#ifndef FILE_WANDERING_COMPONENT_HPP
#define FILE_WANDERING_COMPONENT_HPP

#include "brickengine/components/component_impl.hpp"
#include <functional>

class WanderingComponent : public ComponentImpl<WanderingComponent> {
public:
    using DeathFunction = std::function<void (int entity_id)>;
    WanderingComponent(DeathFunction on_death);
    static std::string getNameStatic();

    // Data
    double duration = 1;
    double elapsed_time = 0;
    double waited_for = 0;
    int random = 0;
    bool can_move = false;
    DeathFunction on_death;
};

#endif // FILE_WANDERING_COMPONENT_HPP