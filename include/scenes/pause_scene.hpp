#ifndef FILE_PAUSE_SCENE_HPP
#define FILE_PAUSE_SCENE_HPP

#include <string>
#include "scenes/menu.hpp"
#include "entities/entity_factory.hpp"
#include "enums/game_state.hpp"

class PauseScene : public Menu<PauseScene> {
public:
    PauseScene(EntityFactory& factory) : Menu<PauseScene>(factory, WIDTH, HEIGHT) {};
    static std::string getTagStatic() {
        return "PauseScene";
    };
    GameState getSystemState() {
        return GameState::Paused;
    };
    void prepare();
    void start();
    void leave();
private:
    static const int WIDTH;
    static const int HEIGHT;
};
#endif // FILE_PAUSE_SCENE_HPP