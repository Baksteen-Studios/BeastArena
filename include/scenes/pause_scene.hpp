#ifndef FILE_PAUSE_SCENE_HPP
#define FILE_PAUSE_SCENE_HPP

#include <memory>

#include "scenes/menu.hpp"
#include "entities/entity_factory.hpp"
#include "enums/game_state.hpp"

class PauseScene : public Menu<PauseScene> {
public:
    PauseScene(EntityFactory& factory, BrickEngine& engine);
    static std::string getTagStatic() {
        return "PauseScene";
    };
    GameState getSystemState() const{
        return GameState::Paused;
    };
    static SceneLayer getLayerStatic() {
        return SceneLayer::Secondary;
    }

    void start();
    void leave();
protected:
    void performPrepare();
private:
    static const int WIDTH = 1920;
    static const int HEIGHT = 1080;
};
#endif // FILE_PAUSE_SCENE_HPP
