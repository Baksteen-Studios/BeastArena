#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <utility>
#include <vector>
#include <deque>
#include <filesystem>
#include <map>
#include <algorithm>
#include <unordered_map>
using namespace std::chrono_literals;

#include "controllers/game_controller.hpp"
#include "brickengine/engine.hpp"
#include "brickengine/rendering/renderer.hpp"
#include "brickengine/rendering/renderables/renderable.hpp"
#include "brickengine/components/colliders/rectangle_collider_component.hpp"
#include "brickengine/rendering/renderable_factory.hpp"
#include "brickengine/input.hpp"
#include "brickengine/systems/rendering_system.hpp"
#include "brickengine/systems/physics_system.hpp"
#include "brickengine/systems/displacement_system.hpp"
#include "brickengine/std/random.hpp"
#include "brickengine/input_keycode.hpp"
#include "brickengine/json/json.hpp"
#include "brickengine/scenes/enums/scene_layer.hpp"

#include "systems/pickup_system.hpp"
#include "systems/click_system.hpp"
#include "systems/weapon_system.hpp"
#include "systems/damage_system.hpp"
#include "systems/despawn_system.hpp"
#include "systems/movement_system.hpp"
#include "systems/critter_system.hpp"
#include "systems/game_system.hpp"
#include "systems/ready_up_system.hpp"
#include "systems/highscore_system.hpp"
#include "systems/lobby_system.hpp"
#include "systems/spawn_system.hpp"

#include "entities/layers.hpp"
#include "player_input.hpp"
#include "brickengine/input_keycode.hpp"
#include "brickengine/json/json.hpp"
#include "scenes/data/level/player_spawn.hpp"
#include "scenes/data/level/gadget_spawn.hpp"
#include "scenes/data/level/solid.hpp"
#include "brickengine/components/colliders/rectangle_collider_component.hpp"
#include "brickengine/std/random.hpp"
#include "systems/game_speed_system.hpp"

#include "components/wandering_component.hpp"
#include "scenes/data/level/player_spawn.hpp"
#include "scenes/data/level/gadget_spawn.hpp"
#include "scenes/data/level/solid.hpp"
#include "scenes/main_menu.hpp"
#include "scenes/lobby.hpp"
#include "scenes/help_scene.hpp"
#include "scenes/credits_scene.hpp"

#include "components/stats_component.hpp"

#include "scenes/level_scene.hpp"
#include "scenes/intermission_scene.hpp"
#include "scenes/end_scene.hpp"
#include "scenes/highscore_scene.hpp"

#include "data/score.hpp"

GameController::GameController() {
    this->should_quit = false;

    this->delta_time = 1;
#ifdef PERFORMANCE_DEBUGGING
    this->fps_cap = 9999;
#else
    this->fps_cap = 144;
#endif // PERFORMANCE_DEBUGGING

    // From layers.hpp
    this->layers = { 0, 1, 2, 3, 4 };

    this->delta_time_modifier = std::unique_ptr<double>(new double(1));

    engine = std::make_unique<BrickEngine>("Beast Arena", SCREEN_WIDTH, SCREEN_HEIGHT, layers, fps_cap);
    engine->start();
    entityManager = std::make_shared<EntityManager>();
    entityFactory = std::make_shared<EntityFactory>(entityManager, *engine->getRenderableFactory());

    std::unordered_map<std::string, std::set<std::string>> is_trigger_exceptions;
    is_trigger_exceptions.insert({ "Critter", std::set<std::string> { "Platform" } });
    is_trigger_exceptions.insert({ "Weapon", std::set<std::string> { "Platform" } });
    is_trigger_exceptions.insert({ "Player", std::set<std::string> { "Platform" } });
    is_trigger_exceptions.insert({ "Bullet", std::set<std::string> { "Player" } });
    is_trigger_exceptions.insert({ "DeadPlayer", std::set<std::string> { "Platform" } });
    is_trigger_exceptions.insert({ "Spawner", std::set<std::string> { "Platform" } });
    is_trigger_exceptions.insert({ "Ready", std::set<std::string> { "Platform" } });
    is_trigger_exceptions.insert({ "Trophy", std::set<std::string> { "Platform" } });
    collision_detector = std::make_unique<CollisionDetector2>(is_trigger_exceptions, *entityManager);

    createGameStateManager();
    scene_manager = std::make_unique<SceneManager<GameState>>(*entityManager, *game_state_manager);
    score_json = std::make_unique<ScoreJson>();
    entityManager->setGetCurrentSceneTagFunction(scene_manager->createGetPrimaryTagFunction());

    setupInput();
    loadMainMenu();
}

