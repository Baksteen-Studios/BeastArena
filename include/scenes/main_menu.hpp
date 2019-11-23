#ifndef FILE_MAIN_MENU_HPP
#define FILE_MAIN_MENU_HPP

#include <string>
#include <vector>

#include "scenes/menu.hpp"
#include "entities/entity_factory.hpp"
#include "brickengine/scenes/scene_manager.hpp"
#include "controllers/game_controller.hpp"

class MainMenu : public Menu<MainMenu> {
public:
    MainMenu(EntityFactory& factory, int width, int height, GameController* game_controller);

    static std::string getTagStatic() {
        return "MainMenu";
    }
    GameState getSystemState() {
        return GameState::MainMenu;
    }

    void prepare();
    void start();
    void leave();
private:
    static const int WIDTH = 1920;
    static const int HEIGHT = 1080;
};
#endif // FILE_MAIN_MENU_HPP