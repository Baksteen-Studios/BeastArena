#include "scenes/credits_scene.hpp"

#include "scenes/data/menu/button.hpp"

CreditsScene::CreditsScene(EntityFactory& factory, BrickEngine& engine, GameController& game_controller) : BeastScene(factory, engine, WIDTH, HEIGHT), game_controller(game_controller) { }

void CreditsScene::start() {
    // Background
    factory.createImage("colors/white.png", this->screen_width / 2, this->screen_height / 2, this->screen_width, this->screen_height, Layers::Background, 255);

    // Back button
    Button back_button = Button();
    back_button.alpha = 255;
    back_button.on_click = [gm = &game_controller]() {
        gm->loadMainMenu();
    };
    back_button.text.color = { 255, 255, 255, 255 };
    back_button.text.font_size = 72;
    back_button.text.text = "Back";
    back_button.text.x = 100;
    back_button.text.x_scale = 150;
    back_button.text.y = 95;
    back_button.text.y_scale = 100;
    back_button.texture_path = "menu/button.png";
    back_button.x = 100;
    back_button.x_scale = 150;
    back_button.y = 100;
    back_button.y_scale = 100;
    factory.createButton(back_button, getRelativeModifier());

    // BeastArena logo
    factory.createImage("menu/logo.png", 960 / getRelativeModifier(), 140 / getRelativeModifier(), 680 / getRelativeModifier(), 106 / getRelativeModifier(), Layers::Foreground, 255);

    // Names
    factory.createText("Mark van der Meer - SCRUM Master", 960 / getRelativeModifier(), 350 / getRelativeModifier(), 800 / getRelativeModifier(), 120 / getRelativeModifier(), 120, { 0, 0, 0, 255 });
    factory.createText("Bram-Boris Meerlo - Version control", 960 / getRelativeModifier(), 500 / getRelativeModifier(), 950 / getRelativeModifier(), 120 / getRelativeModifier(), 120, { 0, 0, 0, 255 });
    factory.createText("Peter-Jan Gootzen - Software architect", 960 / getRelativeModifier(), 650 / getRelativeModifier(), 1000 / getRelativeModifier(), 120 / getRelativeModifier(), 120, { 0, 0, 0, 255 });
    factory.createText("Jan Schollaert - Game designer", 960 / getRelativeModifier(), 800 / getRelativeModifier(), 800 / getRelativeModifier(), 120 / getRelativeModifier(), 120, { 0, 0, 0, 255 });
    factory.createText("Luuk Santegoeds - Product owner", 960 / getRelativeModifier(), 950 / getRelativeModifier(), 820 / getRelativeModifier(), 120 / getRelativeModifier(), 120, { 0, 0, 0, 255 });

    engine.toggleCursor(true);

    if(!engine.getSoundManager().isPlaying()) {
        engine.getSoundManager().playMusic("music/main.mp3");
    }
}

void CreditsScene::leave() {

}

void CreditsScene::performPrepare() {

}