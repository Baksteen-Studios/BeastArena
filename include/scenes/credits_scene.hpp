#ifndef FILE_CREDITS_SCENE_HPP
#define FILE_CREDITS_SCENE_HPP

#include "scenes/beast_scene.hpp"

class CreditsScene : public BeastScene<CreditsScene> {
public:
    CreditsScene(EntityFactory& factory, BrickEngine& engine, GameController& game_controller);

    static std::string getTagStatic() {
        return "CreditsScene";
    }
    GameState getSystemState() const {
        return GameState::Menu;
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

#endif // FILE_CREDITS_SCENE_HPP