#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <vector>
using namespace std::chrono_literals;

#include "controllers/game_controller.hpp"
#include "brickengine/engine.hpp"
#include "brickengine/rendering/renderer.hpp"
#include "brickengine/rendering/renderables/renderable.hpp"
#include "brickengine/rendering/renderable_factory.hpp"
#include "brickengine/input.hpp"
#include "brickengine/systems/rendering_system.hpp"
#include "brickengine/systems/physics_system.hpp"
#include "entities/layers.hpp"
#include "systems/movement_system.hpp"
#include "player_input.hpp"
#include "brickengine/input_keycode.hpp"
#include "brickengine/json_parser/json.hpp"
#include "level/level.hpp"
#include "exceptions/sizeMismatchException.hpp"
#include "exceptions/notEnoughPlayerSpawnsException.hpp"
#include "level/player_spawn.hpp"
#include "level/gadget_spawn.hpp"
#include "level/solid.hpp"

GameController::GameController() {
    this->delta_time = 1;
    this->fps_cap = 144;
    // From layers.hpp
    this->layers = { 0, 1, 2, 3, 4 };

    engine = std::make_unique<BrickEngine>("Beast Arena", SCREEN_WIDTH, SCREEN_HEIGHT, layers, fps_cap);
    engine->start();
    entityManager = std::make_shared<EntityManager>();
    entityFactory = std::make_shared<EntityFactory>(entityManager, *engine->getRenderableFactory());
    collisionDetector = std::make_shared<CollisionDetector>(entityManager);
    scene_manager = std::make_shared<SceneManager>(entityFactory, entityManager, engine.get());

    createSystems();
    setupInput();
    createTestEntities();
}

void GameController::createSystems() {
    systems = std::vector<std::unique_ptr<System>>();
    systems.push_back(std::make_unique<MovementSystem>(collisionDetector, entityManager, entityFactory));
    systems.push_back(std::make_unique<PhysicsSystem>(collisionDetector, entityManager));
    systems.push_back(std::make_unique<RenderingSystem>(entityManager, *engine->getRenderer()));
}

void GameController::createTestEntities() {
    entityFactory->createPanda(0, 0, 1);
    entityFactory->createGorilla(0, 0, 2);

    Json level1_json = Json("assets/levels/level1.json", true);
    auto level = loadLevel(level1_json);
    scene_manager->loadLevel(level.get());
}

const std::unique_ptr<Level> GameController::loadLevel(Json json) const {
    std::unique_ptr<Level> level = std::unique_ptr<Level>(new Level());
    level->version = json.getDouble("version");
    level->name = json.getString("name");
    level->description = json.getString("description");

    double relative_modifier = json.getInt("width") / (double)SCREEN_WIDTH;
    if(!json.getInt("height") / SCREEN_HEIGHT == relative_modifier) {
        throw SizeMismatchException();
    }
    level->relative_modifier = relative_modifier;

    level->bg_path = json.getString("bg_path");
    level->bg_music = json.getString("bg_music");

    // Create player spawns
    if(json.getVector("player_spawns").size() < 4) {
        throw NotEnoughPlayerSpawnsException();
    }

    for(Json player_spawn_json : json.getVector("player_spawns")) {
        PlayerSpawn player_spawn = PlayerSpawn();

        player_spawn.x = player_spawn_json.getInt("x");
        player_spawn.y = player_spawn_json.getInt("y");

        level->player_spawns.push_back(player_spawn);
    }

    // Create weapon and item spawns
    for(Json gadget_spawn_json : json.getVector("gadget_spawns")) {
        GadgetSpawn gadget_spawn = GadgetSpawn();

        if(gadget_spawn_json.getString("type") == "weapon") {
            gadget_spawn.gadget_spawn_type = GadgetSpawnType::WEAPON;
        } else if(gadget_spawn_json.getString("type") == "item") {
            gadget_spawn.gadget_spawn_type = GadgetSpawnType::ITEM;
        }

        gadget_spawn.available_spawns = gadget_spawn_json.getStringVector("available_spawns");
        gadget_spawn.respawn_timer = gadget_spawn_json.getInt("respawn_timer");
        gadget_spawn.x = gadget_spawn_json.getInt("x");
        gadget_spawn.y = gadget_spawn_json.getInt("y");

        level->gadget_spawns.push_back(gadget_spawn);
    }

    // Create platforms
    for(Json solid_json : json.getVector("solids")) {
        Solid solid = Solid();

        if(solid_json.getString("type") == "rectangle") {
            solid.type = SolidType::RECTANGLE;
        }

        solid.x = solid_json.getInt("x");
        solid.y = solid_json.getInt("y");
        solid.xScale = solid_json.getInt("xScale");
        solid.yScale = solid_json.getInt("yScale");

        level->solids.push_back(solid);
    }

    return level;
}

void GameController::setupInput() {
    BrickInput<PlayerInput>& input = BrickInput<PlayerInput>::getInstance();
    std::unordered_map<int, std::unordered_map<InputKeyCode, PlayerInput>> inputMapping;
    // Player 1
    inputMapping[1][InputKeyCode::EKey_w] = PlayerInput::UP;
    inputMapping[1][InputKeyCode::EKey_a] = PlayerInput::LEFT;
    inputMapping[1][InputKeyCode::EKey_s] = PlayerInput::DOWN;
    inputMapping[1][InputKeyCode::EKey_d] = PlayerInput::RIGHT;
    // Player 2
    inputMapping[2][InputKeyCode::EKey_up] = PlayerInput::UP;
    inputMapping[2][InputKeyCode::EKey_left] = PlayerInput::LEFT;
    inputMapping[2][InputKeyCode::EKey_down] = PlayerInput::DOWN;
    inputMapping[2][InputKeyCode::EKey_right] = PlayerInput::RIGHT;

    input.setInputMapping(inputMapping);
}

void GameController::gameLoop() {
    double delta_time_count = 0;
    while(true) {
        auto start_time = std::chrono::high_resolution_clock::now();

        BrickInput<PlayerInput>::getInstance().processInput();

        engine->getRenderer()->clearScreen();

        for (auto& system : systems) {
            system->update(delta_time);
        }

        engine->drawFpsCounter();
        engine->getRenderer()->drawScreen();

        auto end_time = std::chrono::high_resolution_clock::now();
        engine->delay(start_time, end_time);
        delta_time = engine->getDeltatime();
        delta_time_count += delta_time;
    }
    engine->stop();
}
