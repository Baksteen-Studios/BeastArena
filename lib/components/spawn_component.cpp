#include "components/spawn_component.hpp"

SpawnComponent::SpawnComponent(int respawn_timer, std::vector<CreateGadgetCompsFn> gadget_fns) 
                : respawn_timer(respawn_timer), time_elapsed(respawn_timer), gadget_fns(gadget_fns)  {}

std::string SpawnComponent::SpawnComponent::getNameStatic() {
    return "SpawnComponent";
}