void GameController::createGameStateManager() {
    auto state_systems = std::make_unique<GameStateManager<GameState>::StateSystems>();
    state_systems->insert({ GameState::Menu, std::make_unique<GameStateManager<GameState>::Systems>() });
    state_systems->insert({ GameState::Lobby, std::make_unique<GameStateManager<GameState>::Systems>() });
    state_systems->insert({ GameState::InGame, std::make_unique<std::vector<std::unique_ptr<System>>>() });
    state_systems->insert({ GameState::EndGame, std::make_unique<std::vector<std::unique_ptr<System>>>() });
    state_systems->insert({ GameState::Highscore, std::make_unique<std::vector<std::unique_ptr<System>>>() });

    // Menu
    state_systems->at(GameState::Menu)->push_back(std::make_unique<ClickSystem>(entityManager));
    state_systems->at(GameState::Menu)->push_back(std::make_unique<RenderingSystem>(entityManager, *engine->getRenderer()));

    // Lobby
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<LobbySystem>(entityFactory, entityManager));
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<ClickSystem>(entityManager));
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<MovementSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<PhysicsSystem>(*collision_detector, entityManager, *delta_time_modifier.get()));
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<PickupSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<CritterSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<WeaponSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<DamageSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<DespawnSystem>(*collision_detector, entityManager, SCREEN_WIDTH, SCREEN_HEIGHT));
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<SpawnSystem>(entityManager, entityFactory));
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<DisplacementSystem>(*collision_detector, entityManager));
    state_systems->at(GameState::Lobby)->push_back(std::make_unique<RenderingSystem>(entityManager, *engine->getRenderer()));

    // In game
    state_systems->at(GameState::InGame)->push_back(std::make_unique<GameSpeedSystem>(entityManager, *delta_time_modifier.get()));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<GameSystem>(entityManager, *this));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<ClickSystem>(entityManager));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<MovementSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<PhysicsSystem>(*collision_detector, entityManager, *delta_time_modifier.get()));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<PickupSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<CritterSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<WeaponSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<DamageSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<DespawnSystem>(*collision_detector, entityManager, SCREEN_WIDTH, SCREEN_HEIGHT));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<SpawnSystem>(entityManager, entityFactory));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<DisplacementSystem>(*collision_detector, entityManager));
    state_systems->at(GameState::InGame)->push_back(std::make_unique<RenderingSystem>(entityManager, *engine->getRenderer()));

    state_systems->at(GameState::EndGame)->push_back(std::make_unique<ClickSystem>(entityManager));
    state_systems->at(GameState::EndGame)->push_back(std::make_unique<MovementSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::EndGame)->push_back(std::make_unique<PhysicsSystem>(*collision_detector, entityManager, *delta_time_modifier.get()));
    state_systems->at(GameState::EndGame)->push_back(std::make_unique<PickupSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::EndGame)->push_back(std::make_unique<CritterSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::EndGame)->push_back(std::make_unique<ReadyUpSystem>(entityManager, entityFactory, [this]() { this->loadLobby(); }));
    state_systems->at(GameState::EndGame)->push_back(std::make_unique<DamageSystem>(*collision_detector, entityManager, entityFactory));
    state_systems->at(GameState::EndGame)->push_back(std::make_unique<DespawnSystem>(*collision_detector, entityManager, SCREEN_WIDTH, SCREEN_HEIGHT));
    state_systems->at(GameState::EndGame)->push_back(std::make_unique<SpawnSystem>(entityManager, entityFactory));
    state_systems->at(GameState::EndGame)->push_back(std::make_unique<DisplacementSystem>(*collision_detector, entityManager));
    state_systems->at(GameState::EndGame)->push_back(std::make_unique<RenderingSystem>(entityManager, *engine->getRenderer()));

    // Highscores
    state_systems->at(GameState::Highscore)->push_back(std::make_unique<ClickSystem>(entityManager));
    state_systems->at(GameState::Highscore)->push_back(std::make_unique<HighscoreSystem>(entityManager, entityFactory, *score_json));
    state_systems->at(GameState::Highscore)->push_back(std::make_unique<RenderingSystem>(entityManager, *engine->getRenderer()));

    std::unordered_map<GameState, bool> reset_on_set_state;
    reset_on_set_state.insert({ GameState::InGame, true });
    reset_on_set_state.insert({ GameState::EndGame, true });
    reset_on_set_state.insert({ GameState::Highscore, true });
    reset_on_set_state.insert({ GameState::Lobby, true });
    reset_on_set_state.insert({ GameState::Menu, true });
    reset_on_set_state.insert({ GameState::Paused, false });

    GameState begin_state = GameState::Unintialized;
    game_state_manager = std::make_unique<GameStateManager<GameState>>(std::move(state_systems), reset_on_set_state, begin_state);
}

