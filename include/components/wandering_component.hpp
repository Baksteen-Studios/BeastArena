#ifndef FILE_WANDERING_COMPONENT_HPP
#define FILE_WANDERING_COMPONENT_HPP

#include "brickengine/components/component_impl.hpp"
#include <functional>

class WanderingComponent : public ComponentImpl<WanderingComponent> {
public:
    WanderingComponent();
    static std::string getNameStatic();

    // Data
    double duration = 1;
    double elapsed_time = 0;
    double waited_for = 0;
    double wait_duration = 10;
    int random = 0;
    bool can_move = false;
};

#endif // FILE_WANDERING_COMPONENT_HPP
