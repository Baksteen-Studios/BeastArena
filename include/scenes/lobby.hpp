#ifndef FILE_LOBBY_HPP
#define FILE_LOBBY_HPP

#include <utility>

#include "scenes/beast_scene.hpp"
#include "entities/entity_factory.hpp"
#include "brickengine/scenes/scene_manager.hpp"
#include "controllers/game_controller.hpp"
#include "scenes/data/menu/button.hpp"
#include "scenes/data/menu/image.hpp"
#include "scenes/data/level/solid.hpp"
#include "enums/character.hpp"

class Lobby : public BeastScene<Lobby> {
public:
    Lobby(EntityFactory& factory, BrickEngine& engine, GameController& game_controller);

    static std::string getTagStatic() {
        return "Lobby";
    }
    GameState getSystemState() const {
        return GameState::Lobby;
    }
    static SceneLayer getLayerStatic() {
        return SceneLayer::Primary;
    }

    void start();
    void leave();
protected:
    void performPrepare();
private:
    static const int WIDTH = 1920;
    static const int HEIGHT = 1080;
    GameController& game_controller;

    // Buttons
    std::vector<Button> buttons;

    // Images
    std::vector<Image> images;

    // Solids
    std::vector<Solid> solids;

    // Critters
    std::vector<std::pair<int, int>> critters;

    // Character selection components --> std::pair<player_id, std::pair<x, y>>
    std::vector<std::pair<int, std::pair<int, int>>> character_selection_components;

    std::vector<Character> available_characters = {
        Character::GORILLA,
        Character::PANDA,
        Character::CHEETAH,
        Character::ELEPHANT
    };
};

#endif // FILE_LOBBY_HPP