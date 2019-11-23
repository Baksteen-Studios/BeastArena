#ifndef FILE_BEAST_SCENE_HPP
#define FILE_BEAST_SCENE_HPP

#include <string>

#include "brickengine/scenes/scene_impl.hpp"
#include "enums/game_state.hpp"
#include "entities/entity_factory.hpp"
#include "controllers/game_controller.hpp"

template<typename SceneType>
class BeastScene : SceneImpl<SceneType, GameState> {
public:
    BeastScene(EntityFactory& factory, SceneLayer layer, int width, int height) : SceneImpl<SceneType, GameState>(layer), factory(factory), width(width), height(height) {}

    EntityFactory& factory;

    // General information
    double getRelativeModifier(); // Modifier needed to create entities at the right place with the right size
    std::string bg_path;
    std::string bg_music;

    int screen_width = GameController::SCREEN_WIDTH;
    int screen_height = GameController::SCREEN_HEIGHT;
    int width;
    int height;
private:
};

#endif // FILE_BEAST_SCENE_HPP