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
#include "systems/click_system.hpp"
#include "entities/layers.hpp"
#include "systems/movement_system.hpp"
#include "player_input.hpp"
#include "brickengine/input_keycode.hpp"
#include "brickengine/json/json.hpp"
#include "level/level.hpp"
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
    systems.push_back(std::make_unique<ClickSystem>(entityManager));
    systems.push_back(std::make_unique<MovementSystem>(collisionDetector, entityManager, entityFactory));
    systems.push_back(std::make_unique<PhysicsSystem>(collisionDetector, entityManager));
    systems.push_back(std::make_unique<RenderingSystem>(entityManager, *engine->getRenderer()));
}

void GameController::createTestEntities() {
    entityFactory->createPanda(0, 0, 1);
    entityFactory->createGorilla(0, 0, 2);

    Json level_json = Json("assets/levels/level2.json", true);
    auto level = std::unique_ptr<Level>(new Level(level_json, SCREEN_WIDTH, SCREEN_HEIGHT));
    scene_manager->loadLevel(level.get());
}

void GameController::setupInput() {
    BrickInput<PlayerInput>& input = BrickInput<PlayerInput>::getInstance();
    std::unordered_map<int, std::unordered_map<InputKeyCode, PlayerInput>> inputMapping;
    // Player 1
    inputMapping[1][InputKeyCode::EKey_w] = PlayerInput::UP;
    inputMapping[1][InputKeyCode::EKey_a] = PlayerInput::LEFT;
    inputMapping[1][InputKeyCode::EKey_s] = PlayerInput::DOWN;
    inputMapping[1][InputKeyCode::EKey_d] = PlayerInput::RIGHT;
    inputMapping[1][InputKeyCode::EKey_mouse_left] = PlayerInput::MOUSE_LEFT;
    inputMapping[1][InputKeyCode::EKey_mouse_right] = PlayerInput::MOUSE_RIGHT;
    // Player 2
    inputMapping[2][InputKeyCode::EKey_up] = PlayerInput::UP;
    inputMapping[2][InputKeyCode::EKey_left] = PlayerInput::LEFT;
    inputMapping[2][InputKeyCode::EKey_down] = PlayerInput::DOWN;
    inputMapping[2][InputKeyCode::EKey_right] = PlayerInput::RIGHT;

    input.setInputMapping(inputMapping);
}

void GameController::gameLoop() {
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
    }
    engine->stop();
}