void GameController::setupInput() {
    BrickInput<PlayerInput>& input = BrickInput<PlayerInput>::getInstance();
    std::map<int, std::unordered_map<InputKeyCode, PlayerInput>> inputMapping;

    // This map is used to determine what the value of the input should be when pressed.
    std::unordered_map<InputKeyCode, signed int> axis_mapping;

    // Keyboard
    // Player 1
    inputMapping[1][InputKeyCode::EKey_w] = PlayerInput::Y_AXIS;
    inputMapping[1][InputKeyCode::EKey_a] = PlayerInput::X_AXIS;
    inputMapping[1][InputKeyCode::EKey_s] = PlayerInput::Y_AXIS;
    inputMapping[1][InputKeyCode::EKey_d] = PlayerInput::X_AXIS;
    inputMapping[1][InputKeyCode::EKey_q] = PlayerInput::GRAB;
    inputMapping[1][InputKeyCode::EKey_e] = PlayerInput::SHOOT;
    inputMapping[1][InputKeyCode::EKey_mouse_left] = PlayerInput::MOUSE_LEFT;
    inputMapping[1][InputKeyCode::EKey_mouse_right] = PlayerInput::MOUSE_RIGHT;
    inputMapping[1][InputKeyCode::EKey_pagedown] = PlayerInput::SPEED_DOWN;
    inputMapping[1][InputKeyCode::EKey_pageup] = PlayerInput::SPEED_UP;
    inputMapping[1][InputKeyCode::EKey_home] = PlayerInput::SPEED_RESET;

    axis_mapping[InputKeyCode::EKey_w] = 1;
    axis_mapping[InputKeyCode::EKey_a] = -1;
    axis_mapping[InputKeyCode::EKey_s] = -1;
    axis_mapping[InputKeyCode::EKey_d] = 1;

    // Player 2
    inputMapping[2][InputKeyCode::EKey_up] = PlayerInput::Y_AXIS;
    inputMapping[2][InputKeyCode::EKey_left] = PlayerInput::X_AXIS;
    inputMapping[2][InputKeyCode::EKey_down] = PlayerInput::Y_AXIS;
    inputMapping[2][InputKeyCode::EKey_right] = PlayerInput::X_AXIS;
    inputMapping[2][InputKeyCode::EKey_rctrl] = PlayerInput::GRAB;
    inputMapping[2][InputKeyCode::EKey_rshift] = PlayerInput::SHOOT;

    axis_mapping[InputKeyCode::EKey_up] = 1;
    axis_mapping[InputKeyCode::EKey_left] = -1;
    axis_mapping[InputKeyCode::EKey_down] = -1;
    axis_mapping[InputKeyCode::EKey_right] = 1;

    // Player 3
    inputMapping[3][InputKeyCode::EKey_t] = PlayerInput::Y_AXIS;
    inputMapping[3][InputKeyCode::EKey_f] = PlayerInput::X_AXIS;
    inputMapping[3][InputKeyCode::EKey_g] = PlayerInput::Y_AXIS;
    inputMapping[3][InputKeyCode::EKey_h] = PlayerInput::X_AXIS;
    inputMapping[3][InputKeyCode::EKey_r] = PlayerInput::GRAB;
    inputMapping[3][InputKeyCode::EKey_y] = PlayerInput::SHOOT;
    
    axis_mapping[InputKeyCode::EKey_t] = 1;
    axis_mapping[InputKeyCode::EKey_f] = -1;
    axis_mapping[InputKeyCode::EKey_g] = -1;
    axis_mapping[InputKeyCode::EKey_h] = 1;
    
    // Player 4
    inputMapping[4][InputKeyCode::EKey_i] = PlayerInput::Y_AXIS;
    inputMapping[4][InputKeyCode::EKey_j] = PlayerInput::X_AXIS;
    inputMapping[4][InputKeyCode::EKey_k] = PlayerInput::Y_AXIS;
    inputMapping[4][InputKeyCode::EKey_l] = PlayerInput::X_AXIS;
    inputMapping[4][InputKeyCode::EKey_u] = PlayerInput::GRAB;
    inputMapping[4][InputKeyCode::EKey_o] = PlayerInput::SHOOT;

    axis_mapping[InputKeyCode::EKey_i] = 1;
    axis_mapping[InputKeyCode::EKey_j] = -1;
    axis_mapping[InputKeyCode::EKey_k] = -1;
    axis_mapping[InputKeyCode::EKey_l] = 1;

    // Controller
    inputMapping[1][InputKeyCode::EController_x_axis] = PlayerInput::X_AXIS;
    inputMapping[1][InputKeyCode::EController_a] = PlayerInput::Y_AXIS;
    inputMapping[1][InputKeyCode::EController_x] = PlayerInput::SHOOT;
    inputMapping[1][InputKeyCode::EController_b] = PlayerInput::GRAB;

    inputMapping[2][InputKeyCode::EController_x_axis] = PlayerInput::X_AXIS;
    inputMapping[2][InputKeyCode::EController_a] = PlayerInput::Y_AXIS;
    inputMapping[2][InputKeyCode::EController_x] = PlayerInput::SHOOT;
    inputMapping[2][InputKeyCode::EController_b] = PlayerInput::GRAB;

    inputMapping[3][InputKeyCode::EController_x_axis] = PlayerInput::X_AXIS;
    inputMapping[3][InputKeyCode::EController_a] = PlayerInput::Y_AXIS;
    inputMapping[3][InputKeyCode::EController_x] = PlayerInput::SHOOT;
    inputMapping[3][InputKeyCode::EController_b] = PlayerInput::GRAB;

    inputMapping[4][InputKeyCode::EController_x_axis] = PlayerInput::X_AXIS;
    inputMapping[4][InputKeyCode::EController_a] = PlayerInput::Y_AXIS;
    inputMapping[4][InputKeyCode::EController_x] = PlayerInput::SHOOT;
    inputMapping[4][InputKeyCode::EController_b] = PlayerInput::GRAB;

    std::unordered_map<PlayerInput, double> time_to_wait_mapping;
    time_to_wait_mapping[PlayerInput::GRAB] = 0.1;
    time_to_wait_mapping[PlayerInput::MOUSE_LEFT] = 0.1;
    time_to_wait_mapping[PlayerInput::SPEED_DOWN] = 0.1;
    time_to_wait_mapping[PlayerInput::SPEED_UP] = 0.1;
    time_to_wait_mapping[PlayerInput::SPEED_RESET] = 0.1;

    input.setInputMapping(inputMapping, time_to_wait_mapping, axis_mapping);
}

