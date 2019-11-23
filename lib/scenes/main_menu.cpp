#include "scenes/main_menu.hpp"

#include <string>
#include <functional>

#include "entities/entity_factory.hpp"
#include "brickengine/scenes/scene_manager.hpp"
#include "brickengine/json/json.hpp"
#include "scenes/exceptions/size_mismatch_exception.hpp"
#include "menu/button.hpp"
#include "menu/image.hpp"
#include "brickengine/rendering/renderables/data/color.hpp"
#include "brickengine/json/json.hpp"
#include "controllers/game_controller.hpp"

MainMenu::MainMenu(EntityFactory& factory, BrickEngine& engine, GameController& game_controller)
    : Menu(factory, engine, WIDTH, HEIGHT), game_controller(game_controller) { }

void MainMenu::performPrepare() {
    // General information
    this->bg_path = "colors/white.png";
    this->bg_music = "music/rainforest.mp3";

    // Buttons
    Button start_game_button = Button();
    start_game_button.texture_path = "menu/button.png";
    start_game_button.alpha = 255;
    start_game_button.x = 960;
    start_game_button.y = 340;
    start_game_button.x_scale = 400;
    start_game_button.y_scale = 100;
    start_game_button.text.text = "Play Game!";
    start_game_button.text.font_size = 72;
    start_game_button.text.color = { 255, 255, 255, 255 };
    start_game_button.text.x = 960;
    start_game_button.text.y = 335;
    start_game_button.text.x_scale = 400;
    start_game_button.text.y_scale = 100;
    start_game_button.on_click = [gm = &game_controller]() {
        gm->startGame();
    };
    this->buttons.push_back(start_game_button);

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
    engine.toggleCursor(true);

    // Create the background
    factory.createImage(this->bg_path, this->screen_width / 2, this->screen_height / 2, this->screen_width, this->screen_height, Layers::Background, 255);

    // Load the buttons
    for (Button button : this->buttons) {
        auto ids = factory.createButton(button, getRelativeModifier());
    }

    // Load the images
    for(Image image : images) {
        factory.createImage(image.texture_path, image.x / getRelativeModifier(), image.y / getRelativeModifier(), image.x_scale / getRelativeModifier(), image.y_scale / getRelativeModifier(), Layers::Middleground, image.alpha);
    }
}
void MainMenu::leave() {}
