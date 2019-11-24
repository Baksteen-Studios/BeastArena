#ifndef FILE_GADGET_SPAWN_HPP
#define FILE_GADGET_SPAWN_HPP

#include <vector>
#include <string>
#include "enums/gadget_spawn_type.hpp"

struct GadgetSpawn {
public:
    GadgetSpawnType gadget_spawn_type;
    std::vector<std::string> available_spawns;
    double respawn_timer;
    int x;
    int y;
};

#endif // FILE_GADGET_SPAWN_HPP