void GameController::gameLoop() {
#ifdef PERFORMANCE_DEBUGGING
    double totalTime = 0.0;
    const int FPS_HISTORY_MAX = 500;
    std::deque<int> fps_history { FPS_HISTORY_MAX };
#endif // PERFORMANCE_DEBUGGING

    while(!should_quit) {
        auto start_time = std::chrono::high_resolution_clock::now();

        BrickInput<PlayerInput>::getInstance().processInput(delta_time);

        engine->getRenderer()->clearScreen();

        delta_time *= *delta_time_modifier.get();

        for (auto& system : game_state_manager->getSystems()) {
            system->update(delta_time);
        }

        engine->drawFpsCounter();
        engine->getRenderer()->drawScreen();

#ifdef PERFORMANCE_DEBUGGING
        CollisionDetector2CacheInfo collision_cache_info = collision_detector->getCacheInfo();
        auto entities_with_colliders = entityManager->getEntitiesByComponent<RectangleColliderComponent>();
        std::cout << "Collision Detector 2 © - continuous calculations: " << collision_cache_info.continuous_calculations_counter << std::endl;
        std::cout << "Collision Detector 2 © - continuous cache hits: " << collision_cache_info.continuous_cache_hits * entities_with_colliders.size() << std::endl;
        std::cout << "Collision Detector 2 © - discrete calculations: " << collision_cache_info.discrete_calculated_counter << std::endl;
        std::cout << "Collision Detector 2 © - discrete cache hits:  " << collision_cache_info.discrete_cache_hits * entities_with_colliders.size() << std::endl;
        std::cout << "FPS: " << engine->getFps() << std::endl;
        std::cout << "Entities with colliders: " << entities_with_colliders.size() << std::endl;
        std::cout << "Total running time: " << totalTime << std::endl;
        int total = 0;
        for (int& fps : fps_history) {
            total += fps;
        }
        std::cout << "Average FPS: " << total / fps_history.size() << std::endl;
#endif // PERFORMANCE_DEBUGGING

        collision_detector->invalidateCache();

        auto end_time = std::chrono::high_resolution_clock::now();
        engine->delay(start_time, end_time);
        delta_time = engine->getDeltatime();

#ifdef PERFORMANCE_DEBUGGING
        totalTime += delta_time;
        if (fps_history.size() >= FPS_HISTORY_MAX)
            fps_history.pop_front();
        fps_history.push_back(engine->getFps());
#endif // PERFORMANCE_DEBUGGING
    }
    engine->getSoundManager().stopMusic();
    engine->stop();
}

