#include "controllers/game_controller.hpp"
#include "menu/main_menu.hpp"

int main() {
    auto gc = GameController();

    MainMenu main_menu = MainMenu(gc.getScreenWidth(), gc.getScreenWidth(), &gc);
    gc.getSceneManager().loadMenu(main_menu);

    gc.gameLoop();
    return 0;
}
