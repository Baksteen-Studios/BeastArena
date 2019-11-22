#include "controllers/game_controller.hpp"

int main() {
    GameController gc;

    gc.loadMainMenu();

    gc.gameLoop();
    return 0;
}
