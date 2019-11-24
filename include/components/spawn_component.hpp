#ifndef FILE_SPAWN_COMPONENT_HPP
#define FILE_SPAWN_COMPONENT_HPP

#include "brickengine/components/component_impl.hpp"
#include <functional>
#include "brickengine/enum/direction.hpp"
#include "enums/gadget_spawn_type.hpp"

class SpawnComponent : public ComponentImpl<SpawnComponent> {
public:
    SpawnComponent(GadgetSpawnType gadget_spawn_type, std::vector<std::string> available_spawns, int x, int y, int respawn_timer);
    static std::string getNameStatic();

    // Data
    GadgetSpawnType gadget_spawn_type;
    std::vector<std::string> available_spawns;
    double respawn_timer;
    int x;
    int y;
    double time_elapsed = 0;
};

#endif // FILE_SPAWN_COMPONENT_HPP