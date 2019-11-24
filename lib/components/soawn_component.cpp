#include "components/spawn_component.hpp"

SpawnComponent::SpawnComponent(GadgetSpawnType gadget_spawn_type, std::vector<std::string> available_spawns, int x, int y, int respawn_timer) 
                : gadget_spawn_type(gadget_spawn_type), available_spawns(available_spawns), x(x), y(y), respawn_timer(respawn_timer)  {}

std::string SpawnComponent::SpawnComponent::getNameStatic() {
    return "SpawnComponent";
}