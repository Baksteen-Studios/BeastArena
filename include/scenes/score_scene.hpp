#ifndef FILE_SCORE_SCENE_HPP
#define FILE_SCORE_SCENE_HPP

#include "scenes/beast_scene.hpp"
#include "brickengine/json/json.hpp"
#include "entities/entity_factory.hpp"
#include "scenes/data/level/player_spawn.hpp"
#include "scenes/data/level/gadget_spawn.hpp"
#include "scenes/data/level/solid.hpp"
#include "scenes/data/level/critter_spawn.hpp"

class ScoreScene : public BeastScene<ScoreScene> {
public:
    ScoreScene(EntityFactory& entity_factory, BrickEngine& engine, Json json);
    
    static std::string getTagStatic() {
        return "ScoreScene";
    }
    GameState getSystemState() const {
        return GameState::EndGame;
    }
    static SceneLayer getLayerStatic() {
        return SceneLayer::Primary;
    }
    void performPrepare();
    void start();
    void leave();
private:
    Json json;
    EntityFactory& entity_factory;
};

#endif // FILE_SCORE_SCENE_HPP