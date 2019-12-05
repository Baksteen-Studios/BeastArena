#include "scenes/pause_scene.hpp"

#include "entities/layers.hpp"

PauseScene::PauseScene(EntityFactory& factory, BrickEngine& engine) : Menu<PauseScene>(factory, engine, WIDTH, HEIGHT) {};

void PauseScene::performPrepare(){
    // Nothing has to changes here as no player or other entities have to be reset to another 'state' or position.
}

void PauseScene::start() {
    factory.createImage("menu/check.png", 300, 300, 300, 300, Layers::Foreground, 255);
}

void PauseScene::leave() {

}