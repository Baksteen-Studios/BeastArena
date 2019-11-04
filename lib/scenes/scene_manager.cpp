#include "scenes/scene_manager.hpp"

#include <vector>

#include "entities/entity_factory.hpp"
#include "brickengine/entities/entity_manager.hpp"
#include "level/level.hpp"
#include "entities/layers.hpp"

#include "level/solid.hpp"
#include "level/player_spawn.hpp"

SceneManager::SceneManager(std::shared_ptr<EntityFactory> entity_factory, std::shared_ptr<EntityManager> entity_manager, BrickEngine* engine) : entity_factory(entity_factory), entity_manager(entity_manager), engine(engine) {};

void SceneManager::loadLevel(Level* level) const {
    // Create the background
    current_scene_entities.push_back(entity_factory->createImage(level->bg_path, engine->getWindowWidth() / 2, engine->getWindowHeight() / 2, engine->getWindowWidth(), engine->getWindowHeight(), Layers::Background));

    // Load the music
    // TODO

    // Load the player spawns
    // TODO
    for(PlayerSpawn playerSpawn : level->player_spawns){
        
    }
    
    // Load the gadget spawns
    // TODO
    for(GadgetSpawn gadgetSpawn : level->gadget_spawns){
        
    }

    // Create the platforms
    for(Solid platform : level->solids) {
        if(platform.type == SolidType::RECTANGLE) {
            int x = platform.x / level->relative_modifier;
            int y = platform.y / level->relative_modifier;
            int xScale = platform.xScale / level->relative_modifier;
            int yScale = platform.yScale / level->relative_modifier;
            entity_factory->createPlatform(x, y, xScale, yScale);
        }
    }
}

void SceneManager::destroyCurrentScene() const {
    
}