SceneManager<GameState>& GameController::getSceneManager() const {
    return *scene_manager.get();
}

int GameController::getScreenWidth() const {
    return SCREEN_WIDTH;
}

int GameController::getScreenHeight() const {
    return SCREEN_HEIGHT;
}

void GameController::loadLobby() {
    scene_manager->destroyAllScenes();
    scene_manager->createScene<Lobby>(*entityFactory, *engine, *this);
}

void GameController::startGame() {
    auto player_components = entityManager->getEntitiesByComponent<PlayerComponent>();
    if(player_components.size() >= 2) {
        loadLevels();
        loadNextLevel();
    }
}

void GameController::exitGame() {
    should_quit = true;
}

void GameController::loadLevels() {
    // Create list of levels
    std::string levels_path = "assets/levels";
    std::vector<std::string> levels;
    std::vector<std::string> temp_levels;
    for (const auto & entry : std::filesystem::directory_iterator(levels_path))
        levels.push_back(entry.path());

    // Fill queue randomly with levels
    temp_levels = levels;
    auto& r = Random::getInstance();
    while(level_queue.size() != MAX_LEVELS) {
        int random = r.getRandomInt(0, temp_levels.size() - 1);
        level_queue.push(temp_levels.at(random));
        temp_levels.erase(temp_levels.begin() + random);

        // If there are not enough levels
        if(level_queue.size() < MAX_LEVELS && temp_levels.empty()) {
            temp_levels = levels;
        }
    }
}

void GameController::loadNextLevel() {
    // Remove the current scene
    scene_manager->destroyScene(SceneLayer::Primary);
    // Remove the current scene
    if (scene_manager->isSceneActive<IntermissionScene>())
        scene_manager->destroyScene(IntermissionScene::getLayerStatic());

    // Remove the current scene
    if(!level_queue.empty()) {
        // load from queue
        std::string path = level_queue.front();
        level_queue.pop();

        // Create the level
        Json level_json { path, true };
        scene_manager->createScene<LevelScene>(*entityFactory, *engine, level_json);
    } else {
        // There are no levels left in the queue.
        loadEndGameLevel();
    }
}

void GameController::intermission(int timer) {
    scene_manager->destroyScene(SceneLayer::Secondary);
    scene_manager->createScene<IntermissionScene>(timer, *entityFactory);
}

void GameController::loadMainMenu() {
    scene_manager->destroyAllScenes();
    scene_manager->createScene<MainMenu>(*entityFactory, *engine, *this);
}

void GameController::loadHelp() {
    scene_manager->destroyAllScenes();
    scene_manager->createScene<HelpScene>(*entityFactory, *engine, *this);
}

void GameController::loadCredits() {
    scene_manager->destroyAllScenes();
    scene_manager->createScene<CreditsScene>(*entityFactory, *engine, *this);
}

void GameController::loadEndGameLevel() {
    scene_manager->destroyScene(SceneLayer::Primary);
    // Create scores
    auto players = entityManager->getEntitiesByComponent<PlayerComponent>();
    std::unordered_map<std::string, Score> scores;
    for(auto [entity_id, player] : players) {
        auto stats = entityManager->getComponent<StatsComponent>(entity_id);
        Score score;
        score.accidents = stats->accidents;
        score.deaths = stats->deaths;
        score.killed_critters = stats->killed_critters;
        score.kills = stats->kills;
        score.levels_won = stats->levels_won;
        scores.insert_or_assign(player->name, score);
    }
    score_json->writeScores(scores);
    scene_manager->createScene<EndScene>(*entityFactory, *engine);
}

void GameController::showHighscores() {
    scene_manager->destroyAllScenes();
    scene_manager->createScene<HighscoreScene>(*entityManager, *entityFactory, *engine, *score_json, *this);
}