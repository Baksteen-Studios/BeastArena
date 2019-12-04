#include "scenes/main_menu.hpp"

#include <string>
#include <functional>

#include "entities/entity_factory.hpp"
#include "brickengine/scenes/scene_manager.hpp"
#include "brickengine/json/json.hpp"
#include "scenes/exceptions/size_mismatch_exception.hpp"
#include "scenes/data/menu/button.hpp"
#include "scenes/data/menu/image.hpp"
#include "brickengine/rendering/renderables/data/color.hpp"
#include "brickengine/json/json.hpp"
#include "controllers/game_controller.hpp"

MainMenu::MainMenu(EntityFactory& factory, BrickEngine& engine, GameController& game_controller)
    : Menu(factory, engine, WIDTH, HEIGHT), game_controller(game_controller) { }

void MainMenu::performPrepare() {
    entity_components = std::make_unique<std::vector<std::unique_ptr<std::vector<std::unique_ptr<Component>>>>>();

    // General information
    this->bg_path = "colors/white.png";
    this->bg_music = "music/main.mp3";
   {
        auto on_click = [gm = &game_controller]() {
            gm->showHighscores();
        };
        auto comps_list = factory.createButton("Highscores", { 255, 255, 255, 255}, 70, "menu/button.png", 960, 490, 400, 100, 255, getRelativeModifier(), on_click);
        for(auto& comps : comps_list) {
            entity_components->push_back(std::move(comps));
        }
    }
    {
        auto on_click = [gm = &game_controller]() {
            gm->startGame();
        };
        auto comps_list = factory.createButton("Play Game!", { 255, 255, 255, 255}, 70, "menu/button.png", 960, 340, 400, 100, 255, getRelativeModifier(), on_click);
        for(auto& comps : comps_list) {
            entity_components->push_back(std::move(comps));
        }
    }
 

    // Images
    Image logo = Image();
    logo.texture_path = "menu/logo.png";
    logo.alpha = 255;
    logo.x = 960;
    logo.y = 210;
    logo.x_scale = 680;
    logo.y_scale = 106;
    this->images.push_back(logo);
}

void MainMenu::start() {
    // Create the background
    auto comps = factory.createImage(this->bg_path, this->screen_width / 2, this->screen_height / 2, this->screen_width, this->screen_height, 1, Layers::Background, 255);
    factory.addToEntityManager(std::move(comps));

    // Load the images
    for(Image image : images) {
        auto comps = factory.createImage(image.texture_path, image.x, image.y, image.x_scale, image.y_scale, getRelativeModifier(), Layers::Middleground, image.alpha);
        factory.addToEntityManager(std::move(comps));
    }

    engine.toggleCursor(true);
    engine.getSoundManager().playMusic(this->bg_music);
}
void MainMenu::leave() {}
