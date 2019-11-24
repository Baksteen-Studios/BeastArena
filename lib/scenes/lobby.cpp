#include "scenes/lobby.hpp"

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

Lobby::Lobby(EntityFactory& factory, BrickEngine& engine, GameController& game_controller)
    : Menu(factory, engine, WIDTH, HEIGHT), game_controller(game_controller) { }

void Lobby::performPrepare() {
    // General information
    this->bg_path = "colors/white.png";
    this->bg_music = "music/main.mp3";

    // Buttons
    Button back_button = Button();
    back_button.texture_path = "menu/button.png";
    back_button.alpha = 255;
    back_button.x = 100;
    back_button.y = 100;
    back_button.x_scale = 150;
    back_button.y_scale = 100;
    back_button.text.text = "Back";
    back_button.text.font_size = 72;
    back_button.text.color = { 255, 255, 255, 255 };
    back_button.text.x = 100;
    back_button.text.y = 95;
    back_button.text.x_scale = 150;
    back_button.text.y_scale = 100;
    back_button.on_click = [gm = &game_controller]() {
        gm->loadMainMenu();
    };
    this->buttons.push_back(back_button);

    // Images
    Image logo = Image();
    logo.texture_path = "menu/logo.png";
    logo.alpha = 255;
    logo.x = 960;
    logo.y = 106;
    logo.x_scale = 680;
    logo.y_scale = 106;
    this->images.push_back(logo);

    Image player1_selector = Image();
    player1_selector.texture_path = "menu/frame.png";
    player1_selector.alpha = 255;
    player1_selector.x = 270;
    player1_selector.y = 400;
    player1_selector.x_scale = 380;
    player1_selector.y_scale = 380;
    this->images.push_back(player1_selector);

    Image player1_left_arrow = Image();
    player1_left_arrow.texture_path = "arrows/left-arrow.png";
    player1_left_arrow.alpha = 255;
    player1_left_arrow.x = 125;
    player1_left_arrow.y = 380;
    player1_left_arrow.x_scale = 40;
    player1_left_arrow.y_scale = 60;
    this->images.push_back(player1_left_arrow);

    Image player1_right_arrow = Image();
    player1_right_arrow.texture_path = "arrows/right-arrow.png";
    player1_right_arrow.alpha = 255;
    player1_right_arrow.x = 415;
    player1_right_arrow.y = 380;
    player1_right_arrow.x_scale = 40;
    player1_right_arrow.y_scale = 60;
    this->images.push_back(player1_right_arrow);

    Image player2_selector = Image();
    player2_selector.texture_path = "menu/frame.png";
    player2_selector.alpha = 255;
    player2_selector.x = 730;
    player2_selector.y = 400;
    player2_selector.x_scale = 380;
    player2_selector.y_scale = 380;
    this->images.push_back(player2_selector);

    Image player2_left_arrow = Image();
    player2_left_arrow.texture_path = "arrows/left-arrow.png";
    player2_left_arrow.alpha = 255;
    player2_left_arrow.x = 585;
    player2_left_arrow.y = 380;
    player2_left_arrow.x_scale = 40;
    player2_left_arrow.y_scale = 60;
    this->images.push_back(player2_left_arrow);

    Image player2_right_arrow = Image();
    player2_right_arrow.texture_path = "arrows/right-arrow.png";
    player2_right_arrow.alpha = 255;
    player2_right_arrow.x = 875;
    player2_right_arrow.y = 380;
    player2_right_arrow.x_scale = 40;
    player2_right_arrow.y_scale = 60;
    this->images.push_back(player2_right_arrow);

    Image player3_selector = Image();
    player3_selector.texture_path = "menu/frame.png";
    player3_selector.alpha = 255;
    player3_selector.x = 1190;
    player3_selector.y = 400;
    player3_selector.x_scale = 380;
    player3_selector.y_scale = 380;
    this->images.push_back(player3_selector);

    Image player3_left_arrow = Image();
    player3_left_arrow.texture_path = "arrows/left-arrow.png";
    player3_left_arrow.alpha = 255;
    player3_left_arrow.x = 1045;
    player3_left_arrow.y = 380;
    player3_left_arrow.x_scale = 40;
    player3_left_arrow.y_scale = 60;
    this->images.push_back(player3_left_arrow);

    Image player3_right_arrow = Image();
    player3_right_arrow.texture_path = "arrows/right-arrow.png";
    player3_right_arrow.alpha = 255;
    player3_right_arrow.x = 1335;
    player3_right_arrow.y = 380;
    player3_right_arrow.x_scale = 40;
    player3_right_arrow.y_scale = 60;
    this->images.push_back(player3_right_arrow);

    Image player4_selector = Image();
    player4_selector.texture_path = "menu/frame.png";
    player4_selector.alpha = 255;
    player4_selector.x = 1650;
    player4_selector.y = 400;
    player4_selector.x_scale = 380;
    player4_selector.y_scale = 380;
    this->images.push_back(player4_selector);

    Image player4_left_arrow = Image();
    player4_left_arrow.texture_path = "arrows/left-arrow.png";
    player4_left_arrow.alpha = 255;
    player4_left_arrow.x = 1505;
    player4_left_arrow.y = 380;
    player4_left_arrow.x_scale = 40;
    player4_left_arrow.y_scale = 60;
    this->images.push_back(player4_left_arrow);

    Image player4_right_arrow = Image();
    player4_right_arrow.texture_path = "arrows/right-arrow.png";
    player4_right_arrow.alpha = 255;
    player4_right_arrow.x = 1795;
    player4_right_arrow.y = 380;
    player4_right_arrow.x_scale = 40;
    player4_right_arrow.y_scale = 60;
    this->images.push_back(player4_right_arrow);
}

void Lobby::start() {
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

    engine.toggleCursor(true);
    engine.getSoundManager().playMusic(this->bg_music);
}
void Lobby::leave() {}
