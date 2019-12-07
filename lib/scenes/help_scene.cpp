#include "scenes/help_scene.hpp"

#include "entities/entity_factory.hpp"
#include "scenes/data/menu/image.hpp"

HelpScene::HelpScene(EntityFactory& factory, BrickEngine& engine, GameController& game_controller) : Menu(factory, engine, WIDTH, HEIGHT), game_controller(game_controller) { }

void HelpScene::performPrepare() {
    entity_components = std::make_unique<std::vector<EntityComponents>>();

    // Images
    Image keyboard = Image();
    keyboard.texture_path = "menu/keyboard.png";
    keyboard.alpha = 255;
    keyboard.x = 1320;
    keyboard.y = 180;
    keyboard.x_scale = 1160;
    keyboard.y_scale = 338;
    this->images.push_back(keyboard);

    Image controller = Image();
    controller.texture_path = "menu/controller.png";
    controller.alpha = 255;
    controller.x = 1560;
    controller.y = 600;
    controller.x_scale = 552;
    controller.y_scale = 386;
    this->images.push_back(controller);

    // Buttons
    {
        auto on_click = [gm = &game_controller]() {
            gm->loadMainMenu();
        };
        auto comps_list = factory.createButton("Back", { 255, 255, 255, 255}, 72, "menu/button.png", 100, 95, 150, 100, 255, getRelativeModifier(), on_click);
        for(auto& comps : comps_list) {
            entity_components->push_back(std::move(comps));
        }
    }

    // Text
    entity_components->push_back(factory.createText("Keyboard colors:", { 0, 0, 0, 255 }, 72, 200, 200, 300, 75, getRelativeModifier()));
    entity_components->push_back(factory.createText("Black = player 1", { 0, 0, 0, 255 }, 72, 200, 300, 300, 75, getRelativeModifier()));
    entity_components->push_back(factory.createText("Red = player 2", { 0, 0, 0, 255 }, 72, 200, 400, 300, 75, getRelativeModifier()));
    entity_components->push_back(factory.createText("Blue = player 3", { 0, 0, 0, 255 }, 72, 200, 500, 300, 75, getRelativeModifier()));
    entity_components->push_back(factory.createText("Green = player 4", { 0, 0, 0, 255 }, 72, 200, 600, 300, 75, getRelativeModifier()));
    entity_components->push_back(factory.createText("G = Grab", { 0, 0, 0, 255 }, 72, 200, 720, 300, 75, getRelativeModifier()));
    entity_components->push_back(factory.createText("S = Shoot", { 0, 0, 0, 255 }, 72, 200, 800, 300, 75, getRelativeModifier()));
    entity_components->push_back(factory.createText("Controllers get assigned to", { 0, 0, 0, 255 }, 72, 800, 450, 700, 150, getRelativeModifier()));
    entity_components->push_back(factory.createText("players in the same order", { 0, 0, 0, 255 }, 72, 800, 600, 700, 150, getRelativeModifier()));
    entity_components->push_back(factory.createText("they are connected to the pc", { 0, 0, 0, 255 }, 72, 800, 750, 700, 150, getRelativeModifier()));
    entity_components->push_back(factory.createText("How to play?", { 0, 0, 0, 255 }, 72, 200, 950, 300, 75, getRelativeModifier()));
    entity_components->push_back(factory.createText("Try to be the last standing player by killing the other players", { 0, 0, 0, 255 }, 72, 850, 1020, 1600, 75, getRelativeModifier()));
}

void HelpScene::start() {
    // Create the background
    auto comps = factory.createImage("colors/white.png", this->width / 2, this->height / 2, this->width, this->height, getRelativeModifier(), Layers::Background, 255);
    factory.addToEntityManager(std::move(comps));

    // Load the images
    for(Image image : images) {
        auto comps = factory.createImage(image.texture_path, image.x, image.y, image.x_scale, image.y_scale, getRelativeModifier(), Layers::Middleground, image.alpha);
        factory.addToEntityManager(std::move(comps));
    }

    engine.toggleCursor(true);

    if(!engine.getSoundManager().isPlaying()) {
        engine.getSoundManager().playMusic("music/main.mp3");
    }
}

void HelpScene::leave() {

}