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

GameController::GameController(){
    this->delta_time = 1;
    this->fps_cap = 120;
    this->fps_frame_time = 1000 / this->fps_cap;
}

int GameController::calculateFps(BrickEngine* engine, int start_time){
    this->delta_time = engine->getTicks() - start_time;
    if (this->delta_time == 0)
        this->delta_time = 1;
    if (this->delta_time < this->fps_frame_time) {
        double delay = this->fps_frame_time - this->delta_time;
        engine->delay(delay);
        this->delta_time += delay;
    }
    
    // The FPS cannot be calculated correctly because getTicks returns and integer of miliseconds. 
    // 1000 / 60 = 16.666666. But we can only caluclate 1000 / 16 = 62.5 where 0.5 is lost because of this integer precision issue.
    double fps = 1000.0 / this->delta_time;
    return (int)fps;
}

void GameController::createFpsCounter(int fps) {
    auto dst = std::unique_ptr<Rect>(new Rect { 0, 0, 75, 75});
    this->fps_counter.~unique_ptr();
    this->fps_counter = this->renderable_factory->createText(std::to_string(fps), 24, { 255, 255, 255, 0}, this->top_layer, std::move(dst));
}

void GameController::gameLoop() {
    BrickEngine engine = BrickEngine("Beast Arena", 1280, 720, { 1, 2, 3 });
    this->renderable_factory = engine.getRenderableFactory();
    // Always draw the FPS counter on the highest layer
    this->top_layer = 2;
    this->createFpsCounter(0);

    auto dst_max = std::unique_ptr<Rect>(new Rect { 0, 0, 1000, 330});
    std::unique_ptr<Renderable> makker_max = renderable_factory->createImage("./assets/graphics/verstappen_laughing.bmp", 1, std::move(dst_max));

    auto text = this->renderable_factory->createText("TESTEST", 200, { 255, 255, 255, 0 }, this->top_layer, std::unique_ptr<Rect>(new Rect {0, 0, 400, 400}));

    auto circle = renderable_factory->createCircle(600, 350, 300, true, { 255, 255, 1, 200 }, 2);
    auto line = renderable_factory->createLine(50, 600, 50, 50, { 255, 20, 255, 200 }, 2);

    Renderer* renderer = engine.getRenderer();

    while(true) {
        int start_time = engine.getTicks();

        renderer->clearScreen();
        renderer->queueRenderable(line.get());
        renderer->queueRenderable(circle.get());
        renderer->queueRenderable(fps_counter.get());
        renderer->queueRenderable(text.get());
        renderer->queueRenderable(makker_max.get());
        renderer->drawScreen();

        int fps = this->calculateFps(&engine, start_time);
        this->createFpsCounter(fps);
    }
}