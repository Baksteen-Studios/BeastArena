#include "scenes/help_scene.hpp"

#include "scenes/data/menu/button.hpp"

HelpScene::HelpScene(EntityFactory& factory, BrickEngine& engine, GameController& game_controller) : BeastScene(factory, engine, WIDTH, HEIGHT), game_controller(game_controller) { }

void HelpScene::start() {
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
    
    // Keyboard image
    factory.createImage("menu/keyboard.png", 1320 / getRelativeModifier(), 180 / getRelativeModifier(), 1160 / getRelativeModifier(), 338 / getRelativeModifier(), Layers::Foreground, 255);

    // Controller image
    factory.createImage("menu/controller.png", 1560 / getRelativeModifier(), 600 / getRelativeModifier(), 552 / getRelativeModifier(), 386 / getRelativeModifier(), Layers::Foreground, 255);

    // Text
    factory.createText("Keyboard colors:", 200 / getRelativeModifier(), 200 / getRelativeModifier(), 300 / getRelativeModifier(), 75 / getRelativeModifier(), 72, { 0, 0, 0, 255 });
    factory.createText("Black = player 1", 200 / getRelativeModifier(), 300 / getRelativeModifier(), 300 / getRelativeModifier(), 75 / getRelativeModifier(), 72, { 0, 0, 0, 255 });
    factory.createText("Red = player 2", 200 / getRelativeModifier(), 400 / getRelativeModifier(), 300 / getRelativeModifier(), 75 / getRelativeModifier(), 72, { 0, 0, 0, 255 });
    factory.createText("Blue = player 3", 200 / getRelativeModifier(), 500 / getRelativeModifier(), 300 / getRelativeModifier(), 75 / getRelativeModifier(), 72, { 0, 0, 0, 255 });
    factory.createText("Green = player 4", 200 / getRelativeModifier(), 600 / getRelativeModifier(), 300 / getRelativeModifier(), 75 / getRelativeModifier(), 72, { 0, 0, 0, 255 });

    factory.createText("G = Grab", 200 / getRelativeModifier(), 720 / getRelativeModifier(), 300 / getRelativeModifier(), 75 / getRelativeModifier(), 72, { 0, 0, 0, 255 });
    factory.createText("S = Shoot", 200 / getRelativeModifier(), 800 / getRelativeModifier(), 300 / getRelativeModifier(), 75 / getRelativeModifier(), 72, { 0, 0, 0, 255 });

    factory.createText("Controllers get assigned to", 800 / getRelativeModifier(), 450 / getRelativeModifier(), 700 / getRelativeModifier(), 150 / getRelativeModifier(), 72, { 0, 0, 0, 255 });
    factory.createText("players in the same order", 800 / getRelativeModifier(), 600 / getRelativeModifier(), 700 / getRelativeModifier(), 150 / getRelativeModifier(), 72, { 0, 0, 0, 255 });
    factory.createText("they are connected to the pc", 800 / getRelativeModifier(), 750 / getRelativeModifier(), 700 / getRelativeModifier(), 150 / getRelativeModifier(), 72, { 0, 0, 0, 255 });

    factory.createText("How to play?", 200 / getRelativeModifier(), 950 / getRelativeModifier(), 300 / getRelativeModifier(), 75 / getRelativeModifier(), 72, { 0, 0, 0, 255 });
    factory.createText("Try to be the last standing player for 10 levels by killing the other players", 945 / getRelativeModifier(), 1020 / getRelativeModifier(), 1800 / getRelativeModifier(), 75 / getRelativeModifier(), 72, { 0, 0, 0, 255 });

    engine.toggleCursor(true);

    if(!engine.getSoundManager().isPlaying()) {
        engine.getSoundManager().playMusic("music/main.mp3");
    }
}

void HelpScene::leave() {

}

void HelpScene::performPrepare() {

}