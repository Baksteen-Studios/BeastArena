#ifndef FILE_LOBBY_HPP
#define FILE_LOBBY_HPP

#include "scenes/menu.hpp"
#include "entities/entity_factory.hpp"
#include "brickengine/scenes/scene_manager.hpp"
#include "controllers/game_controller.hpp"

class Lobby : public Menu<Lobby> {
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
};

#endif // FILE_LOBBY_HPP