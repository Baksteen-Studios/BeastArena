#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
using namespace std::chrono_literals;

#include "controllers/game_controller.hpp"
#include "brickengine/engine.hpp"
#include "brickengine/rendering/renderer.hpp"
#include "brickengine/rendering/renderables/renderable.hpp"
#include "brickengine/rendering/renderable_factory.hpp"

GameController::GameController() {
    this->delta_time = 0;
    this->fps_cap = 120;
    //In nanoseconds for precision
    this->fps_frame_time = 1'000'000'000 / this->fps_cap;
    this->top_layer = 2;
    this->layers = { 1, 2, 3 };

    engine = std::make_unique<BrickEngine>("Beast Arena", 1280, 720, layers);

    createSystems();
    createFpsCounter(0);
}

int GameController::calculateFps(std::chrono::time_point<std::chrono::high_resolution_clock> start_time){
    auto end_time = std::chrono::high_resolution_clock::now();
    auto delta_time_in_nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
    int64_t delay = fps_frame_time - delta_time_in_nanoseconds;
    //Updating deltatime for ECS
    delta_time = (delta_time_in_nanoseconds + delay) / 1'000'000'000.0;
    std::this_thread::sleep_for(std::chrono::nanoseconds(delay));
    return 1'000'000'000 / (delta_time_in_nanoseconds + delay);
}

void GameController::createFpsCounter(int fps) {
    auto dst = std::unique_ptr<Rect>(new Rect { 0, 0, 75, 75});
    this->fps_counter.~unique_ptr();
    this->fps_counter = engine->getRenderableFactory()->createText(std::to_string(fps), 24, { 255, 255, 255, 0}, this->top_layer, std::move(dst));
}

void GameController::createSystems() {
    this->systems = std::vector<std::unique_ptr<System>>();
}

void GameController::gameLoop() {
    while(true) {
        std::chrono::time_point start_time = std::chrono::high_resolution_clock::now();
        engine->getRenderer()->clearScreen();

        for (auto& system : systems) {
            system->update(delta_time);
        }

        engine->getRenderer()->queueRenderable(fps_counter.get());
        engine->getRenderer()->drawScreen();
        int fps = this->calculateFps(start_time);
        this->createFpsCounter(fps);
    }
}