#ifndef FILE_LEVEL_HPP
#define FILE_LEVEL_HPP

#include <string>
#include <vector>

#include "level/player_spawn.hpp"
#include "level/gadget_spawn.hpp"
#include "level/solid.hpp"

class Level {
public:
    // General information
    double version;
    std::string name;
    std::string description;
    double relative_modifier; // Modifier needed to create entities at the right place with the right size
    std::string bg_path;
    std::string bg_music;

    // Player spawns
    std::vector<PlayerSpawn> player_spawns;

    // Gadget spawns
    std::vector<GadgetSpawn> gadget_spawns;

    // Solids (floors and walls)
    std::vector<Solid> solids;
};

#endif // FILE_LEVEL_HPP