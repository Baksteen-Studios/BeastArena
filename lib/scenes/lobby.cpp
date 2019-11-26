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
#include "brickengine/components/player_component.hpp"
#include "controllers/game_controller.hpp"
#include "brickengine/input.hpp"
#include "player_input.hpp"
#include "components/character_selection_component.hpp"
#include "brickengine/std/random.hpp"

Lobby::Lobby(EntityFactory& factory, BrickEngine& engine, GameController& game_controller)
    : BeastScene(factory, engine, WIDTH, HEIGHT), game_controller(game_controller) { }

void Lobby::performPrepare() {
    // General information
    this->bg_path = "backgrounds/pixel-forest.png";
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

    Button start_game_button = Button();
    start_game_button.texture_path = "menu/button.png";
    start_game_button.alpha = 255;
    start_game_button.x = 960;
    start_game_button.y = 700;
    start_game_button.x_scale = 350;
    start_game_button.y_scale = 150;
    start_game_button.text.text = "Start Game!";
    start_game_button.text.font_size = 72;
    start_game_button.text.color = { 255, 255, 255, 255 };
    start_game_button.text.x = 960;
    start_game_button.text.y = 695;
    start_game_button.text.x_scale = 450;
    start_game_button.text.y_scale = 150;
    start_game_button.on_click = [gm = &game_controller]() {
        gm->startGame();
    };
    this->buttons.push_back(start_game_button);

    // Images
    Image logo = Image();
    logo.texture_path = "menu/logo.png";
    logo.alpha = 255;
    logo.x = 960;
    logo.y = 106;
    logo.x_scale = 680;
    logo.y_scale = 106;
    logo.layer = Layers::Middleground;
    this->images.push_back(logo);

    Image player1_selector = Image();
    player1_selector.texture_path = "menu/frame.png";
    player1_selector.alpha = 255;
    player1_selector.x = 270;
    player1_selector.y = 400;
    player1_selector.x_scale = 380;
    player1_selector.y_scale = 380;
    player1_selector.layer = Layers::Middleground;
    this->images.push_back(player1_selector);

    Image player1_selector_background = Image();
    player1_selector_background.texture_path = "colors/white.png";
    player1_selector_background.alpha = 90;
    player1_selector_background.x = player1_selector.x;
    player1_selector_background.y = player1_selector.y - 30;
    player1_selector_background.x_scale = player1_selector.x_scale;
    player1_selector_background.y_scale = player1_selector.y_scale - 60;
    player1_selector_background.layer = Layers::Lowground;
    this->images.push_back(player1_selector_background);

    Image player1_left_arrow = Image();
    player1_left_arrow.texture_path = "arrows/left-arrow.png";
    player1_left_arrow.alpha = 255;
    player1_left_arrow.x = 125;
    player1_left_arrow.y = 380;
    player1_left_arrow.x_scale = 40;
    player1_left_arrow.y_scale = 60;
    player1_left_arrow.layer = Layers::Middleground;
    this->images.push_back(player1_left_arrow);

    Image player1_right_arrow = Image();
    player1_right_arrow.texture_path = "arrows/right-arrow.png";
    player1_right_arrow.alpha = 255;
    player1_right_arrow.x = 415;
    player1_right_arrow.y = 380;
    player1_right_arrow.x_scale = 40;
    player1_right_arrow.y_scale = 60;
    player1_right_arrow.layer = Layers::Middleground;
    this->images.push_back(player1_right_arrow);

    Image player2_selector = Image();
    player2_selector.texture_path = "menu/frame.png";
    player2_selector.alpha = 255;
    player2_selector.x = 730;
    player2_selector.y = 400;
    player2_selector.x_scale = 380;
    player2_selector.y_scale = 380;
    player2_selector.layer = Layers::Middleground;
    this->images.push_back(player2_selector);

    Image player2_selector_background = Image();
    player2_selector_background.texture_path = "colors/white.png";
    player2_selector_background.alpha = 90;
    player2_selector_background.x = player2_selector.x;
    player2_selector_background.y = player2_selector.y - 30;
    player2_selector_background.x_scale = player2_selector.x_scale;
    player2_selector_background.y_scale = player2_selector.y_scale - 60;
    player2_selector_background.layer = Layers::Lowground;
    this->images.push_back(player2_selector_background);

    Image player2_left_arrow = Image();
    player2_left_arrow.texture_path = "arrows/left-arrow.png";
    player2_left_arrow.alpha = 255;
    player2_left_arrow.x = 585;
    player2_left_arrow.y = 380;
    player2_left_arrow.x_scale = 40;
    player2_left_arrow.y_scale = 60;
    player2_left_arrow.layer = Layers::Middleground;
    this->images.push_back(player2_left_arrow);

    Image player2_right_arrow = Image();
    player2_right_arrow.texture_path = "arrows/right-arrow.png";
    player2_right_arrow.alpha = 255;
    player2_right_arrow.x = 875;
    player2_right_arrow.y = 380;
    player2_right_arrow.x_scale = 40;
    player2_right_arrow.y_scale = 60;
    player2_right_arrow.layer = Layers::Middleground;
    this->images.push_back(player2_right_arrow);

    Image player3_selector = Image();
    player3_selector.texture_path = "menu/frame.png";
    player3_selector.alpha = 255;
    player3_selector.x = 1190;
    player3_selector.y = 400;
    player3_selector.x_scale = 380;
    player3_selector.y_scale = 380;
    player3_selector.layer = Layers::Middleground;
    this->images.push_back(player3_selector);

    Image player3_selector_background = Image();
    player3_selector_background.texture_path = "colors/white.png";
    player3_selector_background.alpha = 90;
    player3_selector_background.x = player3_selector.x;
    player3_selector_background.y = player3_selector.y - 30;
    player3_selector_background.x_scale = player3_selector.x_scale;
    player3_selector_background.y_scale = player3_selector.y_scale - 60;
    player3_selector_background.layer = Layers::Lowground;
    this->images.push_back(player3_selector_background);

    Image player3_left_arrow = Image();
    player3_left_arrow.texture_path = "arrows/left-arrow.png";
    player3_left_arrow.alpha = 255;
    player3_left_arrow.x = 1045;
    player3_left_arrow.y = 380;
    player3_left_arrow.x_scale = 40;
    player3_left_arrow.y_scale = 60;
    player3_left_arrow.layer = Layers::Middleground;
    this->images.push_back(player3_left_arrow);

    Image player3_right_arrow = Image();
    player3_right_arrow.texture_path = "arrows/right-arrow.png";
    player3_right_arrow.alpha = 255;
    player3_right_arrow.x = 1335;
    player3_right_arrow.y = 380;
    player3_right_arrow.x_scale = 40;
    player3_right_arrow.y_scale = 60;
    player3_right_arrow.layer = Layers::Middleground;
    this->images.push_back(player3_right_arrow);

    Image player4_selector = Image();
    player4_selector.texture_path = "menu/frame.png";
    player4_selector.alpha = 255;
    player4_selector.x = 1650;
    player4_selector.y = 400;
    player4_selector.x_scale = 380;
    player4_selector.y_scale = 380;
    player4_selector.layer = Layers::Middleground;
    this->images.push_back(player4_selector);

    Image player4_selector_background = Image();
    player4_selector_background.texture_path = "colors/white.png";
    player4_selector_background.alpha = 90;
    player4_selector_background.x = player4_selector.x;
    player4_selector_background.y = player4_selector.y - 30;
    player4_selector_background.x_scale = player4_selector.x_scale;
    player4_selector_background.y_scale = player4_selector.y_scale - 60;
    player4_selector_background.layer = Layers::Lowground;
    this->images.push_back(player4_selector_background);

    Image player4_left_arrow = Image();
    player4_left_arrow.texture_path = "arrows/left-arrow.png";
    player4_left_arrow.alpha = 255;
    player4_left_arrow.x = 1505;
    player4_left_arrow.y = 380;
    player4_left_arrow.x_scale = 40;
    player4_left_arrow.y_scale = 60;
    player4_left_arrow.layer = Layers::Middleground;
    this->images.push_back(player4_left_arrow);

    Image player4_right_arrow = Image();
    player4_right_arrow.texture_path = "arrows/right-arrow.png";
    player4_right_arrow.alpha = 255;
    player4_right_arrow.x = 1795;
    player4_right_arrow.y = 380;
    player4_right_arrow.x_scale = 40;
    player4_right_arrow.y_scale = 60;
    player4_right_arrow.layer = Layers::Middleground;
    this->images.push_back(player4_right_arrow);

    // Solids
    Solid bottom = Solid();
    bottom.x = 960;
    bottom.y = 1085;
    bottom.xScale = 1920;
    bottom.yScale = 10;
    bottom.texture_path = "colors/black.jpg";
    bottom.alpha = 255;
    this->solids.push_back(bottom);

    Solid left = Solid();
    left.x = -5;
    left.y = 540;
    left.xScale = 10;
    left.yScale = 1080;
    left.texture_path = "colors/black.jpg";
    left.alpha = 255;
    this->solids.push_back(left);

    Solid right = Solid();
    right.x = 1925;
    right.y = 540;
    right.xScale = 10;
    right.yScale = 1080;
    right.texture_path = "colors/black.jpg";
    right.alpha = 255;
    this->solids.push_back(right);

    Solid behind_button = Solid();
    behind_button.x = start_game_button.x;
    behind_button.y = start_game_button.y;
    behind_button.xScale = start_game_button.x_scale;
    behind_button.yScale = start_game_button.y_scale;
    behind_button.texture_path = "colors/black.jpg";
    behind_button.alpha = 0;
    this->solids.push_back(behind_button);

    // Critters
    this->critters.push_back(std::pair<int, int>(600, 800));
    this->critters.push_back(std::pair<int, int>(700, 800));
    this->critters.push_back(std::pair<int, int>(800, 800));

    // Weapons --> DIRECT FACTORY CALLS IN start()

    // Character selection components
    this->character_selection_components.push_back(std::pair<int, std::pair<int, int>>(1, std::pair<int, int>(player1_selector.x, player1_selector.y)));
    this->character_selection_components.push_back(std::pair<int, std::pair<int, int>>(2, std::pair<int, int>(player2_selector.x, player2_selector.y)));
    this->character_selection_components.push_back(std::pair<int, std::pair<int, int>>(3, std::pair<int, int>(player3_selector.x, player3_selector.y)));
    this->character_selection_components.push_back(std::pair<int, std::pair<int, int>>(4, std::pair<int, int>(player4_selector.x, player4_selector.y)));
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
        factory.createImage(image.texture_path, image.x / getRelativeModifier(), image.y / getRelativeModifier(), image.x_scale / getRelativeModifier(), image.y_scale / getRelativeModifier(), image.layer, image.alpha);
    }

    // Load solids
    for(Solid solid : solids) {
        factory.createPlatform(solid.x / getRelativeModifier(), solid.y / getRelativeModifier(), solid.xScale / getRelativeModifier(), solid.yScale / getRelativeModifier(), solid.texture_path, solid.alpha);
    }

    // Load critters
    for(auto critter : critters) {
        factory.createCritter(critter.first / getRelativeModifier(), critter.second / getRelativeModifier());
    }

    // Load weapons
    factory.createPistol(100, 500, 10);

    // Load character selection components
    for(auto selector : character_selection_components) {
        factory.createCharacterSelector(selector.first, selector.second.first / getRelativeModifier(), selector.second.second / getRelativeModifier());
    }

    engine.toggleCursor(true);

    if(!engine.getSoundManager().isPlaying()) {
        engine.getSoundManager().playMusic(this->bg_music);
    }


    auto& input = BrickInput<PlayerInput>::getInstance();
    input.setTimeToWait(1, PlayerInput::X_AXIS, 0.1);
    input.setTimeToWait(2, PlayerInput::X_AXIS, 0.1);
    input.setTimeToWait(3, PlayerInput::X_AXIS, 0.1);
    input.setTimeToWait(4, PlayerInput::X_AXIS, 0.1);
}

void Lobby::leave() {
    auto& em = factory.getEntityManager();
    auto character_selection_entities = em.getEntitiesByComponent<CharacterSelectionComponent>();

    std::vector<int> need_character_components;
    for (auto& [ entity_id, character_selection ] : character_selection_entities ) {
        if(character_selection->joined && character_selection->picked) {
            if(character_selection->selected_character != Character::RANDOM) {
                available_characters.erase(std::remove(available_characters.begin(), available_characters.end(), character_selection->selected_character), available_characters.end());
            } else {
                need_character_components.push_back(entity_id);
            }
        }
    }

    for(int entity_id : need_character_components) {
        auto& random = Random::getInstance();
        auto random_index = random.getRandomInt(0, available_characters.size() - 1);

        auto character_selection_component = em.getComponent<CharacterSelectionComponent>(entity_id);
        factory.createPlayer(character_selection_component->player_id, available_characters.at(random_index), 500, 500);

        available_characters.erase(available_characters.begin() + random_index);

        em.removeEntity(character_selection_component->player_entity_id);
    }

    auto player_entities = em.getEntitiesByComponent<PlayerComponent>();

    for (auto& [ entity_id, player ] : player_entities ) {
        em.removeTag(entity_id, this->getTag());
    }
}