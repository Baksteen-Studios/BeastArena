#ifndef FILE_LEVEL_SCENE_HPP
#define FILE_LEVEL_SCENE_HPP

#include "scenes/beast_scene.hpp"
#include "brickengine/json/json.hpp"
#include "level/player_spawn.hpp"
#include "level/gadget_spawn.hpp"
#include "level/solid.hpp"

class LevelScene : BeastScene<LevelScene> {
public:
    LevelScene(Json json);

    static std::string getTagStatic() {
        return "LevelScene";
    }
    GameState getSystemState() {
        return GameState::InGame;
    }

    void prepare();
    void start();
    void leave();

private:
    Json json;
    
    // General information
    double version;
    std::string name;
    std::string description;

    // Player spawns
    std::vector<PlayerSpawn> player_spawns;

    // Gadget spawns
    std::vector<GadgetSpawn> gadget_spawns;

    // Solids (floors and walls)
    std::vector<Solid> solids;
};

#endif // FILE_LEVEL_SCENE_HPP